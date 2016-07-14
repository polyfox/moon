/*
 * Moon Vector4, a wrapper around Moon::Vector4
 */
#include <mruby.h>
#include <mruby/array.h>
#include <mruby/class.h>
#include <mruby/data.h>
#include <mruby/numeric.h>
#include "moon/mrb/vector1.hxx"
#include "moon/mrb/vector2.hxx"
#include "moon/mrb/vector3.hxx"
#include "moon/mrb/vector4.hxx"
#include "moon/mrb/vector_unroll.hxx"
#include "vec_helper.h"

#define m_vector_operator(__op__) \
  return mmrb_vector4_value(mrb, get_vector4_value(mrb, self) __op__ vector4_from_mrb_args(mrb));

#define m_vector_int_operator(__op__) \
  return mmrb_vector4_value(mrb, Moon::Vector4(glm::ivec4(get_vector4_value(mrb, self)) __op__ glm::ivec4(vector4_from_mrb_args(mrb))));

static void
vector4_free(mrb_state *mrb, void *p)
{
  Moon::Vector4 *vec4 = (Moon::Vector4*)p;
  if (vec4) {
    delete(vec4);
  }
}

MOON_C_API const struct mrb_data_type vector4_data_type = { "Moon::Vector4", vector4_free };

DEF_VEC_HELPERS(vector4, Moon::Vector4, mmrb_get_vector4_class(mrb), &vector4_data_type);

static Moon::Vector4
mmrb_vector4_extract_args(mrb_state *mrb, int argc, mrb_value *vals)
{
  mrb_float result[4];
  mmrb_vector_unroll(mrb, argc, vals, 4, result);
  return Moon::Vector4(result[0], result[1], result[2], result[3]);
}

static Moon::Vector4
vector4_from_mrb_args(mrb_state *mrb)
{
  mrb_value *vals;
  int len;
  mrb_get_args(mrb, "*", &vals, &len);
  return mmrb_vector4_extract_args(mrb, len, vals);
}

MOON_C_API Moon::Vector4
mmrb_to_vector4(mrb_state *mrb, mrb_value obj)
{
  return mmrb_vector4_extract_args(mrb, 1, &obj);
}

MOON_C_API mrb_value
mmrb_vector4_value(mrb_state *mrb, Moon::Vector4 vec)
{
  return set_vector4(mrb, new_vector4(mrb), vec);
}

/**
 * Initializes the vector4
 * @overload initialize
 * @overload initialize(x, y, z, w)
 *   @param [Numeric] x
 *   @param [Numeric] y
 *   @param [Numeric] z
 *   @param [Numeric] w
 */
static mrb_value
vector4_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_float x = 0.0;
  mrb_float y = 0.0;
  mrb_float z = 0.0;
  mrb_float w = 0.0;
  mrb_get_args(mrb, "|ffff", &x, &y, &z, &w);
  cleanup_vector4(mrb, self);
  mrb_data_init(self, new Moon::Vector4(x, y, z, w), &vector4_data_type);
  return self;
}

static mrb_value
vector4_initialize_copy(mrb_state *mrb, mrb_value self)
{
  cleanup_vector4(mrb, self);
  mrb_data_init(self, new Moon::Vector4(vector4_from_mrb_args(mrb)), &vector4_data_type);
  return self;
}

/**
 * Rotates the order of the parameters for builtin methods
 *
 * @param [Object] other
 * @return [Array<Object>[2]]
 * @api private
 */
static mrb_value
vector4_coerce(mrb_state *mrb, mrb_value self)
{
  mrb_value other;
  mrb_get_args(mrb, "o", &other);
  mrb_value argv[2] = { self, other };
  return mrb_ary_new_from_values(mrb, 2, argv);
}

/**
 * Compares self and other to determine if they are equal
 *
 * @param [Object] other
 * @return [Boolean] true other and self are equal, false otherwise
 */
static mrb_value
vector4_eq(mrb_state *mrb, mrb_value self)
{
  mrb_value other;
  mrb_get_args(mrb, "o", &other);
  if (mrb_obj_is_kind_of(mrb, other, mmrb_get_vector4_class(mrb))) {
    return mrb_bool_value((*mmrb_vector4_ptr(mrb, self)) == (*mmrb_vector4_ptr(mrb, other)));
  }
  return mrb_bool_value(false);
}

static mrb_value
vector4_get_x(mrb_state *mrb, mrb_value self)
{
  return mrb_float_value(mrb, mmrb_vector4_ptr(mrb, self)->x);
}

static mrb_value
vector4_get_y(mrb_state *mrb, mrb_value self)
{
  return mrb_float_value(mrb, mmrb_vector4_ptr(mrb, self)->y);
}

static mrb_value
vector4_get_z(mrb_state *mrb, mrb_value self)
{
  return mrb_float_value(mrb, mmrb_vector4_ptr(mrb, self)->z);
}

static mrb_value
vector4_get_w(mrb_state *mrb, mrb_value self)
{
  return mrb_float_value(mrb, mmrb_vector4_ptr(mrb, self)->w);
}

static mrb_value
vector4_set_x(mrb_state *mrb, mrb_value self)
{
  mrb_float x;
  mrb_get_args(mrb, "f", &x);
  mmrb_vector4_ptr(mrb, self)->x = x;
  return self;
}

static mrb_value
vector4_set_y(mrb_state *mrb, mrb_value self)
{
  mrb_float y;
  mrb_get_args(mrb, "f", &y);
  mmrb_vector4_ptr(mrb, self)->y = y;
  return self;
}

static mrb_value
vector4_set_z(mrb_state *mrb, mrb_value self)
{
  mrb_float z;
  mrb_get_args(mrb, "f", &z);
  mmrb_vector4_ptr(mrb, self)->z = z;
  return self;
}

static mrb_value
vector4_set_w(mrb_state *mrb, mrb_value self)
{
  mrb_float w;
  mrb_get_args(mrb, "f", &w);
  mmrb_vector4_ptr(mrb, self)->w = w;
  return self;
}

/**
 * Returns the negated value of the vector
 *
 * @return [Vector4]
 */
static mrb_value
vector4_negate(mrb_state *mrb, mrb_value self)
{
  return mmrb_vector4_value(mrb, -get_vector4_value(mrb, self));
}

/**
 * Returns the identify of the vector
 *
 * @return [Vector4]
 */
static mrb_value
vector4_identity(mrb_state *mrb, mrb_value self)
{
  return mrb_obj_dup(mrb, self);
}

/* Returns a vector in the same direction, but with length of 1.
 * @return [Vector4]
 */
static mrb_value
vector4_normalize(mrb_state *mrb, mrb_value self)
{
  return mmrb_vector4_value(mrb, glm::normalize(get_vector4_value(mrb, self)));
}

/* Returns the length of x, i.e., sqrt(x * x).
 * @return [Float]
 */
static mrb_value
vector4_length(mrb_state *mrb, mrb_value self)
{
  return mrb_float_value(mrb, glm::length(get_vector4_value(mrb, self)));
}

static mrb_value
vector4_add(mrb_state *mrb, mrb_value self)
{
  m_vector_operator(+);
}

static mrb_value
vector4_sub(mrb_state *mrb, mrb_value self)
{
  m_vector_operator(-);
}

static mrb_value
vector4_mul(mrb_state *mrb, mrb_value self)
{
  m_vector_operator(*);
}

static mrb_value
vector4_div(mrb_state *mrb, mrb_value self)
{
  m_vector_operator(/);
}

/**
 * Returns the bitwise not result of self
 *
 * @return [Vector4]
 */
static mrb_value
vector4_not(mrb_state *mrb, mrb_value self)
{
  return mmrb_vector4_value(mrb, Moon::Vector4(~(glm::ivec4(*mmrb_vector4_ptr(mrb, self)))));
}

static mrb_value
vector4_modulo(mrb_state *mrb, mrb_value self)
{
  m_vector_int_operator(%);
}

static mrb_value
vector4_shl(mrb_state *mrb, mrb_value self)
{
  m_vector_int_operator(<<);
}

static mrb_value
vector4_shr(mrb_state *mrb, mrb_value self)
{
  m_vector_int_operator(>>);
}

static mrb_value
vector4_and(mrb_state *mrb, mrb_value self)
{
  m_vector_int_operator(&);
}

static mrb_value
vector4_or(mrb_state *mrb, mrb_value self)
{
  m_vector_int_operator(|);
}

static mrb_value
vector4_xor(mrb_state *mrb, mrb_value self)
{
  m_vector_int_operator(^);
}

static mrb_value
vector4_dot(mrb_state *mrb, mrb_value self)
{
  return mrb_float_value(mrb, glm::dot(get_vector4_value(mrb, self), vector4_from_mrb_args(mrb)));
}

/* Returns the distance betwwen self and other, i.e., length(p0 - p1).
 * @param [Vector4] other
 * @return [Float]
 */
static mrb_value
vector4_distance(mrb_state *mrb, mrb_value self)
{
  Moon::Vector4 *other;
  mrb_get_args(mrb, "d", &other, &vector4_data_type);
  return mrb_float_value(mrb, glm::distance(get_vector4_value(mrb, self), *other));
}

static mrb_value
vector4_set(mrb_state *mrb, mrb_value self)
{
  return set_vector4(mrb, self, vector4_from_mrb_args(mrb));
}

/**
 * Returns an array with all the vector components
 *
 * @return [Array<Float>]
 */
static mrb_value
vector4_to_a(mrb_state *mrb, mrb_value self)
{
  Moon::Vector4* mvec4 = mmrb_vector4_ptr(mrb, self);
  mrb_value argv[4] = { mrb_float_value(mrb, mvec4->x),
                        mrb_float_value(mrb, mvec4->y),
                        mrb_float_value(mrb, mvec4->z),
                        mrb_float_value(mrb, mvec4->w) };
  return mrb_ary_new_from_values(mrb, 4, argv);
}

static mrb_value
vector4_s_extract(mrb_state *mrb, mrb_value self)
{
  Moon::Vector4 v4a = vector4_from_mrb_args(mrb);
  mrb_value argv[4] = { mrb_float_value(mrb, v4a.x),
                        mrb_float_value(mrb, v4a.y),
                        mrb_float_value(mrb, v4a.z),
                        mrb_float_value(mrb, v4a.w) };
  return mrb_ary_new_from_values(mrb, 4, argv);
}

static mrb_value
vector4_s_cast(mrb_state *mrb, mrb_value klass)
{
  return set_vector4(mrb, new_vector4(mrb), vector4_from_mrb_args(mrb));
}

MOON_C_API void
mmrb_vector4_init(mrb_state *mrb)
{
  struct RClass *mod = mrb_define_module(mrb, "Moon");
  struct RClass *vector4_class = mrb_define_class_under(mrb, mod, "Vector4", mrb->object_class);
  MRB_SET_INSTANCE_TT(vector4_class, MRB_TT_DATA);

  mrb_define_method(mrb, vector4_class, "initialize",      vector4_initialize,      MRB_ARGS_OPT(4));
  mrb_define_method(mrb, vector4_class, "initialize_copy", vector4_initialize_copy, MRB_ARGS_REQ(1));
  /* coercion */
  mrb_define_method(mrb, vector4_class, "coerce",          vector4_coerce,          MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vector4_class, "==",              vector4_eq,              MRB_ARGS_REQ(1));
  /* attribute setters */
  mrb_define_method(mrb, vector4_class, "x",               vector4_get_x,           MRB_ARGS_NONE());
  mrb_define_method(mrb, vector4_class, "y",               vector4_get_y,           MRB_ARGS_NONE());
  mrb_define_method(mrb, vector4_class, "z",               vector4_get_z,           MRB_ARGS_NONE());
  mrb_define_method(mrb, vector4_class, "w",               vector4_get_w,           MRB_ARGS_NONE());
  mrb_define_method(mrb, vector4_class, "x=",              vector4_set_x,           MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vector4_class, "y=",              vector4_set_y,           MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vector4_class, "z=",              vector4_set_z,           MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vector4_class, "w=",              vector4_set_w,           MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vector4_class, "set",             vector4_set,             MRB_ARGS_ANY());
  /* arithmetic */
  mrb_define_method(mrb, vector4_class, "-@",              vector4_negate,          MRB_ARGS_NONE());
  mrb_define_method(mrb, vector4_class, "+@",              vector4_identity,        MRB_ARGS_NONE());
  mrb_define_method(mrb, vector4_class, "normalize",       vector4_normalize,       MRB_ARGS_NONE());
  mrb_define_method(mrb, vector4_class, "length",          vector4_length,          MRB_ARGS_NONE());
  mrb_define_method(mrb, vector4_class, "+",               vector4_add,             MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vector4_class, "-",               vector4_sub,             MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vector4_class, "*",               vector4_mul,             MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vector4_class, "/",               vector4_div,             MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vector4_class, "dot",             vector4_dot,             MRB_ARGS_REQ(1));
  //mrb_define_method(mrb, vector4_class, "cross",           vector4_cross,           MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vector4_class, "distance",        vector4_distance,        MRB_ARGS_REQ(1));
  /* bitwise operators */
  mrb_define_method(mrb, vector4_class, "~@",              vector4_not,             MRB_ARGS_NONE());
  mrb_define_method(mrb, vector4_class, "%",               vector4_modulo,          MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vector4_class, "<<",              vector4_shl,             MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vector4_class, ">>",              vector4_shr,             MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vector4_class, "&",               vector4_and,             MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vector4_class, "|",               vector4_or,              MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vector4_class, "^",               vector4_xor,             MRB_ARGS_REQ(1));
  /* conversion */
  mrb_define_method(mrb, vector4_class, "to_a",            vector4_to_a,            MRB_ARGS_NONE());
  /* cast */
  mrb_define_class_method(mrb, vector4_class, "[]",        vector4_s_cast,          MRB_ARGS_ANY());
  mrb_define_class_method(mrb, vector4_class, "extract",   vector4_s_extract,       MRB_ARGS_REQ(1));

  mrb_alias_method(mrb, vector4_class, mrb_intern_cstr(mrb, "r"), mrb_intern_cstr(mrb, "x"));
  mrb_alias_method(mrb, vector4_class, mrb_intern_cstr(mrb, "g"), mrb_intern_cstr(mrb, "y"));
  mrb_alias_method(mrb, vector4_class, mrb_intern_cstr(mrb, "b"), mrb_intern_cstr(mrb, "z"));
  mrb_alias_method(mrb, vector4_class, mrb_intern_cstr(mrb, "a"), mrb_intern_cstr(mrb, "w"));

  mrb_alias_method(mrb, vector4_class, mrb_intern_cstr(mrb, "r="), mrb_intern_cstr(mrb, "x="));
  mrb_alias_method(mrb, vector4_class, mrb_intern_cstr(mrb, "g="), mrb_intern_cstr(mrb, "y="));
  mrb_alias_method(mrb, vector4_class, mrb_intern_cstr(mrb, "b="), mrb_intern_cstr(mrb, "z="));
  mrb_alias_method(mrb, vector4_class, mrb_intern_cstr(mrb, "a="), mrb_intern_cstr(mrb, "w="));
}
