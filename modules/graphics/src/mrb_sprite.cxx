#include <mruby.h>
#include <mruby/array.h>
#include <mruby/class.h>
#include <mruby/data.h>
#include <mruby/hash.h>
#include <mruby/numeric.h>
#include <mruby/variable.h>
#include <mruby/string.h>
#include "moon/api.h"
#include "moon/mrb/sprite.hxx"
#include "moon/mrb/texture.hxx"
#include "moon/mrb/rect.hxx"
#include "moon/mrb/vector3.hxx"
#include "moon/mrb/vector4.hxx"
#include "moon/mrb_err.hxx"
#include "moon/sprite.hxx"
#include "moon/glm.h"
#include "moon/mrb/renderable.hxx"

static void
sprite_free(mrb_state *mrb, void *p)
{
  Moon::Sprite *sprite = static_cast<Moon::Sprite*>(p);
  if (sprite) {
    delete(sprite);
  }
}

MOON_C_API const struct mrb_data_type sprite_data_type = { "Moon::Sprite", sprite_free };

static inline Moon::Sprite*
get_sprite(mrb_state *mrb, mrb_value self)
{
  return static_cast<Moon::Sprite*>(mrb_data_get_ptr(mrb, self, &sprite_data_type));
}

static inline Moon::IntRect*
get_rect(mrb_state *mrb, mrb_value self)
{
  return static_cast<Moon::IntRect*>(mrb_data_get_ptr(mrb, self, &rect_data_type));
}

static inline Moon::Vector2*
get_vector2(mrb_state *mrb, mrb_value self)
{
  return static_cast<Moon::Vector2*>(mrb_data_get_ptr(mrb, self, &vector2_data_type));
}

static inline Moon::Vector4*
get_vector4(mrb_state *mrb, mrb_value self)
{
  return static_cast<Moon::Vector4*>(mrb_data_get_ptr(mrb, self, &vector4_data_type));
}

static mrb_value
sprite_generate_buffers(mrb_state *mrb, mrb_value self)
{
  Moon::Texture *texture = get_valid_texture(mrb, IVget(KEY_TEXTURE));
  Moon::VertexBuffer *vbo = get_vbo(mrb, IVget(KEY_VBO));
  mrb_value clip = IVget("@clip_rect");

  vbo->Clear();
  //Texture coordinates
  GLfloat s0 = 0.f;
  GLfloat s1 = 1.f;
  GLfloat t0 = 0.f;
  GLfloat t1 = 1.f;

  //Vertex coordinates
  GLfloat width = texture->GetWidth();
  GLfloat height = texture->GetHeight();

  //Handle clipping
  if (!mrb_nil_p(clip)) {
    // TODO: get C++ clip rect from mrb rect
    Moon::IntRect *clip_rect = get_rect(mrb, clip);

    //Texture coordinates
    s0 = (float)clip_rect->x / texture->GetWidth();
    s1 = (float)(clip_rect->x + clip_rect->w) / texture->GetWidth();
    t0 = (float)clip_rect->y / texture->GetHeight();
    t1 = (float)(clip_rect->y + clip_rect->h) / texture->GetHeight();
    //Vertex coordinates
    width = clip_rect->w;
    height = clip_rect->h;
  }

  Moon::Vertex vertices[4] = {
    { {0.f,   0.f},    {s0, t0}, Moon::Vector4(0, 0, 0, 0) },
    { {width, 0.f},    {s1, t0}, Moon::Vector4(0, 0, 0, 0) },
    { {width, height}, {s1, t1}, Moon::Vector4(0, 0, 0, 0) },
    { {0.f,   height}, {s0, t1}, Moon::Vector4(0, 0, 0, 0) }
  };
  GLuint indices[4] = {0, 1, 3, 2}; // rendering indices
  vbo->PushBack(vertices, 4, indices, 4);

  return self;
}

static mrb_value
sprite_render(mrb_state *mrb, mrb_value self)
{
  mrb_float x, y, z;
  mrb_get_args(mrb, "fff", &x, &y, &z);

  // TODO: extract texture, shader, origin, angle, color, tone and opacity

  if (!texture) return;
  if (!shader) return;

  shader->Use();
  // rotation matrix - rotate the model around specified origin
  // really ugly, we translate the rotation origin to 0,0, rotate,
  // then translate back to original position
  glm::mat4 rotation_matrix = glm::translate(glm::rotate(
        glm::translate(glm::mat4(1.0f), glm::vec3(origin.x, origin.y, 0)),
        glm::radians(angle),
        glm::vec3(0, 0, 1)
        ), glm::vec3(-origin.x, -origin.y, 0));
  // model matrix - move it to the correct position in the world
  glm::mat4 model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
  // calculate the ModelViewProjection matrix (faster to do on CPU, once for all vertices instead of per vertex)
  glm::mat4 mvp_matrix = Moon::Shader::projection_matrix * Moon::Shader::view_matrix * model_matrix * rotation_matrix;
  glUniformMatrix4fv(shader->GetUniform("mvp_matrix"), 1, GL_FALSE, glm::value_ptr(mvp_matrix));
  glUniform1f(shader->GetUniform("opacity"), opacity);
  glUniform4fv(shader->GetUniform("color"), 1, glm::value_ptr(color));
  glUniform4fv(shader->GetUniform("tone"), 1, glm::value_ptr(tone));
  //Set texture ID
  glActiveTexture(GL_TEXTURE0);
  texture->Bind();
  glUniform1i(shader->GetUniform("tex"), /*GL_TEXTURE*/0);
  vbo.Render(GL_TRIANGLE_STRIP);
  return self;
}

static mrb_value
sprite_opacity_set(mrb_state *mrb, mrb_value self)
{
  mrb_float f;
  mrb_get_args(mrb, "f", &f);
  get_sprite(mrb, self)->opacity = glm::clamp(f, 0.0, 1.0);
  return self;
}

MOON_C_API void
mmrb_sprite_init(mrb_state *mrb, struct RClass* mod)
{
  struct RClass *sprite_class = mrb_define_class_under(mrb, mod, "Sprite", mrb->object_class);
  MRB_SET_INSTANCE_TT(sprite_class, MRB_TT_DATA);

  mrb_define_method(mrb, sprite_class, "generate_buffers", sprite_generate_buffers,    MRB_ARGS_NONE());
  mrb_define_method(mrb, sprite_class, "render",           sprite_render,        MRB_ARGS_REQ(3));
}
