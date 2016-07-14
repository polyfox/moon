/*
 * Moon Vector3, a wrapper around Moon::Vector3
 */
#include <mruby.h>
#include <mruby/array.h>
#include <mruby/class.h>
#include <mruby/data.h>
#include <mruby/numeric.h>
#include "moon/mrb/vector1.hxx"
#include "moon/mrb/vector2.hxx"
#include "moon/mrb/vector3.hxx"
#include "moon/mrb/vector_unroll.hxx"
#include "vec_helper.h"

#define m_vector_operator(__op__) \
  return mmrb_vector3_value(mrb, get_vector3_value(mrb, self) __op__ vector3_from_mrb_args(mrb));

#define m_vector_int_operator(__op__) \
  return mmrb_vector3_value(mrb, Moon::Vector3(glm::ivec3(get_vector3_value(mrb, self)) __op__ glm::ivec3(vector3_from_mrb_args(mrb))));

static void
vector3_free(mrb_state *mrb, void *p)
{
  Moon::Vector3 *vec3 = (Moon::Vector3*)p;
  if (vec3) {
    delete(vec3);
  }
}

MOON_C_API const struct mrb_data_type vector3_data_type = { "Moon::Vector3", vector3_free };

DEF_VEC_HELPERS(vector3, Moon::Vector3, mmrb_get_vector3_class(mrb), &vector3_data_type);

static Moon::Vector3
mmrb_vector3_extract_args(mrb_state *mrb, int argc, mrb_value *vals)
{
  mrb_float result[3];
  mmrb_vector_unroll(mrb, argc, vals, 3, result);
  return Moon::Vector3(result[0], result[1], result[2]);
}

static Moon::Vector3
vector3_from_mrb_args(mrb_state *mrb)
{
  mrb_value *vals;
  int len;
  mrb_get_args(mrb, "*", &vals, &len);
  return mmrb_vector3_extract_args(mrb, len, vals);
}

MOON_C_API Moon::Vector3
mmrb_to_vector3(mrb_state *mrb, mrb_value obj)
{
  return mmrb_vector3_extract_args(mrb, 1, &obj);
}

MOON_C_API mrb_value
mmrb_vector3_value(mrb_state *mrb, Moon::Vector3 vec)
{
  return set_vector3(mrb, new_vector3(mrb), vec);
}

/**
 * Initializes the vector
 *
 * @param [Numeric] x
 * @param [Numeric] y
 * @param [Numeric] z
 * @return [self]
 */
static mrb_value
vector3_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_float x = 0.0;
  mrb_float y = 0.0;
  mrb_float z = 0.0;
  mrb_get_args(mrb, "|fff", &x, &y, &z);
  cleanup_vector3(mrb, self);
  mrb_data_init(self, new Moon::Vector3(x, y, z), &vector3_data_type);
  return self;
};

static mrb_value
vector3_initialize_copy(mrb_state *mrb, mrb_value self)
{
  cleanup_vector3(mrb, self);
  mrb_data_init(self, new Moon::Vector3(vector3_from_mrb_args(mrb)), &vector3_data_type);
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
vector3_coerce(mrb_state *mrb, mrb_value self)
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
vector3_eq(mrb_state *mrb, mrb_value self)
{
  mrb_value other;
  mrb_get_args(mrb, "o", &other);
  if (mrb_obj_is_kind_of(mrb, other, mmrb_get_vector3_class(mrb))) {
    return mrb_bool_value((*mmrb_vector3_ptr(mrb, self)) == (*mmrb_vector3_ptr(mrb, other)));
  }
  return mrb_bool_value(false);
}

static mrb_value
vector3_get_x(mrb_state *mrb, mrb_value self)
{
  return mrb_float_value(mrb, mmrb_vector3_ptr(mrb, self)->x);
}

static mrb_value
vector3_get_y(mrb_state *mrb, mrb_value self)
{
  return mrb_float_value(mrb, mmrb_vector3_ptr(mrb, self)->y);
}

static mrb_value
vector3_get_z(mrb_state *mrb, mrb_value self)
{
  return mrb_float_value(mrb, mmrb_vector3_ptr(mrb, self)->z);
}

static mrb_value
vector3_set_x(mrb_state *mrb, mrb_value self)
{
  mrb_float x;
  mrb_get_args(mrb, "f", &x);
  mmrb_vector3_ptr(mrb, self)->x = x;
  return mrb_nil_value();
}

static mrb_value
vector3_set_y(mrb_state *mrb, mrb_value self)
{
  mrb_float y;
  mrb_get_args(mrb, "f", &y);
  mmrb_vector3_ptr(mrb, self)->y = y;
  return mrb_nil_value();
}

static mrb_value
vector3_set_z(mrb_state *mrb, mrb_value self)
{
  mrb_float z;
  mrb_get_args(mrb, "f", &z);
  mmrb_vector3_ptr(mrb, self)->z = z;
  return mrb_nil_value();
}

/**
 * Returns the negated value of the vector
 *
 * @return [Vector3]
 */
static mrb_value
vector3_negate(mrb_state *mrb, mrb_value self)
{
  return mmrb_vector3_value(mrb, -get_vector3_value(mrb, self));
}

/**
 * Returns the identify of the vector
 *
 * @return [Vector3]
 */
static mrb_value
vector3_identity(mrb_state *mrb, mrb_value self)
{
  return mrb_obj_dup(mrb, self);
}

/* Returns a vector in the same direction, but with length of 1.
 * @return [Vector3]
 */
static mrb_value
vector3_normalize(mrb_state *mrb, mrb_value self)
{
  return mmrb_vector3_value(mrb, glm::normalize(get_vector3_value(mrb, self)));
}

/* Returns the length of x, i.e., sqrt(x * x).
 * @return [Float]
*/
static mrb_value
vector3_length(mrb_state *mrb, mrb_value self)
{
  return mrb_float_value(mrb, glm::length(get_vector3_value(mrb, self)));
}

// @return [Vector3]
static mrb_value
vector3_add(mrb_state *mrb, mrb_value self)
{
  m_vector_operator(+);
}

static mrb_value
vector3_sub(mrb_state *mrb, mrb_value self)
{
  m_vector_operator(-);
}

static mrb_value
vector3_mul(mrb_state *mrb, mrb_value self)
{
  m_vector_operator(*);
}

static mrb_value
vector3_div(mrb_state *mrb, mrb_value self)
{
  m_vector_operator(/);
}

/**
 * Returns the bitwise not result of self
 *
 * @return [Vector3]
 */
static mrb_value
vector3_not(mrb_state *mrb, mrb_value self)
{
  return mmrb_vector3_value(mrb, Moon::Vector3(~(glm::ivec3(*mmrb_vector3_ptr(mrb, self)))));
}

static mrb_value
vector3_modulo(mrb_state *mrb, mrb_value self)
{
  m_vector_int_operator(%);
}

static mrb_value
vector3_shl(mrb_state *mrb, mrb_value self)
{
  m_vector_int_operator(<<);
}

static mrb_value
vector3_shr(mrb_state *mrb, mrb_value self)
{
  m_vector_int_operator(>>);
}

static mrb_value
vector3_and(mrb_state *mrb, mrb_value self)
{
  m_vector_int_operator(&);
}

static mrb_value
vector3_or(mrb_state *mrb, mrb_value self)
{
  m_vector_int_operator(|);
}

static mrb_value
vector3_xor(mrb_state *mrb, mrb_value self)
{
  m_vector_int_operator(^);
}

/* Returns the dot product of self and other, i.e., result = self * other.
 * @param [Vector3] other
 * @return [Float]
 */
static mrb_value
vector3_dot(mrb_state *mrb, mrb_value self)
{
  Moon::Vector3 *other;
  mrb_get_args(mrb, "d", &other, &vector3_data_type);
  return mrb_float_value(mrb, glm::dot(get_vector3_value(mrb, self), *other));
}

/* Returns the cross product of self and other.
 * @param [Vector3] other
 * @return [Vector3]
 */
static mrb_value
vector3_cross(mrb_state *mrb, mrb_value self)
{
  Moon::Vector3 *other;
  mrb_value dest_vec;
  mrb_get_args(mrb, "d", &other, &vector3_data_type);
  dest_vec = mrb_obj_dup(mrb, self);
  set_vector3(mrb, dest_vec, glm::cross(get_vector3_value(mrb, self), *other));
  return dest_vec;
}

/* Returns the distance betwwen self and other, i.e., length(p0 - p1).
 * @param [Vector3] other
 * @return [Float]
 */
static mrb_value
vector3_distance(mrb_state *mrb, mrb_value self)
{
  Moon::Vector3 *other;
  mrb_get_args(mrb, "d", &other, &vector3_data_type);
  return mrb_float_value(mrb, glm::distance(get_vector3_value(mrb, self), *other));
}
/*
 * @param [Vector3] other
 * @param [Float] angle
 * @return [Vector3]
 */
static mrb_value
vector3_rotate(mrb_state *mrb, mrb_value self)
{
  Moon::Vector3 *other;
  mrb_float angle;
  mrb_get_args(mrb, "df", &other, &vector3_data_type, &angle);
  return mmrb_vector3_value(mrb, glm::rotate(*mmrb_vector3_ptr(mrb, self), (float)angle, *other));
}

/* Rotate the vector around the x axis.
 * @param [Float] angle
 * @return [Vector3]
 */
static mrb_value
vector3_rotate_x(mrb_state *mrb, mrb_value self)
{
  mrb_float angle;
  mrb_get_args(mrb, "f", &angle);
  return mmrb_vector3_value(mrb, glm::rotateX(*mmrb_vector3_ptr(mrb, self), (float)angle));
}

/* Rotate the vector around the y axis.
 * @param [Float] angle
 * @return [Vector3]
 */
static mrb_value
vector3_rotate_y(mrb_state *mrb, mrb_value self)
{
  mrb_float angle;
  mrb_get_args(mrb, "f", &angle);
  return mmrb_vector3_value(mrb, glm::rotateY(*mmrb_vector3_ptr(mrb, self), (float)angle));
}

/* Rotate the vector around the z axis.
 * @param [Float] angle
 * @return [Vector3]
 */
static mrb_value
vector3_rotate_z(mrb_state *mrb, mrb_value self)
{
  mrb_float angle;
  mrb_get_args(mrb, "f", &angle);
  return mmrb_vector3_value(mrb, glm::rotateZ(*mmrb_vector3_ptr(mrb, self), (float)angle));
}

/* Linear interpolation of two quaternions.
 *
 * The interpolation is oriented.
 *
 * @param [Vector3] other quaternion
 * @param [Float] delta Interpolation factor. The interpolation is defined in the range [0, 1].
 * @return [Vector3]
 */
static mrb_value
vector3_lerp(mrb_state *mrb, mrb_value self)
{
  Moon::Vector3 *other;
  mrb_float delta;
  mrb_get_args(mrb, "df", &other, &vector3_data_type, &delta);
  return mmrb_vector3_value(mrb, glm::lerp(*mmrb_vector3_ptr(mrb, self), *other, (float)delta));
}

/* Spherical linear interpolation of two quaternions.
 *
 * Returns the slurp interpolation between two quaternions.
 *
 * The interpolation always take the short path and the rotation is performed at constant speed.
 *
 * @param [Vector3] other quaternion
 * @param [Float] delta Interpolation factor. The interpolation is defined beyond the range [0, 1].
 * @return [Vector3]
 */
static mrb_value
vector3_slerp(mrb_state *mrb, mrb_value self)
{
  Moon::Vector3 *other;
  mrb_float delta;
  mrb_get_args(mrb, "df", &other, &vector3_data_type, &delta);
  return mmrb_vector3_value(mrb, glm::slerp(*mmrb_vector3_ptr(mrb, self), *other, (float)delta));
}

static mrb_value
vector3_set(mrb_state *mrb, mrb_value self)
{
  Moon::Vector3 *v3;
  v3 = mmrb_vector3_ptr(mrb, self);
  *v3 = vector3_from_mrb_args(mrb);
  return self;
}

// @return [Array<Float>]
static mrb_value
vector3_to_a(mrb_state *mrb, mrb_value self)
{
  Moon::Vector3 *mvec3 = mmrb_vector3_ptr(mrb, self);
  mrb_value argv[3] = { mrb_float_value(mrb, mvec3->x),
                        mrb_float_value(mrb, mvec3->y),
                        mrb_float_value(mrb, mvec3->z) };
  return mrb_ary_new_from_values(mrb, 3, argv);
}

// @return [Array<Float>]
static mrb_value
vector3_s_extract(mrb_state *mrb, mrb_value klass)
{
  Moon::Vector3 src_vect = vector3_from_mrb_args(mrb);
  mrb_value argv[3] = { mrb_float_value(mrb, src_vect.x),
                        mrb_float_value(mrb, src_vect.y),
                        mrb_float_value(mrb, src_vect.z) };
  return mrb_ary_new_from_values(mrb, 3, argv);
}

static mrb_value
vector3_s_cast(mrb_state *mrb, mrb_value klass)
{
  return mmrb_vector3_value(mrb, vector3_from_mrb_args(mrb));
}

MOON_C_API void
mmrb_vector3_init(mrb_state *mrb)
{
  struct RClass *mod = mrb_define_module(mrb, "Moon");
  struct RClass *vector3_class = mrb_define_class_under(mrb, mod, "Vector3", mrb->object_class);
  MRB_SET_INSTANCE_TT(vector3_class, MRB_TT_DATA);

  mrb_define_method(mrb, vector3_class, "initialize",      vector3_initialize,      MRB_ARGS_OPT(3));
  mrb_define_method(mrb, vector3_class, "initialize_copy", vector3_initialize_copy, MRB_ARGS_REQ(1));
  /* coercion */
  mrb_define_method(mrb, vector3_class, "coerce",          vector3_coerce,          MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vector3_class, "==",              vector3_eq,              MRB_ARGS_REQ(1));
  /* attribute setters */
  mrb_define_method(mrb, vector3_class, "x",               vector3_get_x,        MRB_ARGS_NONE());
  mrb_define_method(mrb, vector3_class, "y",               vector3_get_y,        MRB_ARGS_NONE());
  mrb_define_method(mrb, vector3_class, "z",               vector3_get_z,        MRB_ARGS_NONE());
  mrb_define_method(mrb, vector3_class, "x=",              vector3_set_x,        MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vector3_class, "y=",              vector3_set_y,        MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vector3_class, "z=",              vector3_set_z,        MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vector3_class, "set",             vector3_set,             MRB_ARGS_ANY());
  /* arithmetic */
  mrb_define_method(mrb, vector3_class, "-@",              vector3_negate,          MRB_ARGS_NONE());
  mrb_define_method(mrb, vector3_class, "+@",              vector3_identity,        MRB_ARGS_NONE());
  mrb_define_method(mrb, vector3_class, "+",               vector3_add,             MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vector3_class, "-",               vector3_sub,             MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vector3_class, "*",               vector3_mul,             MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vector3_class, "/",               vector3_div,             MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vector3_class, "dot",             vector3_dot,             MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vector3_class, "cross",           vector3_cross,           MRB_ARGS_REQ(1));
  /* */
  mrb_define_method(mrb, vector3_class, "normalize",       vector3_normalize,       MRB_ARGS_NONE());
  mrb_define_method(mrb, vector3_class, "length",          vector3_length,          MRB_ARGS_NONE());
  mrb_define_method(mrb, vector3_class, "distance",        vector3_distance,        MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vector3_class, "rotate",          vector3_rotate,          MRB_ARGS_REQ(2));
  mrb_define_method(mrb, vector3_class, "rotate_x",        vector3_rotate_x,        MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vector3_class, "rotate_y",        vector3_rotate_y,        MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vector3_class, "rotate_z",        vector3_rotate_z,        MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vector3_class, "lerp",            vector3_lerp,            MRB_ARGS_REQ(2));
  mrb_define_method(mrb, vector3_class, "slerp",           vector3_slerp,           MRB_ARGS_REQ(2));
  /* bitwise operators */
  mrb_define_method(mrb, vector3_class, "~@",              vector3_not,             MRB_ARGS_NONE());
  mrb_define_method(mrb, vector3_class, "%",               vector3_modulo,          MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vector3_class, "<<",              vector3_shl,             MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vector3_class, ">>",              vector3_shr,             MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vector3_class, "&",               vector3_and,             MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vector3_class, "|",               vector3_or,              MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vector3_class, "^",               vector3_xor,             MRB_ARGS_REQ(1));
  /* conversion */
  mrb_define_method(mrb, vector3_class, "to_a",            vector3_to_a,            MRB_ARGS_NONE());
  /* cast */
  mrb_define_class_method(mrb, vector3_class, "[]",        vector3_s_cast,          MRB_ARGS_ANY());
  mrb_define_class_method(mrb, vector3_class, "extract",   vector3_s_extract,       MRB_ARGS_REQ(1));

  mrb_alias_method(mrb, vector3_class, mrb_intern_cstr(mrb, "r"), mrb_intern_cstr(mrb, "x"));
  mrb_alias_method(mrb, vector3_class, mrb_intern_cstr(mrb, "g"), mrb_intern_cstr(mrb, "y"));
  mrb_alias_method(mrb, vector3_class, mrb_intern_cstr(mrb, "b"), mrb_intern_cstr(mrb, "z"));

  mrb_alias_method(mrb, vector3_class, mrb_intern_cstr(mrb, "r="), mrb_intern_cstr(mrb, "x="));
  mrb_alias_method(mrb, vector3_class, mrb_intern_cstr(mrb, "g="), mrb_intern_cstr(mrb, "y="));
  mrb_alias_method(mrb, vector3_class, mrb_intern_cstr(mrb, "b="), mrb_intern_cstr(mrb, "z="));
}
