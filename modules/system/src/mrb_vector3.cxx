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
#include "vec_helper.h"

#define m_vector_operator(__op__) \
  return mmrb_vector3_value(mrb, get_vector3_value(mrb, self) __op__ vector3_from_mrb_args(mrb));

static struct RClass *vector3_class = NULL;

static void
vector3_free(mrb_state *mrb, void *p)
{
  Moon::Vector3 *vec3 = (Moon::Vector3*)p;
  if (vec3) {
    delete(vec3);
  }
}

MOON_C_API const struct mrb_data_type vector3_data_type = { "Vector3", vector3_free };

DEF_VEC_HELPERS(vector3, Moon::Vector3, vector3_class, &vector3_data_type);

static inline Moon::Vector3
mmrb_vector3_extract_mrb_num(mrb_state *mrb, mrb_value obj)
{
  double i = mmrb_to_flo(mrb, obj);
  return Moon::Vector3(i, i, i);
}

static inline Moon::Vector3
mmrb_vector3_extract_mrb_array(mrb_state *mrb, mrb_value obj)
{
  Moon::Vector3 result;
  int _ary_len = mrb_ary_len(mrb, obj);
  if (_ary_len != 3) {
    mrb_raisef(mrb, E_ARGUMENT_ERROR,
               "wrong array size %d (expected 3)", _ary_len);
  } else {
    result.x = mmrb_to_flo(mrb, RARRAY_PTR(obj)[0]);
    result.y = mmrb_to_flo(mrb, RARRAY_PTR(obj)[1]);
    result.z = mmrb_to_flo(mrb, RARRAY_PTR(obj)[2]);
  }
  return result;
}

static inline Moon::Vector3
mmrb_vector3_extract_mrb_to_vec3(mrb_state *mrb, mrb_value obj)
{
  return get_vector3_value(mrb, mrb_funcall(mrb, obj, "to_vec3", 0));
}

static Moon::Vector3
mmrb_vector3_extract_args(mrb_state *mrb, int argc, mrb_value *vals)
{
  Moon::Vector3 result;

  switch (argc) {
  case 1: {
    mrb_value val;
    val = vals[0];
    switch (mrb_type(val)) {
    case MRB_TT_FIXNUM:
    case MRB_TT_FLOAT:
      result = mmrb_vector3_extract_mrb_num(mrb, val);
      break;
    case MRB_TT_ARRAY:
      result = mmrb_vector3_extract_mrb_array(mrb, val);
      break;
    case MRB_TT_DATA:
      if (DATA_TYPE(val) == &vector3_data_type) {
        result = get_vector3_value(mrb, val);
        break;
      }
    default:
      if (mrb_respond_to(mrb, val, mrb_intern_cstr(mrb, "to_vec3"))) {
        result = mmrb_vector3_extract_mrb_to_vec3(mrb, val);
        break;
      } else {
        mrb_raisef(mrb, E_TYPE_ERROR,
                   "wrong type %S (expected Numeric, Array or Vector3)",
                   mrb_str_new_cstr(mrb, mrb_obj_classname(mrb, val)));
      }
    }
    break;
  } case 2: {
    int index = 0;
    for (int i = 0; i < 2; ++i) {
      mrb_value val = vals[i];
      if (mrb_type(val) == MRB_TT_DATA) {
        const mrb_data_type *t = (mrb_data_type*)DATA_TYPE(val);
        if (t == &vector1_data_type) {
          Moon::Vector1 v1 = mmrb_to_vector1(mrb, val);
          result[index++] = v1[0]; if (index >= 3) break;
        } else if (t == &vector2_data_type) {
          Moon::Vector2 v2 = mmrb_to_vector2(mrb, val);
          result[index++] = v2[0]; if (index >= 3) break;
          result[index++] = v2[1];
        } else {
          mrb_raisef(mrb, E_TYPE_ERROR,
                     "wrong type %S (expected Vector1, Vector2)",
                     mrb_str_new_cstr(mrb, mrb_obj_classname(mrb, val)));
        }
      } else if ((mrb_type(val) == MRB_TT_FIXNUM) || (mrb_type(val) == MRB_TT_FLOAT)) {
        result[index++] = mrb_to_flo(mrb, val);
      }
      if (index >= 3) break;
    };
    if (index < 3) {
      mrb_raisef(mrb, E_ARGUMENT_ERROR, "not enough parameters (required 3)");
    }
    break;
  } case 3: {
    result.x = mmrb_to_flo(mrb, vals[0]);
    result.y = mmrb_to_flo(mrb, vals[1]);
    result.z = mmrb_to_flo(mrb, vals[2]);
    break;
  } default:
    mrb_raisef(mrb, E_ARGUMENT_ERROR,
               //"wrong number of arguments (%d for 1 or 3)", argc);
               "wrong number of arguments (%d for 1, 2, or 3)", argc);
  }
  return result;
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

static mrb_value
vector3_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_float x = 0.0;
  mrb_float y = 0.0;
  mrb_float z = 0.0;
  Moon::Vector3 *vec3;
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

static mrb_value
vector3_coerce(mrb_state *mrb, mrb_value self)
{
  mrb_value other;
  mrb_get_args(mrb, "o", &other);
  mrb_value argv[2] = { self, other };
  return mrb_ary_new_from_values(mrb, 2, argv);
}

static mrb_value
vector3_get_x(mrb_state *mrb, mrb_value self)
{
  return mrb_float_value(mrb, get_vector3(mrb, self)->x);
}

static mrb_value
vector3_get_y(mrb_state *mrb, mrb_value self)
{
  return mrb_float_value(mrb, get_vector3(mrb, self)->y);
}

static mrb_value
vector3_get_z(mrb_state *mrb, mrb_value self)
{
  return mrb_float_value(mrb, get_vector3(mrb, self)->z);
}

static mrb_value
vector3_set_x(mrb_state *mrb, mrb_value self)
{
  mrb_float x;
  mrb_get_args(mrb, "f", &x);
  get_vector3(mrb, self)->x = x;
  return mrb_nil_value();
}

static mrb_value
vector3_set_y(mrb_state *mrb, mrb_value self)
{
  mrb_float y;
  mrb_get_args(mrb, "f", &y);
  get_vector3(mrb, self)->y = y;
  return mrb_nil_value();
}

static mrb_value
vector3_set_z(mrb_state *mrb, mrb_value self)
{
  mrb_float z;
  mrb_get_args(mrb, "f", &z);
  get_vector3(mrb, self)->z = z;
  return mrb_nil_value();
}

static mrb_value
vector3_negate(mrb_state *mrb, mrb_value self)
{
  return mmrb_vector3_value(mrb, -get_vector3_value(mrb, self));
}

static mrb_value
vector3_identity(mrb_state *mrb, mrb_value self)
{
  return mrb_obj_dup(mrb, self);
}

static mrb_value
vector3_normalize(mrb_state *mrb, mrb_value self)
{
  return mmrb_vector3_value(mrb, glm::normalize(get_vector3_value(mrb, self)));
}

static mrb_value
vector3_length(mrb_state *mrb, mrb_value self)
{
  return mrb_float_value(mrb, glm::length(get_vector3_value(mrb, self)));
}

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

//static mrb_value //vector3_mod(mrb_state *mrb, mrb_value self) {
//  m_vector_operator(%);
//};

static mrb_value
vector3_dot(mrb_state *mrb, mrb_value self)
{
  Moon::Vector3 *other;
  mrb_get_args(mrb, "d", &other, &vector3_data_type);
  return mrb_float_value(mrb, glm::dot(get_vector3_value(mrb, self), *other));
}

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

static mrb_value
vector3_distance(mrb_state *mrb, mrb_value self)
{
  Moon::Vector3 diff = get_vector3_value(mrb, self) - vector3_from_mrb_args(mrb);
  return mrb_float_value(mrb, glm::dot(diff, diff));
}

static mrb_value
vector3_rotate(mrb_state *mrb, mrb_value self)
{
  Moon::Vector3 *other;
  mrb_float angle;
  mrb_get_args(mrb, "df", &other, &vector3_data_type, &angle);
  return mmrb_vector3_value(mrb, glm::rotate(*get_vector3(mrb, self), (float)angle, *other));
}

static mrb_value
vector3_rotate_x(mrb_state *mrb, mrb_value self)
{
  mrb_float angle;
  mrb_get_args(mrb, "f", &angle);
  return mmrb_vector3_value(mrb, glm::rotateX(*get_vector3(mrb, self), (float)angle));
}

static mrb_value
vector3_rotate_y(mrb_state *mrb, mrb_value self)
{
  mrb_float angle;
  mrb_get_args(mrb, "f", &angle);
  return mmrb_vector3_value(mrb, glm::rotateY(*get_vector3(mrb, self), (float)angle));
}

static mrb_value
vector3_rotate_z(mrb_state *mrb, mrb_value self)
{
  mrb_float angle;
  mrb_get_args(mrb, "f", &angle);
  return mmrb_vector3_value(mrb, glm::rotateZ(*get_vector3(mrb, self), (float)angle));
}

static mrb_value
vector3_lerp(mrb_state *mrb, mrb_value self)
{
  Moon::Vector3 *other;
  mrb_float delta;
  mrb_get_args(mrb, "df", &other, &vector3_data_type, &delta);
  return mmrb_vector3_value(mrb, glm::lerp(*get_vector3(mrb, self), *other, (float)delta));
}

static mrb_value
vector3_slerp(mrb_state *mrb, mrb_value self)
{
  Moon::Vector3 *other;
  mrb_float delta;
  mrb_get_args(mrb, "df", &other, &vector3_data_type, &delta);
  return mmrb_vector3_value(mrb, glm::slerp(*get_vector3(mrb, self), *other, (float)delta));
}

static mrb_value
vector3_set(mrb_state *mrb, mrb_value self)
{
  Moon::Vector3 *v3;
  v3 = get_vector3(mrb, self);
  *v3 = vector3_from_mrb_args(mrb);
  return self;
}

static mrb_value
vector3_to_a(mrb_state *mrb, mrb_value self)
{
  Moon::Vector3 *mvec3 = get_vector3(mrb, self);
  mrb_value argv[3] = { mrb_float_value(mrb, mvec3->x),
                        mrb_float_value(mrb, mvec3->y),
                        mrb_float_value(mrb, mvec3->z) };
  return mrb_ary_new_from_values(mrb, 3, argv);
}

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
mmrb_vector3_init(mrb_state *mrb, struct RClass *mod)
{
  vector3_class = mrb_define_class_under(mrb, mod, "Vector3", mrb->object_class);
  MRB_SET_INSTANCE_TT(vector3_class, MRB_TT_DATA);

  mrb_define_method(mrb, vector3_class, "initialize",      vector3_initialize,      MRB_ARGS_OPT(3));
  mrb_define_method(mrb, vector3_class, "initialize_copy", vector3_initialize_copy, MRB_ARGS_REQ(1));
  /* coercion */
  mrb_define_method(mrb, vector3_class, "coerce",          vector3_coerce,          MRB_ARGS_REQ(1));
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
  /*mrb_define_method(mrb, vector3_class, "~@",              vector3_not,             MRB_ARGS_NONE());*/
  /*mrb_define_method(mrb, vector3_class, "%",               vector3_modulo,          MRB_ARGS_REQ(1));*/
  /*mrb_define_method(mrb, vector3_class, "<<",              vector3_shl,             MRB_ARGS_REQ(1));*/
  /*mrb_define_method(mrb, vector3_class, ">>",              vector3_shr,             MRB_ARGS_REQ(1));*/
  /*mrb_define_method(mrb, vector3_class, "&",               vector3_and,             MRB_ARGS_REQ(1));*/
  /*mrb_define_method(mrb, vector3_class, "|",               vector3_or,              MRB_ARGS_REQ(1));*/
  /*mrb_define_method(mrb, vector3_class, "^",               vector3_xor,             MRB_ARGS_REQ(1));*/
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
