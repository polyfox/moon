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
#include "moon/mrb/matrix4.hxx"
#include "moon/mrb/texture.hxx"
#include "moon/mrb_err.hxx"
#include "moon/mrb/helpers.hxx"
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
  Moon::Matrix4 transform;

  RenderState() :
    opacity(1.0),
    angle(0.0),
    origin(0.0, 0.0),
    color(1.0, 1.0, 1.0, 1.0),
    tone(0.0, 0.0, 0.0, 1.0) {};
};

static mrb_value
spritesheet_generate_buffers(mrb_state *mrb, mrb_value self)
{
  Moon::Texture *texture = get_valid_texture(mrb, moon_iv_get(mrb, self, KEY_TEXTURE));
  Moon::VertexBuffer *vbo = get_vbo(mrb, moon_iv_get(mrb, self, KEY_VBO));
  const GLuint tile_width = mrb_fixnum(moon_iv_get(mrb, self, "@w"));
  const GLuint tile_height = mrb_fixnum(moon_iv_get(mrb, self, "@h"));
  const GLfloat tiles_per_row = texture->GetWidth() / tile_width;
  const GLfloat tiles_per_column = texture->GetHeight() / tile_height;
  const GLuint total_sprites = tiles_per_row * tiles_per_column;

  // set @cell_count
  IVset("@cell_count", mrb_fixnum_value(total_sprites));

  // TODO: clear out VBO if we're calling generate_buffers again
  // note: this is only called once in initialize, so it never needs clearing
  // right now

  for (GLuint i = 0; i < total_sprites; ++i) {
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
  const int total_sprites = mrb_int(mrb, moon_iv_get(mrb, self, "@cell_count"));
  if ((index < 0) || (index >= total_sprites)) {
    mrb_raisef(mrb, E_ARGUMENT_ERROR, "sprite index is out of range.");
  }
  const int offset = index * 4;
  Moon::Texture *texture = get_texture(mrb, moon_iv_get(mrb, self, KEY_TEXTURE));
  Moon::Shader *shader = get_shader(mrb, moon_iv_get(mrb, self, KEY_SHADER));
  Moon::VertexBuffer *vbo = get_vbo(mrb, moon_iv_get(mrb, self, KEY_VBO));

  shader->Use();

  glm::mat4 rotation_matrix = moon_rotate(render_ops.angle, render_ops.origin);
  //model matrix - move it to the correct position in the world
  glm::mat4 model_matrix = glm::translate(render_ops.transform, position);

  // calculate the ModelViewProjection matrix (faster to do on CPU, once for all vertices instead of per vertex)
  glm::mat4 mvp_matrix = Moon::Shader::projection_matrix * Moon::Shader::view_matrix * model_matrix * rotation_matrix;
  shader->SetUniform("mvp_matrix", mvp_matrix);

  shader->SetUniform("opacity", render_ops.opacity);
  shader->SetUniform("color", render_ops.color);
  shader->SetUniform("tone", render_ops.tone);

  //Set texture ID
  glActiveTexture(GL_TEXTURE0);
  texture->Bind();
  shader->SetUniform("tex", /*GL_TEXTURE*/0);

  vbo->Render(GL_TRIANGLE_STRIP, offset);
};

static void
set_render_options(mrb_state *mrb, mrb_value options, RenderState *render_state)
{
  mrb_value keys = mrb_hash_keys(mrb, options);
  int len = mrb_ary_len(mrb, keys);
  const mrb_value *keys_ary = RARRAY_PTR(keys);

  for (int i = 0; i < len; ++i) {
    mrb_value key = keys_ary[i];
    if (!mrb_symbol_p(key)) continue;

    mrb_value val = mrb_hash_get(mrb, options, key);
    // :opacity
    if (mrb_symbol(key) == id_opacity) {
      render_state->opacity = mrb_to_flo(mrb, val);

    // :color
    } else if (mrb_symbol(key) == id_color) {
      render_state->color = mmrb_to_vector4(mrb, val);

    // :tone
    } else if (mrb_symbol(key) == id_tone) {
      render_state->tone = mmrb_to_vector4(mrb, val);

    // :ox
    } else if (mrb_symbol(key) == id_ox) {
      render_state->origin.x = mrb_to_flo(mrb, val);

    // :oy
    } else if (mrb_symbol(key) == id_oy) {
      render_state->origin.y = mrb_to_flo(mrb, val);

    // :angle
    } else if (mrb_symbol(key) == id_angle) {
      render_state->angle = mrb_to_flo(mrb, val);

    // :transform
    } else if (mrb_symbol(key) == id_transform) {
      render_state->transform = mmrb_to_matrix4(mrb, val);
    }
  }
}

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
  RenderState render_state;
  mrb_get_args(mrb, "fffi|H", &x, &y, &z, &index, &options);
  if (!mrb_nil_p(options)) {
    set_render_options(mrb, options, &render_state);
  }
  render(mrb, self, glm::vec3(x, y, z), index, render_state);
  return self;
}

static mrb_value
spritesheet_push_quad(mrb_state *mrb, mrb_value self)
{
  RenderState render_state;
  Moon::VertexBuffer *dest_vbo;
  Moon::VertexBuffer *vbo;
  mrb_float x, y, z;
  mrb_value options = mrb_nil_value();
  mrb_int offset = 0;
  GLuint index;
  Moon::Vertex vertices[4];
  GLuint indices[] = { 0, 1, 3, 2, 3, 1 };
  mrb_get_args(mrb, "dfffi|H",
    &dest_vbo, &vbo_data_type,
    &x, &y, &z, &offset,
    &options
  );
  if (!mrb_nil_p(options)) {
    set_render_options(mrb, options, &render_state);
  }

  vbo = get_vbo(mrb, moon_iv_get(mrb, self, KEY_VBO));
  Moon::Vector2 pos(x, y);
  render_state.opacity = 1.0;
  index = offset * 4;
  // now apply render state changes
  for (int i = 0; i < 4; ++i) {
    vertices[i] = vbo->GetVertex(index + i);
    vertices[i].pos += pos;
    vertices[i].color.a *= render_state.opacity;
  }

  dest_vbo->PushBack(vertices, 4, indices, 6);
  return self;
}

MOON_C_API void
mmrb_spritesheet_init(mrb_state *mrb, struct RClass* mod)
{
  struct RClass *ss_cls = mrb_define_class_under(mrb, mod, "Spritesheet", mrb->object_class);

  mrb_define_method(mrb, ss_cls, "render",           spritesheet_render,           MRB_ARGS_ARG(4,1));
  mrb_define_method(mrb, ss_cls, "generate_buffers", spritesheet_generate_buffers, MRB_ARGS_NONE());
  mrb_define_method(mrb, ss_cls, "push_quad",        spritesheet_push_quad,        MRB_ARGS_ARG(5,1));

  id_opacity   = mrb_intern_cstr(mrb, "opacity");
  id_tone      = mrb_intern_cstr(mrb, "tone");
  id_color     = mrb_intern_cstr(mrb, "color");
  id_ox        = mrb_intern_cstr(mrb, "ox");
  id_oy        = mrb_intern_cstr(mrb, "oy");
  id_angle     = mrb_intern_cstr(mrb, "angle");
  id_transform = mrb_intern_cstr(mrb, "transform");
}
