/*
 * Moon Vector2, a wrapper around Moon::Vector2
 */
#include <mruby.h>
#include <mruby/array.h>
#include <mruby/class.h>
#include <mruby/data.h>
#include <mruby/numeric.h>
#include "moon/mrb/vector1.hxx"
#include "moon/mrb/vector2.hxx"
#include "moon/mrb/vector_unroll.hxx"
#include "vec_helper.h"

#define m_vector_operator(__op__) \
  return mmrb_vector2_value(mrb, get_vector2_value(mrb, self) __op__ vector2_from_mrb_args(mrb));

#define m_vector_int_operator(__op__) \
  return mmrb_vector2_value(mrb, Moon::Vector2(glm::ivec2(get_vector2_value(mrb, self)) __op__ glm::ivec2(vector2_from_mrb_args(mrb))));

static void
vector2_free(mrb_state *mrb, void *p)
{
  Moon::Vector2 *vec2 = (Moon::Vector2*)p;
  if (vec2) {
    delete(vec2);
  }
}

MOON_C_API const struct mrb_data_type vector2_data_type = { "Moon::Vector2", vector2_free };

DEF_VEC_HELPERS(vector2, Moon::Vector2, mmrb_get_vector2_class(mrb), &vector2_data_type);

static Moon::Vector2
mmrb_vector2_extract_args(mrb_state *mrb, int argc, mrb_value *vals)
{
  mrb_float result[2];
  mmrb_vector_unroll(mrb, argc, vals, 2, result);
  return Moon::Vector2(result[0], result[1]);
}

static Moon::Vector2
vector2_from_mrb_args(mrb_state *mrb)
{
  mrb_value *vals;
  int len;
  mrb_get_args(mrb, "*", &vals, &len);
  return mmrb_vector2_extract_args(mrb, len, vals);
}

MOON_C_API Moon::Vector2
mmrb_to_vector2(mrb_state *mrb, mrb_value obj)
{
  return mmrb_vector2_extract_args(mrb, 1, &obj);
}

MOON_C_API mrb_value
mmrb_vector2_value(mrb_state *mrb, Moon::Vector2 vec)
{
  return set_vector2(mrb, new_vector2(mrb), vec);
}

/**
 * Initializes the vector
 *
 * @param [Numeric] x
 * @param [Numeric] y
 * @return [self]
 */
static mrb_value
vector2_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_float x = 0.0;
  mrb_float y = 0.0;
  mrb_get_args(mrb, "|ff", &x, &y);
  cleanup_vector2(mrb, self);
  mrb_data_init(self, new Moon::Vector2(x, y), &vector2_data_type);
  return self;
}

static mrb_value
vector2_initialize_copy(mrb_state *mrb, mrb_value self)
{
  cleanup_vector2(mrb, self);
  mrb_data_init(self, new Moon::Vector2(vector2_from_mrb_args(mrb)), &vector2_data_type);
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
vector2_coerce(mrb_state *mrb, mrb_value self)
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
vector2_eq(mrb_state *mrb, mrb_value self)
{
  mrb_value other;
  mrb_get_args(mrb, "o", &other);
  if (mrb_obj_is_kind_of(mrb, other, mmrb_get_vector2_class(mrb))) {
    return mrb_bool_value((*mmrb_vector2_ptr(mrb, self)) == (*mmrb_vector2_ptr(mrb, other)));
  }
  return mrb_bool_value(false);
}

static mrb_value
vector2_get_x(mrb_state *mrb, mrb_value self)
{
  return mrb_float_value(mrb, mmrb_vector2_ptr(mrb, self)->x);
}

static mrb_value
vector2_get_y(mrb_state *mrb, mrb_value self)
{
  return mrb_float_value(mrb, mmrb_vector2_ptr(mrb, self)->y);
}

static mrb_value
vector2_set_x(mrb_state *mrb, mrb_value self)
{
  mrb_float x;
  mrb_get_args(mrb, "f", &x);
  mmrb_vector2_ptr(mrb, self)->x = x;
  return mrb_nil_value();
}

static mrb_value
vector2_set_y(mrb_state *mrb, mrb_value self)
{
  mrb_float y;
  mrb_get_args(mrb, "f", &y);
  mmrb_vector2_ptr(mrb, self)->y = y;
  return mrb_nil_value();
}

/**
 * Returns the negated value of the vector
 *
 * @return [Vector2]
 */
static mrb_value
vector2_negate(mrb_state *mrb, mrb_value self)
{
  return mmrb_vector2_value(mrb, -get_vector2_value(mrb, self));
}

/**
 * Returns the identify of the vector
 *
 * @return [Vector2]
 */
static mrb_value
vector2_identity(mrb_state *mrb, mrb_value self)
{
  return mrb_obj_dup(mrb, self);
}

static mrb_value
vector2_add(mrb_state *mrb, mrb_value self)
{
  m_vector_operator(+);
}

static mrb_value
vector2_sub(mrb_state *mrb, mrb_value self)
{
  m_vector_operator(-);
}

static mrb_value
vector2_mul(mrb_state *mrb, mrb_value self)
{
  m_vector_operator(*);
}

static mrb_value
vector2_div(mrb_state *mrb, mrb_value self)
{
  m_vector_operator(/);
}

/**
 * Returns the bitwise not result of self
 *
 * @return [Vector2]
 */
static mrb_value
vector2_not(mrb_state *mrb, mrb_value self)
{
  return mmrb_vector2_value(mrb, Moon::Vector2(~(glm::ivec2(*mmrb_vector2_ptr(mrb, self)))));
}

static mrb_value
vector2_modulo(mrb_state *mrb, mrb_value self)
{
  m_vector_int_operator(%);
}

static mrb_value
vector2_shl(mrb_state *mrb, mrb_value self)
{
  m_vector_int_operator(<<);
}

static mrb_value
vector2_shr(mrb_state *mrb, mrb_value self)
{
  m_vector_int_operator(>>);
}

static mrb_value
vector2_and(mrb_state *mrb, mrb_value self)
{
  m_vector_int_operator(&);
}

static mrb_value
vector2_or(mrb_state *mrb, mrb_value self)
{
  m_vector_int_operator(|);
}

static mrb_value
vector2_xor(mrb_state *mrb, mrb_value self)
{
  m_vector_int_operator(^);
}

static mrb_value
vector2_dot(mrb_state *mrb, mrb_value self)
{
  return mrb_float_value(mrb, glm::dot(get_vector2_value(mrb, self), vector2_from_mrb_args(mrb)));
}

/* Returns a vector in the same direction, but with length of 1.
 * @return [Vector2]
 */
static mrb_value
vector2_normalize(mrb_state *mrb, mrb_value self)
{
  return mmrb_vector2_value(mrb, glm::normalize(get_vector2_value(mrb, self)));
}

/**
 * @return [Float] length of the vector
 */
static mrb_value
vector2_length(mrb_state *mrb, mrb_value self)
{
  return mrb_float_value(mrb, glm::length(get_vector2_value(mrb, self)));
}

/* Returns the distance betwwen self and other, i.e., length(p0 - p1).
 * @param [Vector2] other
 * @return [Float]
 */
static mrb_value
vector2_distance(mrb_state *mrb, mrb_value self)
{
  Moon::Vector2 *other;
  mrb_get_args(mrb, "d", &other, &vector2_data_type);
  return mrb_float_value(mrb, glm::distance(get_vector2_value(mrb, self), *other));
}

/*
 * @param [Float] angle
 * @return [Vector2] rotated vector
 */
static mrb_value
vector2_rotate(mrb_state *mrb, mrb_value self)
{
  mrb_float angle;
  mrb_get_args(mrb, "f", &angle);
  return mmrb_vector2_value(mrb, glm::rotate(get_vector2_value(mrb, self), (float)angle));
}

/* Linear interpolation of two quaternions.
 *
 * The interpolation is oriented.
 *
 * @param [Vector2] other quaternion
 * @param [Float] delta Interpolation factor. The interpolation is defined in the range [0, 1].
 * @return [Vector2]
 */
static mrb_value
vector2_lerp(mrb_state *mrb, mrb_value self)
{
  Moon::Vector2 *other;
  mrb_float delta;
  mrb_get_args(mrb, "df", &other, &vector2_data_type, &delta);
  return mmrb_vector2_value(mrb, glm::lerp(get_vector2_value(mrb, self), *other, (float)delta));
}

static mrb_value
vector2_set(mrb_state *mrb, mrb_value self)
{
  set_vector2(mrb, self, vector2_from_mrb_args(mrb));
  return self;
}

static mrb_value
vector2_to_a(mrb_state *mrb, mrb_value self)
{
  Moon::Vector2 *mvec2 = mmrb_vector2_ptr(mrb, self);
  mrb_value argv[2] = { mrb_float_value(mrb, mvec2->x),
                        mrb_float_value(mrb, mvec2->y) };
  return mrb_ary_new_from_values(mrb, 2, argv);
}

static mrb_value
vector2_s_extract(mrb_state *mrb, mrb_value self)
{
  Moon::Vector2 src_vect = vector2_from_mrb_args(mrb);
  mrb_value argv[2] = { mrb_float_value(mrb, src_vect.x),
                        mrb_float_value(mrb, src_vect.y) };
  return mrb_ary_new_from_values(mrb, 2, argv);
}

static mrb_value
vector2_s_cast(mrb_state *mrb, mrb_value klass)
{
  return mmrb_vector2_value(mrb, vector2_from_mrb_args(mrb));
}

static mrb_value
vector2_s_circular_rand(mrb_state *mrb, mrb_value klass)
{
  mrb_float radius;
  mrb_get_args(mrb, "f", &radius);
  return mmrb_vector2_value(mrb, glm::circularRand((float)radius));
}

static mrb_value
vector2_s_disk_rand(mrb_state *mrb, mrb_value klass)
{
  mrb_float radius;
  mrb_get_args(mrb, "f", &radius);
  return mmrb_vector2_value(mrb, glm::diskRand((float)radius));
}

MOON_C_API void
mmrb_vector2_init(mrb_state *mrb)
{
  struct RClass *mod = mrb_define_module(mrb, "Moon");
  struct RClass *vector2_class = mrb_define_class_under(mrb, mod, "Vector2", mrb->object_class);
  MRB_SET_INSTANCE_TT(vector2_class, MRB_TT_DATA);

  mrb_define_method(mrb, vector2_class, "initialize",      vector2_initialize,      MRB_ARGS_OPT(2));
  mrb_define_method(mrb, vector2_class, "initialize_copy", vector2_initialize_copy, MRB_ARGS_REQ(1));
  /* coercion */
  mrb_define_method(mrb, vector2_class, "coerce",          vector2_coerce,          MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vector2_class, "==",              vector2_eq,              MRB_ARGS_REQ(1));
  /* attribute setters */
  mrb_define_method(mrb, vector2_class, "x",               vector2_get_x,        MRB_ARGS_NONE());
  mrb_define_method(mrb, vector2_class, "y",               vector2_get_y,        MRB_ARGS_NONE());
  mrb_define_method(mrb, vector2_class, "x=",              vector2_set_x,        MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vector2_class, "y=",              vector2_set_y,        MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vector2_class, "set",             vector2_set,             MRB_ARGS_ANY());
  /* arithmetic */
  mrb_define_method(mrb, vector2_class, "-@",              vector2_negate,          MRB_ARGS_NONE());
  mrb_define_method(mrb, vector2_class, "+@",              vector2_identity,        MRB_ARGS_NONE());
  mrb_define_method(mrb, vector2_class, "+",               vector2_add,             MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vector2_class, "-",               vector2_sub,             MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vector2_class, "*",               vector2_mul,             MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vector2_class, "/",               vector2_div,             MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vector2_class, "dot",             vector2_dot,             MRB_ARGS_REQ(1));
  /*mrb_define_method(mrb, vector2_class, "cross",           vector2_cross,           MRB_ARGS_REQ(1));*/
  /* */
  mrb_define_method(mrb, vector2_class, "normalize",       vector2_normalize,       MRB_ARGS_NONE());
  mrb_define_method(mrb, vector2_class, "length",          vector2_length,          MRB_ARGS_NONE());
  mrb_define_method(mrb, vector2_class, "distance",        vector2_distance,        MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vector2_class, "rotate",          vector2_rotate,          MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vector2_class, "lerp",            vector2_lerp,            MRB_ARGS_REQ(2));
  /* bitwise operators */
  mrb_define_method(mrb, vector2_class, "~@",              vector2_not,             MRB_ARGS_NONE());
  mrb_define_method(mrb, vector2_class, "%",               vector2_modulo,          MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vector2_class, "<<",              vector2_shl,             MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vector2_class, ">>",              vector2_shr,             MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vector2_class, "&",               vector2_and,             MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vector2_class, "|",               vector2_or,              MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vector2_class, "^",               vector2_xor,             MRB_ARGS_REQ(1));
  /* conversion */
  mrb_define_method(mrb, vector2_class, "to_a",            vector2_to_a,            MRB_ARGS_NONE());
  /* cast */
  mrb_define_class_method(mrb, vector2_class, "[]",        vector2_s_cast,          MRB_ARGS_ANY());
  mrb_define_class_method(mrb, vector2_class, "extract",   vector2_s_extract,       MRB_ARGS_REQ(1));
  /* gen */
  mrb_define_class_method(mrb, vector2_class, "circular_rand",   vector2_s_circular_rand, MRB_ARGS_REQ(1));
  mrb_define_class_method(mrb, vector2_class, "disk_rand",       vector2_s_disk_rand,     MRB_ARGS_REQ(1));
}
