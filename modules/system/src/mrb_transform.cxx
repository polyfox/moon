/*
 * Moon Transform, a wrapper around glm::mat4
 */
#define GLM_FORCE_RADIANS

#include <mruby.h>
#include <mruby/array.h>
#include <mruby/class.h>
#include <mruby/numeric.h>
#include "moon/transform.hxx"
#include "moon/mrb/transform.hxx"
#include "moon/mrb/vector1.hxx"
#include "moon/mrb/vector2.hxx"
#include "moon/mrb/vector3.hxx"
#include "moon/mrb/vector4.hxx"

#define math_op(__op__)                                                   \
  mrb_value rother;                                                       \
  mrb_get_args(mrb, "o", &rother);                                        \
  mrb_value rtarget = mrb_obj_dup(mrb, self);                             \
  Moon::Transform *target_mat4 = get_transform(mrb, rtarget);             \
  const mrb_vtype t = mrb_type(rother);                                   \
  if (t == MRB_TT_DATA) {                                                 \
    const mrb_data_type *dt = DATA_TYPE(rother);                          \
    if (dt == &transform_data_type) { /* Transform */                     \
      Moon::Transform *source_mat4 = get_transform(mrb, rother);          \
      *target_mat4 __op__ ## = *source_mat4;                              \
    } else if (dt == &vector4_data_type) { /* Vector4 */                  \
      *target_mat4 __op__ ## = mmrb_to_vector4(mrb, rother);              \
    }                                                                     \
  } else if (t == MRB_TT_FIXNUM || t == MRB_TT_FLOAT) { /* Scalar */      \
                                                                          \
    *target_mat4 __op__ ## = mrb_to_flo(mrb, rother);                     \
  } else {                                                                \
    mrb_raisef(mrb, E_TYPE_ERROR,                                         \
               "wrong argument type %s (expected Transform, Vector4 or Numeric)", \
               mrb_obj_classname(mrb, rother));                           \
  }                                                                       \
  return rtarget; /* */

static struct RClass *transform_class = NULL;

static void
transform_free(mrb_state *mrb, void *p)
{
  Moon::Transform *transform = (Moon::Transform*)p;
  if (transform) {
    delete(transform);
  }
}

MOON_C_API const struct mrb_data_type transform_data_type = { "Transform", transform_free };

static inline Moon::Transform*
get_transform(mrb_state *mrb, mrb_value self)
{
  return (Moon::Transform*)mrb_data_get_ptr(mrb, self, &transform_data_type);
}

MOON_C_API Moon::Transform
mmrb_to_transform(mrb_state *mrb, mrb_value self)
{
  return *get_transform(mrb, self);
}

MOON_C_API mrb_value
mmrb_transform_value(mrb_state *mrb, Moon::Transform mat)
{
  mrb_value rsult = mrb_obj_new(mrb, transform_class, 0, NULL);
  Moon::Transform *trns = get_transform(mrb, rsult);
  *trns = mat;
  return rsult;
}

/*
 * @overload Transform#initialize()
 * @overload Transform#initialize(Numeric)
 * @overload Transform#initialize(Transform)
 * @overload Transform#initialize(Vector4, Vector4, Vector4, Vector4)
 * @overload Transform#initialize(n1, ..., n16)
 */
static mrb_value
transform_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_value *args;
  int argc;
  mrb_get_args(mrb, "*", &args, &argc);

  Moon::Transform *mat;

  mat = (Moon::Transform*)DATA_PTR(self);
  if (mat) {
    transform_free(mrb, (void*)mat);
  }

  if (argc == 0){
    mat = new Moon::Transform();
  } else if (argc == 1) {
    mrb_value val = args[0];
    if (mrb_type(val) == MRB_TT_DATA) {
      if (DATA_TYPE(val) == &transform_data_type) { /* Transform */
        Moon::Transform *source_mat4;
        source_mat4 = (Moon::Transform*)mrb_data_get_ptr(mrb, val, &transform_data_type);
        mat = new Moon::Transform(*source_mat4);
      } else {
        mrb_raisef(mrb, E_TYPE_ERROR,
                   "wrong argument type %s (expected Transform)",
                   mrb_obj_classname(mrb, args[0]));
        return mrb_nil_value();
      }
    } else {
      double value = mrb_to_flo(mrb, args[0]);
      mat = new Moon::Transform(value);
    }
  } else if (argc == 4) {
    glm::vec4 row1 = mmrb_to_vector4(mrb, args[0]);
    glm::vec4 row2 = mmrb_to_vector4(mrb, args[1]);
    glm::vec4 row3 = mmrb_to_vector4(mrb, args[2]);
    glm::vec4 row4 = mmrb_to_vector4(mrb, args[3]);

    mat = new Moon::Transform(row1, row2, row3, row4);
  } else if (argc == 16) {
    mat = new Moon::Transform(
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

  mrb_data_init(self, mat, &transform_data_type);

  return self;
}

/*
 * @overload initialize_copy(Transform other)
 * @return [nil]
 */
static mrb_value
transform_initialize_copy(mrb_state *mrb, mrb_value self)
{
  Moon::Transform *source_mat;
  mrb_get_args(mrb, "d", &source_mat, &transform_data_type);
  transform_free(mrb, DATA_PTR(self));
  mrb_data_init(self, new Moon::Transform(*source_mat), &transform_data_type);
  return self;
}

/*
 * @return [Array<Object>]
 */
static mrb_value
transform_coerce(mrb_state *mrb, mrb_value self)
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
transform_eq(mrb_state *mrb, mrb_value self)
{
  mrb_value other;
  mrb_get_args(mrb, "o", &other);
  if (mrb_obj_is_kind_of(mrb, other, transform_class)) {
    return mrb_bool_value((*get_transform(mrb, self)) == (*get_transform(mrb, other)));
  }
  return mrb_bool_value(false);
}

/*
 * @overload Transform#[int]
 *   @return [Vector4]
 * @overload Transform#[int, int]
 *   @return [Numeric]
 */
static mrb_value
transform_entry_get(mrb_state *mrb, mrb_value self)
{
  mrb_value *vals;
  int len;
  mrb_get_args(mrb, "*", &vals, &len);

  Moon::Transform *mat4;
  mat4 = (Moon::Transform*)mrb_data_get_ptr(mrb, self, &transform_data_type);

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

/*
 * @overload Transform#[num] = Vector4 value
 * @overload Transform#[num, num] = Numeric value
 */
static mrb_value
transform_entry_set(mrb_state *mrb, mrb_value self)
{
  mrb_value *vals;
  int len;
  mrb_get_args(mrb, "*", &vals, &len);

  Moon::Transform *mat4;
  mat4 = (Moon::Transform*)mrb_data_get_ptr(mrb, self, &transform_data_type);

  if (len == 2) {
    mrb_int x;
    mrb_value rvec4;

    mrb_get_args(mrb, "io", &x, &rvec4);

    if (x < 0 || x >= 4) {
      mrb_raisef(mrb, E_INDEX_ERROR,
                 "x %d is out of range (expected 0...4)", x);
      return mrb_nil_value();
    }

    Moon::Vector4 *vec4;
    vec4 = (Moon::Vector4*)mrb_data_get_ptr(mrb, rvec4, &vector4_data_type);

    (*mat4)[x] = *vec4;
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

/*
 * @return [Transform]
 */
static mrb_value
transform_op_negate(mrb_state *mrb, mrb_value self)
{
  mrb_value dest_mat4 = mrb_obj_dup(mrb, self);

  Moon::Transform* dmat4;
  dmat4 = (Moon::Transform*)mrb_data_get_ptr(mrb, dest_mat4, &transform_data_type);

  Moon::Transform* smat4;
  smat4 = (Moon::Transform*)mrb_data_get_ptr(mrb, self, &transform_data_type);

  *dmat4 = -(*smat4);

  return dest_mat4;
}

/*
 * @return [Transform]
 */
static mrb_value
transform_op_identity(mrb_state *mrb, mrb_value self)
{
  return mrb_obj_dup(mrb, self);
}

/*
 * @return [Transform]
 */
static mrb_value
transform_op_add(mrb_state *mrb, mrb_value self)
{
  math_op(+)
}

/*
 * @return [Transform]
 */
static mrb_value
transform_op_sub(mrb_state *mrb, mrb_value self)
{
  math_op(-)
}

/*
 * @return [Transform]
 */
static mrb_value
transform_op_mul(mrb_state *mrb, mrb_value self)
{
  math_op(*)
}

/*
 * @return [Transform]
 */
static mrb_value
transform_op_div(mrb_state *mrb, mrb_value self)
{
  math_op(/)
};

/*
 * @return [Transform]
 */
//static mrb_value
//transform_op_mod(mrb_state *mrb, mrb_value self)
//{
//  math_op(%)
//}

/*
 * @return [Transform]
 */
static mrb_value
transform_translate(mrb_state *mrb, mrb_value self)
{
  mrb_value *vals;
  int len;
  mrb_get_args(mrb, "*", &vals, &len);

  mrb_value rtarget = mrb_obj_dup(mrb, self);
  Moon::Transform *target_mat4;
  target_mat4 = (Moon::Transform*)mrb_data_get_ptr(mrb, rtarget, &transform_data_type);

  if (len == 1) {
    glm::vec3 v3 = mmrb_to_vector3(mrb, vals[0]);

    *target_mat4 = glm::translate(*target_mat4, v3);
  } else if (len == 3) {
    *target_mat4 = glm::translate(*target_mat4, glm::vec3(
      mrb_to_flo(mrb, vals[0]),
      mrb_to_flo(mrb, vals[1]),
      mrb_to_flo(mrb, vals[2])));
  } else {
    mrb_raisef(mrb, E_ARGUMENT_ERROR,
               "wrong argument count %d (expected 1 or 3)",
               len);
  }
  return rtarget;
};

/*
 * @return [Transform]
 */
static mrb_value
transform_rotate(mrb_state *mrb, mrb_value self)
{
  mrb_value *vals;
  int len;
  mrb_get_args(mrb, "*", &vals, &len);

  mrb_value rtarget = mrb_obj_dup(mrb, self);
  Moon::Transform *target_mat4;
  target_mat4 = (Moon::Transform*)mrb_data_get_ptr(mrb, rtarget, &transform_data_type);

  if (len == 2) {
    mrb_float angle = mrb_to_flo(mrb, vals[0]);
    glm::vec3 rotate_v3 = mmrb_to_vector3(mrb, vals[1]);

    *target_mat4 = glm::rotate(*target_mat4, glm::radians((float)angle), rotate_v3);
  } else if (len == 4) {
    *target_mat4 = glm::rotate(*target_mat4,
      glm::radians((float)mrb_to_flo(mrb, vals[0])),
      glm::vec3(mrb_to_flo(mrb, vals[1]),
                mrb_to_flo(mrb, vals[2]),
                mrb_to_flo(mrb, vals[3])));
  } else {
    mrb_raisef(mrb, E_ARGUMENT_ERROR,
               "wrong argument count %d (expected 2 or 4)",
               len);
  }
  return rtarget;
}

/*
 * @return [Transform]
 */
static mrb_value
transform_scale(mrb_state *mrb, mrb_value self)
{
  mrb_value *vals;
  int len;
  mrb_get_args(mrb, "*", &vals, &len);

  mrb_value rtarget = mrb_obj_dup(mrb, self);
  Moon::Transform *target_mat4;
  target_mat4 = (Moon::Transform*)mrb_data_get_ptr(mrb, rtarget, &transform_data_type);

  if (len == 1) {
    glm::vec3 v3 = mmrb_to_vector3(mrb, vals[0]);

    *target_mat4 = glm::scale(*target_mat4, v3);
  } else if (len == 3) {
    *target_mat4 = glm::scale(*target_mat4, glm::vec3(
      mrb_to_flo(mrb, vals[0]),
      mrb_to_flo(mrb, vals[1]),
      mrb_to_flo(mrb, vals[2])));
  } else {
    mrb_raisef(mrb, E_ARGUMENT_ERROR,
               "wrong argument count %d (expected 1 or 3)",
               len);
  }
  return rtarget;
}

/*
 * @return [Array<Numeric>]
 */
static mrb_value
transform_to_a16(mrb_state *mrb, mrb_value self)
{
  Moon::Transform *mat4;
  mat4 = (Moon::Transform*)mrb_data_get_ptr(mrb, self, &transform_data_type);

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

/*
 * @return [Array<Vector4>]
 */
static mrb_value
transform_to_a(mrb_state *mrb, mrb_value self)
{
  Moon::Transform *mat4;
  mat4 = (Moon::Transform*)mrb_data_get_ptr(mrb, self, &transform_data_type);
  mrb_value argv[4] = { mmrb_vector4_value(mrb, (*mat4)[0]),
                        mmrb_vector4_value(mrb, (*mat4)[1]),
                        mmrb_vector4_value(mrb, (*mat4)[2]),
                        mmrb_vector4_value(mrb, (*mat4)[3]) };
  return mrb_ary_new_from_values(mrb, 4, argv);
}

/*
 * @overload Transform[obj]
 * @return [Transform]
 */
static mrb_value
transform_s_cast(mrb_state *mrb, mrb_value self)
{
  mrb_value *vals;
  int len;
  mrb_get_args(mrb, "*", &vals, &len);

  return mrb_obj_new(mrb, transform_class, len, vals);
}

static mrb_value
transform_s_ortho(mrb_state *mrb, mrb_value self)
{
  mrb_float a, b, c, d, e, f;
  mrb_get_args(mrb, "ffffff", &a, &b, &c, &d, &e, &f);
  return mmrb_transform_value(mrb, glm::ortho(a, b, c, d, e, f));
}

//static mrb_value s_extract(mrb_state *mrb, mrb_value self) {
//  return mrb_nil_value();
//};

MOON_C_API void
mmrb_transform_init(mrb_state *mrb, struct RClass* mod)
{
  transform_class = mrb_define_class_under(mrb, mod, "Transform", mrb->object_class);
  MRB_SET_INSTANCE_TT(transform_class, MRB_TT_DATA);

  mrb_define_method(mrb, transform_class, "initialize",      transform_initialize,      MRB_ARGS_ANY());
  mrb_define_method(mrb, transform_class, "initialize_copy", transform_initialize_copy, MRB_ARGS_REQ(1));

  mrb_define_method(mrb, transform_class, "coerce",          transform_coerce,          MRB_ARGS_REQ(1));

  mrb_define_method(mrb, transform_class, "==",              transform_eq,              MRB_ARGS_REQ(1));

  mrb_define_method(mrb, transform_class, "[]",              transform_entry_get,       MRB_ARGS_ARG(1,1));
  mrb_define_method(mrb, transform_class, "[]=",             transform_entry_set,       MRB_ARGS_ARG(2,1));

  mrb_define_method(mrb, transform_class, "-@",              transform_op_negate,       MRB_ARGS_NONE());
  mrb_define_method(mrb, transform_class, "+@",              transform_op_identity,     MRB_ARGS_NONE());

  mrb_define_method(mrb, transform_class, "+",               transform_op_add,          MRB_ARGS_REQ(1));
  mrb_define_method(mrb, transform_class, "-",               transform_op_sub,          MRB_ARGS_REQ(1));
  mrb_define_method(mrb, transform_class, "*",               transform_op_mul,          MRB_ARGS_REQ(1));
  mrb_define_method(mrb, transform_class, "/",               transform_op_div,          MRB_ARGS_REQ(1));
  //mrb_define_method(mrb, transform_class, "%",               op_mod,          MRB_ARGS_REQ(1));

  mrb_define_method(mrb, transform_class, "translate",       transform_translate,       MRB_ARGS_ANY());
  mrb_define_method(mrb, transform_class, "rotate",          transform_rotate,          MRB_ARGS_ANY());
  mrb_define_method(mrb, transform_class, "scale",           transform_scale,           MRB_ARGS_ANY());

  mrb_define_method(mrb, transform_class, "to_a16",          transform_to_a16,          MRB_ARGS_NONE());
  mrb_define_method(mrb, transform_class, "to_a",            transform_to_a,            MRB_ARGS_NONE());

  mrb_define_class_method(mrb, transform_class, "[]",        transform_s_cast,          MRB_ARGS_ANY());
  mrb_define_class_method(mrb, transform_class, "ortho",     transform_s_ortho,         MRB_ARGS_REQ(6));
  //mrb_define_class_method(mrb, transform_class, "extract",   transform_s_extract,       MRB_ARGS_REQ(1));
}

