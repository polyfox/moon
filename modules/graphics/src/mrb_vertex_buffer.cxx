#include <mruby.h>
#include <mruby/class.h>
#include <mruby/data.h>
#include <mruby/numeric.h>
#include <mruby/string.h>
#include "moon/mrb/vbo.hxx"
#include "moon/mrb/helpers.hxx"
#include "moon/vertex_buffer.hxx"
#include "moon/api.h"

static void
vbo_free(mrb_state *mrb, void *p)
{
  Moon::VertexBuffer *vbo = (Moon::VertexBuffer*)p;
  if (vbo) {
    delete(vbo);
  }
}

MOON_C_API const struct mrb_data_type vbo_data_type = { "Moon::VertexBuffer", vbo_free };

static mrb_value
vbo_initialize(mrb_state *mrb, mrb_value self)
{
  GLenum usage;
  Moon::VertexBuffer *vbo;
  mrb_get_args(mrb, "i", &usage);

  vbo_free(mrb, DATA_PTR(self));
  DATA_PTR(self) = NULL;

  vbo = new Moon::VertexBuffer(usage);
  mrb_data_init(self, vbo, &vbo_data_type);
  return self;
}

static mrb_value
vbo_clear(mrb_state *mrb, mrb_value self)
{
  get_vbo(mrb, self)->Clear();
  return self;
}

static mrb_value
vbo_render(mrb_state *mrb, mrb_value self)
{
  GLenum mode;
  GLuint offset;
  offset = 0;
  mrb_get_args(mrb, "i|i", &mode, &offset);

  get_vbo(mrb, self)->Render(mode, offset);
  return self;
}

static mrb_value
vbo_push_back(mrb_state *mrb, mrb_value self)
{
  Moon::Vector2 *pos;
  Moon::Vector2 *tex_coord;
  Moon::Vector4 *color;
  mrb_get_args(mrb, "ddd",
    &pos, &vbo_data_type,
    &tex_coord, &vbo_data_type,
    &color, &vbo_data_type
  );
  get_vbo(mrb, self)->PushBack(Moon::Vertex(*pos, *tex_coord, *color));
  return self;
}

static mrb_value
vbo_push_indices(mrb_state *mrb, mrb_value self)
{
  mrb_int length;
  mrb_value *values;
  mrb_get_args(mrb, "a", &values, &length);
  // hell no, am I gonna malloc an array for this just to push it at once.
  // its a bit ugly to be resizing it each time though...
  for (int i = 0; i < length; ++i) {
    GLuint index = mrb_int(mrb, values[i]);
    get_vbo(mrb, self)->PushBackIndices(&index, 1);
  }
  return self;
}

static mrb_value
vbo_vertex_count(mrb_state *mrb, mrb_value self)
{
  return mrb_fixnum_value(get_vbo(mrb, self)->GetVertexCount());
}

static mrb_value
vbo_index_count(mrb_state *mrb, mrb_value self)
{
  return mrb_fixnum_value(get_vbo(mrb, self)->GetIndexCount());
}

MOON_C_API void
mmrb_vbo_init(mrb_state *mrb, struct RClass* mod)
{
  struct RClass *vbo_class = mrb_define_class_under(mrb, mod, "VertexBuffer", mrb->object_class);
  MRB_SET_INSTANCE_TT(vbo_class, MRB_TT_DATA);

  mrb_define_method(mrb, vbo_class, "initialize",   vbo_initialize,   MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vbo_class, "clear",        vbo_clear,        MRB_ARGS_NONE());
  mrb_define_method(mrb, vbo_class, "render",       vbo_render,       MRB_ARGS_REQ(1) | MRB_ARGS_OPT(1));
  mrb_define_method(mrb, vbo_class, "push_back",    vbo_push_back,    MRB_ARGS_REQ(3));
  mrb_define_method(mrb, vbo_class, "push_indices", vbo_push_indices, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vbo_class, "vertex_count", vbo_vertex_count, MRB_ARGS_NONE());
  mrb_define_method(mrb, vbo_class, "index_count",  vbo_index_count,  MRB_ARGS_NONE());

  // vbo.render(mode, {offset: [default: 0]})
  // vbo.flush // vbo.upload

  // vbo.push(vertices, indices)
  // vbo.push_vertices(v)
  // vbo.push_indices(i)
}
