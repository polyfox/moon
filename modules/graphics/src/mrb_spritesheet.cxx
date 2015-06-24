#include <mruby.h>
#include <mruby/array.h>
#include <mruby/class.h>
#include <mruby/data.h>
#include <mruby/hash.h>
#include <mruby/numeric.h>
#include <mruby/string.h>
#include "moon/api.h"
#include "moon/mrb/spritesheet.hxx"
#include "moon/mrb/vector3.hxx"
#include "moon/mrb/vector4.hxx"
#include "moon/mrb/transform.hxx"
#include "moon/mrb/texture.hxx"
#include "moon/mrb/renderable.hxx"
#include "moon/mrb_err.hxx"
#include "moon/spritesheet.hxx"
#include "moon/glm.h"

static mrb_sym id_opacity;
static mrb_sym id_tone;
static mrb_sym id_color;
static mrb_sym id_ox;
static mrb_sym id_oy;
static mrb_sym id_angle;
static mrb_sym id_transform;

static mrb_value
spritesheet_generate_buffers(mrb_state *mrb, mrb_value self)
{
  // TODO: check type of mrb_texture to be Moon::Texture
  //mrb_raisef(mrb, E_TYPE_ERROR,
  //                "wrong argument type %S (expected Moon::Texture)",
  //                mrb_str_new_cstr(mrb, mrb_obj_classname(mrb, obj)));
  Moon::Texture *texture = get_texture(mrb, IVget(KEY_TEXTURE));

  // TODO: generalize get_with_typecheck(from, to, expected_class)
  // that fetches the ivar, checks class == expected_class (throws a
  // E_TYPE_ERROR otherwise), then returns the val.

  if (texture->GetID() != 0) {
    mrb_raisef(mrb, E_TYPE_ERROR, "No texture to render with!");
  }

  // TODO: get C++ VBO object
  Moon::VertexBuffer *vbo = get_vbo(mrb, IVget(KEY_VBO));

  GLuint tile_width = mrb_fixnum(IVget("@w"));
  GLuint tile_height = mrb_fixnum(IVget("@h"));

  GLfloat tiles_per_row, tiles_per_column;

  tiles_per_row = texture->GetWidth() / tile_width;
  tiles_per_column = texture->GetHeight() / tile_height;

  GLuint total_sprites = tiles_per_row * tiles_per_column;
  // set @cell_count
  IVset("@cell_count", mrb_fixnum_value(total_sprites));

  // TODO: clear out VBO if we're calling generate_buffers again
  // note: this is only called once in initialize, so it never needs clearing
  // right now

  for(int i = 0; i < total_sprites; ++i) {
    GLfloat ox = (float)(i % (int)tiles_per_row);
    GLfloat oy = (float)(i / (int)tiles_per_row);

    float s0 = (ox) / tiles_per_row;
    float s1 = (ox + 1.0) / tiles_per_row;
    float t0 = (oy) / tiles_per_column;
    float t1 = (oy + 1.0) / tiles_per_column;

    Vertex vertices[4] = {
      { {0.f, 0.f},                {s0, t0}, Vector4(0, 0, 0, 0) },
      { {tile_width, 0.f},         {s1, t0}, Vector4(0, 0, 0, 0) },
      { {tile_width, tile_height}, {s1, t1}, Vector4(0, 0, 0, 0) },
      { {0.f, tile_height},        {s0, t1}, Vector4(0, 0, 0, 0) }
    };

    vbo.PushBackVertices(vertices, 4);
  }

  GLuint indices[4] = {0, 1, 3, 2};
  vbo.PushBackIndices(indices, 4);

  return self;
}

static void spritesheet_render(const float x, const float y, const float z,
    const int index, const Spritesheet::RenderState &render_ops) {

  // if you somehow managed to go out-of-bounds
  // TODO: raise errors for these instead of silently failing
  if ((index < 0) || (index >= (int)total_sprites)) return;
  if (m_texture->GetID() == 0) return;
  if (!shader) return;

  int offset = index * 4;

  shader->Use();

  //model matrix - move it to the correct position in the world
  Vector2 origin = render_ops.origin;
  glm::mat4 model_matrix = glm::translate(render_ops.transform, glm::vec3(x, y, z));
  glm::mat4 rotation_matrix = glm::translate(glm::rotate(
        glm::translate(glm::mat4(1.0f), glm::vec3(origin.x, origin.y, 0)),
        glm::radians(render_ops.angle),
        glm::vec3(0, 0, 1)
        ), glm::vec3(-origin.x, -origin.y, 0));

  // calculate the ModelViewProjection matrix (faster to do on CPU, once for all vertices instead of per vertex)
  glm::mat4 mvp_matrix = Shader::projection_matrix * Shader::view_matrix * model_matrix * rotation_matrix;
  glUniformMatrix4fv(shader->GetUniform("mvp_matrix"), 1, GL_FALSE, glm::value_ptr(mvp_matrix));

  glUniform1f(shader->GetUniform("opacity"), render_ops.opacity);
  glUniform4fv(shader->GetUniform("tone"), 1, glm::value_ptr(render_ops.tone));
  glUniform4fv(shader->GetUniform("color"), 1, glm::value_ptr(render_ops.color));

  //Set texture ID
  glActiveTexture(GL_TEXTURE0);
  m_texture->Bind();
  glUniform1i(shader->GetUniform("tex"), /*GL_TEXTURE*/0);

  m_vbo.RenderWithOffset(GL_TRIANGLE_STRIP, offset);
};
/*
 * @overload Spritesheet#render(x: float, y: float, z: float, index: int)
 * @overload Spritesheet#render(x: float, y: float, z: float, index: int, options: Hash<Symbol, Object>)
 */
static mrb_value
spritesheet_render(mrb_state *mrb, mrb_value self)
{
  mrb_int index;
  mrb_float x, y, z;
  mrb_value options = mrb_nil_value();
  mrb_get_args(mrb, "fffi|H", &x, &y, &z, &index, &options);
  Moon::Spritesheet *spritesheet = get_spritesheet(mrb, self);
  Moon::Spritesheet::RenderState render_op;
  if (!mrb_nil_p(options)) {
    mrb_value keys = mrb_hash_keys(mrb, options);
    int len = mrb_ary_len(mrb, keys);
    const mrb_value *keys_ary = RARRAY_PTR(keys);

    for (int i = 0; i < len; ++i) {
      mrb_value key = keys_ary[i];

      if (mrb_symbol_p(key)) {
        mrb_value val = mrb_hash_get(mrb, options, key);
        // :opacity
        if (mrb_symbol(key) == id_opacity) {
          render_op.opacity = mrb_to_flo(mrb, val);

        // :color
        } else if (mrb_symbol(key) == id_color) {
          render_op.color = mmrb_to_vector4(mrb, val);

        // :tone
        } else if (mrb_symbol(key) == id_tone) {
          render_op.tone = mmrb_to_vector4(mrb, val);

        // :ox
        } else if (mrb_symbol(key) == id_ox) {
          render_op.origin.x = mrb_to_flo(mrb, val);

        // :oy
        } else if (mrb_symbol(key) == id_oy) {
          render_op.origin.y = mrb_to_flo(mrb, val);

        // :angle
        } else if (mrb_symbol(key) == id_angle) {
          render_op.angle = mrb_to_flo(mrb, val);

        // :transform
        } else if (mrb_symbol(key) == id_transform) {
          render_op.transform = mmrb_to_transform(mrb, val);
        }
      }
    }
  }
  spritesheet_render(x, y, z, index, render_op);
  return mrb_nil_value();
}

MOON_C_API void
mmrb_spritesheet_init(mrb_state *mrb, struct RClass* mod)
{
  struct RClass *spritesheet_class = mrb_define_class_under(mrb, mod, "Spritesheet", mrb->object_class);

  mrb_define_method(mrb, spritesheet_class, "render",     spritesheet_render,         MRB_ARGS_ARG(4,1));

  mrb_define_method(mrb, spritesheet_class, "generate_buffers",    spritesheet_generate_buffers,    MRB_ARGS_NONE());
  mrb_define_method(mrb, spritesheet_class, "shader=",    renderable_shader_set<Moon::Spritesheet>,    MRB_ARGS_REQ(1));

  id_opacity   = mrb_intern_cstr(mrb, "opacity");
  id_tone      = mrb_intern_cstr(mrb, "tone");
  id_color     = mrb_intern_cstr(mrb, "color");
  id_ox        = mrb_intern_cstr(mrb, "ox");
  id_oy        = mrb_intern_cstr(mrb, "oy");
  id_angle     = mrb_intern_cstr(mrb, "angle");
  id_transform = mrb_intern_cstr(mrb, "transform");
}
