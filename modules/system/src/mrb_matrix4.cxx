/*
 * Moon Matrix4, a wrapper around glm::mat4
 */
#define GLM_FORCE_RADIANS

#include <mruby.h>
#include <mruby/array.h>
#include <mruby/class.h>
#include <mruby/numeric.h>
#include "moon/matrix4.hxx"
#include "moon/mrb/matrix4.hxx"
#include "moon/mrb/vector1.hxx"
#include "moon/mrb/vector2.hxx"
#include "moon/mrb/vector3.hxx"
#include "moon/mrb/vector4.hxx"

#define math_op_base(__op__, __target_mat4__)                                  \
  const mrb_vtype t = mrb_type(rother);                                        \
  if (t == MRB_TT_DATA) {                                                      \
    const mrb_data_type *dt = DATA_TYPE(rother);                               \
    if (dt == &matrix4_data_type) { /* Matrix4 */                              \
      Moon::Matrix4 *source_mat4 = mmrb_matrix4_ptr(mrb, rother);              \
      *__target_mat4__ __op__ ## = *source_mat4;                               \
    } else if (dt == &vector4_data_type) { /* Vector4 */                       \
      *__target_mat4__ __op__ ## = mmrb_to_vector4(mrb, rother);               \
    }                                                                          \
                                                                               \
  } else if (t == MRB_TT_FIXNUM || t == MRB_TT_FLOAT) { /* Scalar */           \
                                                                               \
    *__target_mat4__ __op__ ## = mrb_to_flo(mrb, rother);                      \
  } else {                                                                     \
    mrb_raisef(mrb, E_TYPE_ERROR,                                              \
               "wrong argument type %s (expected Matrix4, Vector4 or Numeric)",\
               mrb_obj_classname(mrb, rother));                                \
  }

#define math_op_inline(__op__)                                            \
  Moon::Matrix4 *target_mat4;                                             \
  mrb_value rother;                                                       \
  mrb_get_args(mrb, "o", &rother);                                        \
  target_mat4 = mmrb_matrix4_ptr(mrb, self);                              \
  math_op_base(__op__, target_mat4)                                       \
  return self;

#define math_op(__op__)                                                   \
  mrb_value rother;                                                       \
  mrb_get_args(mrb, "o", &rother);                                        \
  mrb_value rtarget = mrb_obj_dup(mrb, self);                             \
  Moon::Matrix4 *target_mat4 = mmrb_matrix4_ptr(mrb, rtarget);            \
  math_op_base(__op__, target_mat4)                                       \
  return rtarget; /* */

static void
matrix4_free(mrb_state *mrb, void *p)
{
  Moon::Matrix4 *matrix4 = (Moon::Matrix4*)p;
  if (matrix4) {
    delete(matrix4);
  }
}

MOON_C_API const struct mrb_data_type matrix4_data_type = { "Moon::Matrix4", matrix4_free };

MOON_C_API Moon::Matrix4
mmrb_to_matrix4(mrb_state *mrb, mrb_value self)
{
  return *mmrb_matrix4_ptr(mrb, self);
}

MOON_C_API mrb_value
mmrb_matrix4_value(mrb_state *mrb, Moon::Matrix4 mat)
{
  mrb_value rsult = mrb_obj_new(mrb, mmrb_get_matrix4_class(mrb), 0, NULL);
  Moon::Matrix4 *trns = mmrb_matrix4_ptr(mrb, rsult);
  *trns = mat;
  return rsult;
}

static void
matrix4_set_m(mrb_state *mrb, Moon::Matrix4 *mat, mrb_int argc, mrb_value *args)
{
  if (argc == 1) {
    mrb_value val = args[0];
    if (mrb_type(val) == MRB_TT_DATA) {
      if (DATA_TYPE(val) == &matrix4_data_type) { /* Matrix4 */
        Moon::Matrix4 *source_mat4;
        source_mat4 = (Moon::Matrix4*)mrb_data_get_ptr(mrb, val, &matrix4_data_type);
        *mat = *source_mat4;
      } else {
        mrb_raisef(mrb, E_TYPE_ERROR,
                   "wrong argument type %s (expected Matrix4)",
                   mrb_obj_classname(mrb, args[0]));
      }
    } else {
      double value = mrb_to_flo(mrb, args[0]);
      *mat = Moon::Matrix4(value);
    }
  } else if (argc == 4) {
    glm::vec4 row1 = mmrb_to_vector4(mrb, args[0]);
    glm::vec4 row2 = mmrb_to_vector4(mrb, args[1]);
    glm::vec4 row3 = mmrb_to_vector4(mrb, args[2]);
    glm::vec4 row4 = mmrb_to_vector4(mrb, args[3]);

    *mat = Moon::Matrix4(row1, row2, row3, row4);
  } else if (argc == 16) {
    *mat = Moon::Matrix4(
      mrb_to_flo(mrb, args[0]),
      mrb_to_flo(mrb, args[1]),
      mrb_to_flo(mrb, args[2]),
      mrb_to_flo(mrb, args[3]),

      mrb_to_flo(mrb, args[4]),
      mrb_to_flo(mrb, args[5]),
      mrb_to_flo(mrb, args[6]),
      mrb_to_flo(mrb, args[7]),

      mrb_to_flo(mrb, args[8]),
      mrb_to_flo(mrb, args[9]),
      mrb_to_flo(mrb, args[10]),
      mrb_to_flo(mrb, args[11]),

      mrb_to_flo(mrb, args[12]),
      mrb_to_flo(mrb, args[13]),
      mrb_to_flo(mrb, args[14]),
      mrb_to_flo(mrb, args[15])
    );
  } else {
    mrb_raisef(mrb, E_ARGUMENT_ERROR,
               "wrong argument count %d (expected 0, 1, 4, or 16)",
               argc);
  }
}

static mrb_value
matrix4_set(mrb_state *mrb, mrb_value self)
{
  mrb_value *args;
  mrb_int argc;
  mrb_get_args(mrb, "*", &args, &argc);
  matrix4_set_m(mrb, mmrb_matrix4_ptr(mrb, self), argc, args);
  return self;
}

static mrb_value
matrix4_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_value *args;
  mrb_int argc;
  mrb_get_args(mrb, "*", &args, &argc);

  Moon::Matrix4 *mat;

  mat = (Moon::Matrix4*)DATA_PTR(self);
  if (mat) {
    matrix4_free(mrb, (void*)mat);
  }
  mat = new Moon::Matrix4();
  mrb_data_init(self, mat, &matrix4_data_type);

  if (argc == 0) {
  } else {
    matrix4_set_m(mrb, mat, argc, args);
  }

  return self;
}

/*
 * @overload initialize_copy(Matrix4 other)
 * @return [self]
 */
static mrb_value
matrix4_initialize_copy(mrb_state *mrb, mrb_value self)
{
  Moon::Matrix4 *source_mat;
  mrb_get_args(mrb, "d", &source_mat, &matrix4_data_type);
  matrix4_free(mrb, DATA_PTR(self));
  mrb_data_init(self, new Moon::Matrix4(*source_mat), &matrix4_data_type);
  return self;
}

/*
 * @return [Array<Object>]
 * @api private
 */
static mrb_value
matrix4_coerce(mrb_state *mrb, mrb_value self)
{
  mrb_value other;
  mrb_get_args(mrb, "o", &other);
  mrb_value argv[2] = { self, other };
  return mrb_ary_new_from_values(mrb, 2, argv);
}

/*
 * @return [Boolean]
 */
static mrb_value
matrix4_eq(mrb_state *mrb, mrb_value self)
{
  mrb_value other;
  mrb_get_args(mrb, "o", &other);
  if (mrb_obj_is_kind_of(mrb, other, mmrb_get_matrix4_class(mrb))) {
    return mrb_bool_value((*mmrb_matrix4_ptr(mrb, self)) == (*mmrb_matrix4_ptr(mrb, other)));
  }
  return mrb_bool_value(false);
}

/*
 * @overload Matrix4#[int]
 *   @return [Vector4]
 * @overload Matrix4#[int, int]
 *   @return [Numeric]
 */
static mrb_value
matrix4_entry_get(mrb_state *mrb, mrb_value self)
{
  mrb_value *vals;
  int len;
  mrb_get_args(mrb, "*", &vals, &len);

  Moon::Matrix4 *mat4;
  mat4 = (Moon::Matrix4*)mrb_data_get_ptr(mrb, self, &matrix4_data_type);

  if (len == 1) {
    mrb_int x;

    mrb_get_args(mrb, "i", &x);

    if (x < 0 || x >= 4) {
      mrb_raisef(mrb, E_INDEX_ERROR,
                 "x %d is out of range (expected 0...4)", x);
      return mrb_nil_value();
    }

    return mmrb_vector4_value(mrb, (*mat4)[x]);
  } else if (len == 2) {
    mrb_int x, y;

    mrb_get_args(mrb, "ii", &x, &y);

    if (x < 0 || x >= 4) {
      mrb_raisef(mrb, E_INDEX_ERROR,
                 "x %d is out of range (expected 0...4)", x);
      return mrb_nil_value();
    }
    if (y < 0 || y >= 4) {
      mrb_raisef(mrb, E_INDEX_ERROR,
                 "y %d is out of range (expected 0...4)", y);
      return mrb_nil_value();
    }

    return mrb_float_value(mrb, (*mat4)[x][y]);
  } else {
    mrb_raisef(mrb, E_ARGUMENT_ERROR,
               "wrong number of arguments (%d for 1, or 2)", len);
  }
  return mrb_nil_value();
}

static mrb_value
matrix4_entry_set(mrb_state *mrb, mrb_value self)
{
  mrb_value *vals;
  int len;
  mrb_get_args(mrb, "*", &vals, &len);

  Moon::Matrix4 *mat4;
  mat4 = (Moon::Matrix4*)mrb_data_get_ptr(mrb, self, &matrix4_data_type);

  if (len == 2) {
    mrb_int x;
    mrb_value rvec4;

    mrb_get_args(mrb, "io", &x, &rvec4);

    if (x < 0 || x >= 4) {
      mrb_raisef(mrb, E_INDEX_ERROR,
                 "x %d is out of range (expected 0...4)", x);
      return mrb_nil_value();
    }

    (*mat4)[x] = mmrb_to_vector4(mrb, rvec4);
  } else if (len == 3) {
    mrb_int x, y;
    mrb_float v;

    mrb_get_args(mrb, "iif", &x, &y, &v);

    if (x < 0 || x >= 4) {
      mrb_raisef(mrb, E_INDEX_ERROR,
                 "x %d is out of range (expected 0...4)", x);
      return mrb_nil_value();
    }
    if (y < 0 || y >= 4) {
      mrb_raisef(mrb, E_INDEX_ERROR,
                 "y %d is out of range (expected 0...4)", y);
      return mrb_nil_value();
    }

    (*mat4)[x][y] = (double)v;
  } else {
    mrb_raisef(mrb, E_ARGUMENT_ERROR,
               "wrong number of arguments (%d for 2, or 3)", len);
  }
  return mrb_nil_value();
}

/* Returns the inverse of the matrix
 *
 * @return [Matrix4]
 */
static mrb_value
matrix4_op_negate(mrb_state *mrb, mrb_value self)
{
  mrb_value dest_mat4 = mrb_obj_dup(mrb, self);

  Moon::Matrix4* dmat4;
  dmat4 = (Moon::Matrix4*)mrb_data_get_ptr(mrb, dest_mat4, &matrix4_data_type);

  Moon::Matrix4* smat4;
  smat4 = (Moon::Matrix4*)mrb_data_get_ptr(mrb, self, &matrix4_data_type);

  *dmat4 = -(*smat4);

  return dest_mat4;
}

/* Returns self
 *
 * @return [Matrix4]
 */
static mrb_value
matrix4_op_identity(mrb_state *mrb, mrb_value self)
{
  return self;
}

/*
 * @return [Matrix4]
 */
static mrb_value
matrix4_op_add(mrb_state *mrb, mrb_value self)
{
  math_op(+)
}

static mrb_value
matrix4_add(mrb_state *mrb, mrb_value self)
{
  math_op_inline(+)
}

/*
 * @return [Matrix4]
 */
static mrb_value
matrix4_op_sub(mrb_state *mrb, mrb_value self)
{
  math_op(-)
}

static mrb_value
matrix4_sub(mrb_state *mrb, mrb_value self)
{
  math_op_inline(-)
}

/*
 * @return [Matrix4]
 */
static mrb_value
matrix4_op_mul(mrb_state *mrb, mrb_value self)
{
  math_op(*)
}

static mrb_value
matrix4_mul(mrb_state *mrb, mrb_value self)
{
  math_op_inline(*)
}

/*
 * @return [Matrix4]
 */
static mrb_value
matrix4_op_div(mrb_state *mrb, mrb_value self)
{
  math_op(/)
};

static mrb_value
matrix4_div(mrb_state *mrb, mrb_value self)
{
  math_op_inline(/)
}

/*
 * @return [Matrix4]
 */
//static mrb_value
//matrix4_op_mod(mrb_state *mrb, mrb_value self)
//{
//  math_op(%)
//}

/**
 * Sets all the matrix components to 1.0f
 *
 * @return [self]
 */
static mrb_value
matrix4_clear(mrb_state *mrb, mrb_value self)
{
  Moon::Matrix4 *mat4 = mmrb_matrix4_ptr(mrb, self);
  *mat4 = Moon::Matrix4(1.0f);
  return self;
}

/*
 * @return [Matrix4]
 */
static Moon::Matrix4
matrix4_translate_m(mrb_state *mrb, mrb_value self)
{
  mrb_value *vals;
  int len;
  mrb_get_args(mrb, "*", &vals, &len);
  Moon::Matrix4 *mat4 = mmrb_matrix4_ptr(mrb, self);
  Moon::Matrix4 target_mat4;
  if (len == 1) {
    glm::vec3 v3 = mmrb_to_vector3(mrb, vals[0]);
    target_mat4 = glm::translate(*mat4, v3);
  } else if (len == 3) {
    target_mat4 = glm::translate(*mat4, glm::vec3(
      mrb_to_flo(mrb, vals[0]),
      mrb_to_flo(mrb, vals[1]),
      mrb_to_flo(mrb, vals[2])));
  } else {
    mrb_raisef(mrb, E_ARGUMENT_ERROR,
               "wrong argument count %d (expected 1 or 3)",
               len);
  }
  return target_mat4;
};

/*
 * @return [Matrix4]
 */
static mrb_value
matrix4_translate(mrb_state *mrb, mrb_value self)
{
  Moon::Matrix4 *target_mat4;
  Moon::Matrix4 mat4 = matrix4_translate_m(mrb, self);
  mrb_value rtarget = mrb_obj_dup(mrb, self);
  target_mat4 = mmrb_matrix4_ptr(mrb, rtarget);
  *target_mat4 = mat4;
  return rtarget;
};

static mrb_value
matrix4_translate_bang(mrb_state *mrb, mrb_value self)
{
  Moon::Matrix4 *target_mat4;
  Moon::Matrix4 mat4 = matrix4_translate_m(mrb, self);
  target_mat4 = mmrb_matrix4_ptr(mrb, self);
  *target_mat4 = mat4;
  return self;
};

/*
 * @return [Matrix4]
 */
static Moon::Matrix4
matrix4_rotate_m(mrb_state *mrb, mrb_value self)
{
  mrb_value *vals;
  int len;
  mrb_get_args(mrb, "*", &vals, &len);
  Moon::Matrix4 *mat4 = mmrb_matrix4_ptr(mrb, self);
  Moon::Matrix4 target_mat4;

  if (len == 2) {
    mrb_float angle = mrb_to_flo(mrb, vals[0]);
    glm::vec3 rotate_v3 = mmrb_to_vector3(mrb, vals[1]);

    target_mat4 = glm::rotate(*mat4, glm::radians((float)angle), rotate_v3);
  } else if (len == 4) {
    target_mat4 = glm::rotate(*mat4,
      glm::radians((float)mrb_to_flo(mrb, vals[0])),
      glm::vec3(mrb_to_flo(mrb, vals[1]),
                mrb_to_flo(mrb, vals[2]),
                mrb_to_flo(mrb, vals[3])));
  } else {
    mrb_raisef(mrb, E_ARGUMENT_ERROR,
               "wrong argument count %d (expected 2 or 4)",
               len);
  }
  return target_mat4;
}

/*
 * @return [Matrix4]
 */
static mrb_value
matrix4_rotate(mrb_state *mrb, mrb_value self)
{
  Moon::Matrix4 *target_mat4;
  Moon::Matrix4 mat4 = matrix4_rotate_m(mrb, self);
  mrb_value rtarget = mrb_obj_dup(mrb, self);
  target_mat4 = mmrb_matrix4_ptr(mrb, rtarget);
  *target_mat4 = mat4;
  return rtarget;
};

static mrb_value
matrix4_rotate_bang(mrb_state *mrb, mrb_value self)
{
  Moon::Matrix4 *target_mat4;
  Moon::Matrix4 mat4 = matrix4_rotate_m(mrb, self);
  target_mat4 = mmrb_matrix4_ptr(mrb, self);
  *target_mat4 = mat4;
  return self;
};

static Moon::Matrix4
matrix4_scale_m(mrb_state *mrb, mrb_value self)
{
  mrb_value *vals;
  int len;
  mrb_get_args(mrb, "*", &vals, &len);

  Moon::Matrix4 *mat4 = mmrb_matrix4_ptr(mrb, self);
  Moon::Matrix4 target_mat4;

  if (len == 1) {
    glm::vec3 v3 = mmrb_to_vector3(mrb, vals[0]);

    target_mat4 = glm::scale(*mat4, v3);
  } else if (len == 3) {
    target_mat4 = glm::scale(*mat4, glm::vec3(
      mrb_to_flo(mrb, vals[0]),
      mrb_to_flo(mrb, vals[1]),
      mrb_to_flo(mrb, vals[2])));
  } else {
    mrb_raisef(mrb, E_ARGUMENT_ERROR,
               "wrong argument count %d (expected 1 or 3)",
               len);
  }
  return target_mat4;
}

static mrb_value
matrix4_scale(mrb_state *mrb, mrb_value self)
{
  Moon::Matrix4 *target_mat4;
  Moon::Matrix4 mat4 = matrix4_scale_m(mrb, self);
  mrb_value rtarget = mrb_obj_dup(mrb, self);
  target_mat4 = mmrb_matrix4_ptr(mrb, rtarget);
  *target_mat4 = mat4;
  return rtarget;
};

static mrb_value
matrix4_scale_bang(mrb_state *mrb, mrb_value self)
{
  Moon::Matrix4 *target_mat4;
  Moon::Matrix4 mat4 = matrix4_scale_m(mrb, self);
  target_mat4 = mmrb_matrix4_ptr(mrb, self);
  *target_mat4 = mat4;
  return self;
};

/* Converts the matrix to an Array of Floats
 * @return [Array<Numeric>]
 */
static mrb_value
matrix4_to_a16(mrb_state *mrb, mrb_value self)
{
  Moon::Matrix4 *mat4;
  mat4 = (Moon::Matrix4*)mrb_data_get_ptr(mrb, self, &matrix4_data_type);

  glm::vec4 row1 = (*mat4)[0];
  glm::vec4 row2 = (*mat4)[1];
  glm::vec4 row3 = (*mat4)[2];
  glm::vec4 row4 = (*mat4)[3];

  mrb_value argv[16] = {
    mrb_float_value(mrb, row1[0]),
    mrb_float_value(mrb, row1[1]),
    mrb_float_value(mrb, row1[2]),
    mrb_float_value(mrb, row1[3]),

    mrb_float_value(mrb, row2[0]),
    mrb_float_value(mrb, row2[1]),
    mrb_float_value(mrb, row2[2]),
    mrb_float_value(mrb, row2[3]),

    mrb_float_value(mrb, row3[0]),
    mrb_float_value(mrb, row3[1]),
    mrb_float_value(mrb, row3[2]),
    mrb_float_value(mrb, row3[3]),

    mrb_float_value(mrb, row4[0]),
    mrb_float_value(mrb, row4[1]),
    mrb_float_value(mrb, row4[2]),
    mrb_float_value(mrb, row4[3])
  };

  return mrb_ary_new_from_values(mrb, 16, argv);
}

/* Converts the matrix to an Array of Vector4s
 * @return [Array<Vector4>]
 */
static mrb_value
matrix4_to_a(mrb_state *mrb, mrb_value self)
{
  Moon::Matrix4 *mat4;
  mat4 = (Moon::Matrix4*)mrb_data_get_ptr(mrb, self, &matrix4_data_type);
  mrb_value argv[4] = { mmrb_vector4_value(mrb, (*mat4)[0]),
                        mmrb_vector4_value(mrb, (*mat4)[1]),
                        mmrb_vector4_value(mrb, (*mat4)[2]),
                        mmrb_vector4_value(mrb, (*mat4)[3]) };
  return mrb_ary_new_from_values(mrb, 4, argv);
}

/*
 * @overload Matrix4[obj]
 * @return [Matrix4]
 */
static mrb_value
matrix4_s_cast(mrb_state *mrb, mrb_value self)
{
  mrb_value *vals;
  int len;
  mrb_get_args(mrb, "*", &vals, &len);

  return mrb_obj_new(mrb, mmrb_get_matrix4_class(mrb), len, vals);
}

/**
 * Creates an Orthographic projection matrix
 *
 * @param [Float] a
 * @param [Float] b
 * @param [Float] c
 * @param [Float] d
 * @param [Float] e
 * @param [Float] f
 * @return [Moon::Matrix4] orthographic matrix
 */
static mrb_value
matrix4_s_ortho(mrb_state *mrb, mrb_value self)
{
  mrb_float a, b, c, d, e, f;
  mrb_get_args(mrb, "ffffff", &a, &b, &c, &d, &e, &f);
  return mmrb_matrix4_value(mrb, glm::ortho(a, b, c, d, e, f));
}

//static mrb_value s_extract(mrb_state *mrb, mrb_value self) {
//  return mrb_nil_value();
//};

MOON_C_API void
mmrb_matrix4_init(mrb_state *mrb)
{
  struct RClass *mod = mrb_define_module(mrb, "Moon");
  struct RClass *matrix4_class = mrb_define_class_under(mrb, mod, "Matrix4", mrb->object_class);
  MRB_SET_INSTANCE_TT(matrix4_class, MRB_TT_DATA);

  mrb_define_method(mrb, matrix4_class, "initialize",      matrix4_initialize,      MRB_ARGS_ANY());
  mrb_define_method(mrb, matrix4_class, "initialize_copy", matrix4_initialize_copy, MRB_ARGS_REQ(1));

  mrb_define_method(mrb, matrix4_class, "coerce",          matrix4_coerce,          MRB_ARGS_REQ(1));

  mrb_define_method(mrb, matrix4_class, "==",              matrix4_eq,              MRB_ARGS_REQ(1));

  mrb_define_method(mrb, matrix4_class, "[]",              matrix4_entry_get,       MRB_ARGS_ARG(1,1));
  mrb_define_method(mrb, matrix4_class, "[]=",             matrix4_entry_set,       MRB_ARGS_ARG(2,1));

  mrb_define_method(mrb, matrix4_class, "-@",              matrix4_op_negate,       MRB_ARGS_NONE());
  mrb_define_method(mrb, matrix4_class, "+@",              matrix4_op_identity,     MRB_ARGS_NONE());

  mrb_define_method(mrb, matrix4_class, "+",               matrix4_op_add,          MRB_ARGS_REQ(1));
  mrb_define_method(mrb, matrix4_class, "-",               matrix4_op_sub,          MRB_ARGS_REQ(1));
  mrb_define_method(mrb, matrix4_class, "*",               matrix4_op_mul,          MRB_ARGS_REQ(1));
  mrb_define_method(mrb, matrix4_class, "/",               matrix4_op_div,          MRB_ARGS_REQ(1));
  //mrb_define_method(mrb, matrix4_class, "%",               op_mod,          MRB_ARGS_REQ(1));

  mrb_define_method(mrb, matrix4_class, "add",             matrix4_add,             MRB_ARGS_REQ(1));
  mrb_define_method(mrb, matrix4_class, "sub",             matrix4_sub,             MRB_ARGS_REQ(1));
  mrb_define_method(mrb, matrix4_class, "mul",             matrix4_mul,             MRB_ARGS_REQ(1));
  mrb_define_method(mrb, matrix4_class, "div",             matrix4_div,             MRB_ARGS_REQ(1));

  mrb_define_method(mrb, matrix4_class, "set",             matrix4_set,             MRB_ARGS_ANY());
  mrb_define_method(mrb, matrix4_class, "clear",           matrix4_clear,           MRB_ARGS_NONE());

  mrb_define_method(mrb, matrix4_class, "translate",       matrix4_translate,       MRB_ARGS_ANY());
  mrb_define_method(mrb, matrix4_class, "rotate",          matrix4_rotate,          MRB_ARGS_ANY());
  mrb_define_method(mrb, matrix4_class, "scale",           matrix4_scale,           MRB_ARGS_ANY());

  mrb_define_method(mrb, matrix4_class, "translate!",      matrix4_translate_bang,  MRB_ARGS_ANY());
  mrb_define_method(mrb, matrix4_class, "rotate!",         matrix4_rotate_bang,     MRB_ARGS_ANY());
  mrb_define_method(mrb, matrix4_class, "scale!",          matrix4_scale_bang,      MRB_ARGS_ANY());

  mrb_define_method(mrb, matrix4_class, "to_a16",          matrix4_to_a16,          MRB_ARGS_NONE());
  mrb_define_method(mrb, matrix4_class, "to_a",            matrix4_to_a,            MRB_ARGS_NONE());

  mrb_define_class_method(mrb, matrix4_class, "[]",        matrix4_s_cast,          MRB_ARGS_ANY());
  mrb_define_class_method(mrb, matrix4_class, "ortho",     matrix4_s_ortho,         MRB_ARGS_REQ(6));
  //mrb_define_class_method(mrb, matrix4_class, "extract",   matrix4_s_extract,       MRB_ARGS_REQ(1));
}

