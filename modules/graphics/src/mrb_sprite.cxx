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
#include "moon/mrb/vector2.hxx"
#include "moon/mrb/vector3.hxx"
#include "moon/mrb/vector4.hxx"
#include "moon/mrb/helpers.hxx"
#include "moon/mrb_err.hxx"
#include "moon/glm.h"

static mrb_value
sprite_generate_buffers(mrb_state *mrb, mrb_value self)
{
  Moon::Texture *texture = get_valid_texture(mrb, moon_iv_get(mrb, self, KEY_TEXTURE));
  Moon::VertexBuffer *vbo = get_vbo(mrb, moon_iv_get(mrb, self, KEY_VBO));
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
  const GLfloat opacity = mrb_to_flo(mrb, moon_iv_get(mrb, self, "@opacity"));
  const GLfloat angle = mrb_to_flo(mrb, moon_iv_get(mrb, self, "@angle"));
  Moon::Texture *texture = get_valid_texture(mrb, moon_iv_get(mrb, self, KEY_TEXTURE));
  Moon::Shader *shader = get_shader(mrb, moon_iv_get(mrb, self, KEY_SHADER));
  Moon::VertexBuffer *vbo = get_vbo(mrb, moon_iv_get(mrb, self, KEY_VBO));
  Moon::Vector2 origin(*get_vector2(mrb, moon_iv_get(mrb, self, KEY_ORIGIN)));
  Moon::Vector4 color(*get_vector4(mrb, moon_iv_get(mrb, self, "@color")));
  Moon::Vector4 tone(*get_vector4(mrb, moon_iv_get(mrb, self, "@tone")));

  shader->Use();

  glm::mat4 rotation_matrix = moon_rotate(angle, origin);
  // model matrix - move it to the correct position in the world
  glm::mat4 model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
  // calculate the ModelViewProjection matrix (faster to do on CPU, once for all vertices instead of per vertex)
  glm::mat4 mvp_matrix = Moon::Shader::projection_matrix * Moon::Shader::view_matrix * model_matrix * rotation_matrix;
  shader->SetUniform("mvp_matrix", mvp_matrix);

  shader->SetUniform("opacity", opacity);
  shader->SetUniform("color",  color);
  shader->SetUniform("tone", tone);

  //Set texture ID
  glActiveTexture(GL_TEXTURE0);
  texture->Bind();
  shader->SetUniform("tex", /*GL_TEXTURE*/0);
  vbo->Render(GL_TRIANGLE_STRIP);
  return self;
}

MOON_C_API void
mmrb_sprite_init(mrb_state *mrb, struct RClass* mod)
{
  struct RClass *sprite_class = mrb_define_class_under(mrb, mod, "Sprite", mrb->object_class);
  mrb_define_method(mrb, sprite_class, "generate_buffers", sprite_generate_buffers, MRB_ARGS_NONE());
  mrb_define_method(mrb, sprite_class, "render",           sprite_render,           MRB_ARGS_REQ(3));
}
