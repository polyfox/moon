#include <mruby.h>
#include <mruby/array.h>
#include <mruby/class.h>
#include <mruby/data.h>
#include <mruby/error.h>
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
#include "moon/glm.h"

static mrb_sym id_opacity;
static mrb_sym id_tone;
static mrb_sym id_color;
static mrb_sym id_ox;
static mrb_sym id_oy;
static mrb_sym id_angle;
static mrb_sym id_transform;

struct RenderState {
  GLfloat opacity;
  GLfloat angle;
  Moon::Vector2 origin;
  Moon::Vector4 color;
  Moon::Vector4 tone;
  Moon::Transform transform;

  RenderState() :
    opacity(1.0),
    angle(0.0),
    origin(0.0, 0.0),
    color(1.0, 1.0, 1.0, 1.0),
    tone(0.0, 0.0, 0.0, 1.0) {};
};

static inline Moon::Texture*
get_valid_texture(mrb_state *mrb, mrb_value obj)
{
  Moon::Texture *texture = get_texture(mrb, obj);
  if (!texture->GetID()) {
    mrb_raisef(mrb, E_ARGUMENT_ERROR, "invalid texture handle.");
  }
  return texture;
}

static mrb_value
spritesheet_generate_buffers(mrb_state *mrb, mrb_value self)
{
  Moon::Texture *texture = get_valid_texture(mrb, IVget(KEY_TEXTURE));
  Moon::VertexBuffer *vbo = get_vbo(mrb, IVget(KEY_VBO));
  const GLuint tile_width = mrb_fixnum(IVget("@w"));
  const GLuint tile_height = mrb_fixnum(IVget("@h"));
  const GLfloat tiles_per_row = texture->GetWidth() / tile_width;
  const GLfloat tiles_per_column = texture->GetHeight() / tile_height;
  const GLuint total_sprites = tiles_per_row * tiles_per_column;

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

    Moon::Vertex vertices[4] = {
      { {0.f, 0.f},                {s0, t0}, Moon::Vector4(0, 0, 0, 0) },
      { {tile_width, 0.f},         {s1, t0}, Moon::Vector4(0, 0, 0, 0) },
      { {tile_width, tile_height}, {s1, t1}, Moon::Vector4(0, 0, 0, 0) },
      { {0.f, tile_height},        {s0, t1}, Moon::Vector4(0, 0, 0, 0) }
    };

    vbo->PushBackVertices(vertices, 4);
  }
  GLuint indices[4] = {0, 1, 3, 2};
  vbo->PushBackIndices(indices, 4);
  return self;
}

static void
render(mrb_state *mrb, mrb_value self, const glm::vec3 position,
    const int index, const RenderState &render_ops) {
  const int total_sprites = mrb_int(mrb, IVget("@cell_count"));
  if ((index < 0) || (index >= total_sprites)) {
    mrb_raisef(mrb, E_ARGUMENT_ERROR, "sprite index is out of range.");
  }

  const int offset = index * 4;
  mrb_value texture_obj = IVget(KEY_TEXTURE);
  mrb_value shader_obj = IVget(KEY_SHADER);
  mrb_value vbo_obj = IVget(KEY_VBO);
  if (mrb_nil_p(texture_obj)) {
    mrb_raisef(mrb, E_TYPE_ERROR, "cannot render with a `nil` @texture");
  }
  if (mrb_nil_p(shader_obj)) {
    mrb_raisef(mrb, E_TYPE_ERROR, "cannot render with a `nil` @shader");
  }
  if (mrb_nil_p(vbo_obj)) {
    mrb_raisef(mrb, E_TYPE_ERROR, "cannot render with a `nil` @vbo");
  }
  Moon::Texture *texture = get_valid_texture(mrb, texture_obj);
  Moon::Shader *shader = get_shader(mrb, shader_obj);
  Moon::VertexBuffer *vbo = get_vbo(mrb, vbo_obj);

  shader->Use();

  //model matrix - move it to the correct position in the world
  Moon::Vector2 origin = render_ops.origin;
  glm::mat4 model_matrix = glm::translate(render_ops.transform, position);
  glm::mat4 rotation_matrix = glm::translate(glm::rotate(
        glm::translate(glm::mat4(1.0f), glm::vec3(origin.x, origin.y, 0)),
        glm::radians(render_ops.angle),
        glm::vec3(0, 0, 1)
        ), glm::vec3(-origin.x, -origin.y, 0));

  // calculate the ModelViewProjection matrix (faster to do on CPU, once for all vertices instead of per vertex)
  glm::mat4 mvp_matrix = Moon::Shader::projection_matrix * Moon::Shader::view_matrix * model_matrix * rotation_matrix;
  glUniformMatrix4fv(shader->GetUniform("mvp_matrix"), 1, GL_FALSE, glm::value_ptr(mvp_matrix));

  glUniform1f(shader->GetUniform("opacity"), render_ops.opacity);
  glUniform4fv(shader->GetUniform("tone"), 1, glm::value_ptr(render_ops.tone));
  glUniform4fv(shader->GetUniform("color"), 1, glm::value_ptr(render_ops.color));

  //Set texture ID
  glActiveTexture(GL_TEXTURE0);
  texture->Bind();
  glUniform1i(shader->GetUniform("tex"), /*GL_TEXTURE*/0);

  vbo->RenderWithOffset(GL_TRIANGLE_STRIP, offset);
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
  RenderState render_op;
  mrb_get_args(mrb, "fffi|H", &x, &y, &z, &index, &options);
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
  render(mrb, self, glm::vec3(x, y, z), index, render_op);
  return mrb_nil_value();
}

MOON_C_API void
mmrb_spritesheet_init(mrb_state *mrb, struct RClass* mod)
{
  struct RClass *spritesheet_class = mrb_define_class_under(mrb, mod, "Spritesheet", mrb->object_class);

  mrb_define_method(mrb, spritesheet_class, "render",           spritesheet_render,         MRB_ARGS_ARG(4,1));
  mrb_define_method(mrb, spritesheet_class, "generate_buffers", spritesheet_generate_buffers,    MRB_ARGS_NONE());

  id_opacity   = mrb_intern_cstr(mrb, "opacity");
  id_tone      = mrb_intern_cstr(mrb, "tone");
  id_color     = mrb_intern_cstr(mrb, "color");
  id_ox        = mrb_intern_cstr(mrb, "ox");
  id_oy        = mrb_intern_cstr(mrb, "oy");
  id_angle     = mrb_intern_cstr(mrb, "angle");
  id_transform = mrb_intern_cstr(mrb, "transform");
}
