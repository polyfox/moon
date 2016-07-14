#include <mruby.h>
#include <mruby/class.h>
#include <mruby/data.h>
#include <mruby/numeric.h>
#include <mruby/string.h>
#include "moon/mrb/rect.hxx"
#include "moon/mrb/vector2.hxx"
#include "moon/mrb/vector3.hxx"
#include "moon/mrb/vector4.hxx"
#include "moon/mrb/vertex_buffer.hxx"
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

/* Creates a new buffer to store vertex and index data on the GPU.
 * @param [Integer] usage Intended usage mode that hints to OpenGL on how to
 * best store the data.
 *
 * @return [VertexBuffer]
 */
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

/* Clears the buffer.
 *
 * @return [self]
 */
static mrb_value
vbo_clear(mrb_state *mrb, mrb_value self)
{
  mmrb_vertex_buffer_ptr(mrb, self)->Clear();
  return self;
}

/* Binds the VAO and renders the buffer.
 * We can optionally specify an offset which "is added to each index before
 * pulling from the vertex data". That makes it easy to just use the same IBO,
 * but use it to render different meshes.
 *
 * One usecase for that is in the Spritesheet, we use a 1,3,2,4 IBO with a VBO
 * for all the different tiles. Simply specifying the tile index as the offset
 * will render the correct tile.
 *
 * @param [Integer] mode OpenGL mode to use for rendering.
 * @param [Integer] offset optional offset into the vertex data
 * @return [self]
 */
static mrb_value
vbo_render(mrb_state *mrb, mrb_value self)
{
  GLenum mode;
  GLuint offset;
  offset = 0;
  mrb_get_args(mrb, "i|i", &mode, &offset);

  mmrb_vertex_buffer_ptr(mrb, self)->Render(mode, offset);
  return self;
}

/**
 * Adds a single vertex to the buffer
 *
 * @param [Vector2] pos
 * @param [Vector2] tex_coord
 * @param [Vector4] color
 * @return [self]
 */
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
  mmrb_vertex_buffer_ptr(mrb, self)->PushBack(Moon::Vertex(*pos, *tex_coord, *color));
  return self;
}

/**
 * Pushes the list of indices into the buffer
 *
 * @param [Array<Integer>] indices
 * @return [self]
 */
static mrb_value
vbo_push_indices(mrb_state *mrb, mrb_value self)
{
  mrb_int length;
  mrb_value *indices;
  mrb_get_args(mrb, "a", &indices, &length);
  // hell no, am I gonna malloc an array for this just to push it at once.
  // its a bit ugly to be resizing it each time though...
  for (int i = 0; i < length; ++i) {
    GLuint index = mrb_int(mrb, indices[i]);
    mmrb_vertex_buffer_ptr(mrb, self)->PushBackIndices(&index, 1);
  }
  return self;
}

/* @return [Integer] Returns the number of VBO vertices stored in the buffer.
*/
static mrb_value
vbo_vertex_count(mrb_state *mrb, mrb_value self)
{
  return mrb_fixnum_value(mmrb_vertex_buffer_ptr(mrb, self)->GetVertexCount());
}

/* @return [Integer] Returns the number of IBO indices stored in the buffer.
*/
static mrb_value
vbo_index_count(mrb_state *mrb, mrb_value self)
{
  return mrb_fixnum_value(mmrb_vertex_buffer_ptr(mrb, self)->GetIndexCount());
}

static void
make_quad(Moon::Vertex vertices[4], Moon::IntRect& quad_rect, Moon::FloatRect& quad_texture_rect, Moon::Vector4& color)
{
  GLfloat x0, x1, y0, y1;
  GLfloat tx0, tx1, ty0, ty1;

  x0 = quad_rect.x;
  y0 = quad_rect.y;
  x1 = quad_rect.x + quad_rect.w;
  y1 = quad_rect.y + quad_rect.h;

  tx0 = quad_texture_rect.x;
  ty0 = quad_texture_rect.y;
  tx1 = quad_texture_rect.x + quad_texture_rect.w;
  ty1 = quad_texture_rect.y + quad_texture_rect.h;

  vertices[0] = (Moon::Vertex){ { x0, y0 }, { tx0, ty0 }, color };
  vertices[1] = (Moon::Vertex){ { x1, y0 }, { tx1, ty0 }, color };
  vertices[2] = (Moon::Vertex){ { x1, y1 }, { tx1, ty1 }, color };
  vertices[3] = (Moon::Vertex){ { x0, y1 }, { tx0, ty1 }, color };
}

static void
vbo_quad_m(mrb_state *mrb, mrb_value self, Moon::Vertex vertices[4])
{
  Moon::IntRect quad_rect;
  Moon::FloatRect quad_texture_rect;
  Moon::Vector4 color;

  mrb_value rect_obj;
  mrb_value tex_rect_obj;
  mrb_value color_obj;

  mrb_get_args(mrb, "ooo", &rect_obj, &tex_rect_obj, &color_obj);
  quad_rect = mmrb_to_rect(mrb, rect_obj);
  quad_texture_rect = mmrb_to_float_rect(mrb, tex_rect_obj);
  color = mmrb_to_vector4(mrb, color_obj);

  make_quad(vertices, quad_rect, quad_texture_rect, color);
}

static mrb_value
vbo_add_quad(mrb_state *mrb, mrb_value self)
{
  Moon::Vertex vertices[4];
  GLuint indices[] = { 0, 1, 3, 2, 3, 1 };
  Moon::VertexBuffer *vbo = NULL;
  vbo_quad_m(mrb, self, vertices);
  vbo = mmrb_vertex_buffer_ptr(mrb, self);
  vbo->PushBack(vertices, 4, indices, 6);
  return self;
}

static mrb_value
vbo_add_quad_vertices(mrb_state *mrb, mrb_value self)
{
  Moon::Vertex vertices[4];
  Moon::VertexBuffer *vbo = NULL;
  vbo_quad_m(mrb, self, vertices);
  vbo = mmrb_vertex_buffer_ptr(mrb, self);
  vbo->PushBackVertices(vertices, 4);
  return self;
}

MOON_C_API void
mmrb_vbo_init(mrb_state *mrb)
{
  struct RClass *mod = mrb_define_module(mrb, "Moon");
  struct RClass *vbo_class = mrb_define_class_under(mrb, mod, "VertexBuffer", mrb->object_class);
  MRB_SET_INSTANCE_TT(vbo_class, MRB_TT_DATA);

  mrb_define_method(mrb, vbo_class, "initialize",        vbo_initialize,        MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vbo_class, "clear",             vbo_clear,             MRB_ARGS_NONE());
  mrb_define_method(mrb, vbo_class, "render",            vbo_render,            MRB_ARGS_REQ(1) | MRB_ARGS_OPT(1));
  mrb_define_method(mrb, vbo_class, "push_back",         vbo_push_back,         MRB_ARGS_REQ(3));
  mrb_define_method(mrb, vbo_class, "push_indices",      vbo_push_indices,      MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vbo_class, "vertex_count",      vbo_vertex_count,      MRB_ARGS_NONE());
  mrb_define_method(mrb, vbo_class, "index_count",       vbo_index_count,       MRB_ARGS_NONE());
  mrb_define_method(mrb, vbo_class, "add_quad",          vbo_add_quad,          MRB_ARGS_REQ(3));
  mrb_define_method(mrb, vbo_class, "add_quad_vertices", vbo_add_quad_vertices, MRB_ARGS_REQ(3));
}
