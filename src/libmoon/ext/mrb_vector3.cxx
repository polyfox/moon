/*
 * Moon Vector3, a wrapper around glm::vec3
 */
#include "moon/mrb/vector3.hxx"
#include "moon/mrb/vector2.hxx"
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtx/compatibility.hpp>
#include "moon/mrb/shared_types.hxx"

#define VECTOR_DATA_TYPE &vector3_data_type
#define VECTOR_MOON_STRUCT moon_vec3
#define VECTOR_STRUCT glm::vec3
#define VECTOR_EXTRACT_FUNC(_mrb_) mmrb_vector3_extract_mrb_args(_mrb_)
#define VECTOR_CLASS vector3_class
#define VECTOR_VALUE mmrb_vector3_value
#define moon_vector_ptr moon_vec3_ptr
#define moon_vector_ref moon_vec3_ref
#include "moon/mrb/vector_private.hxx"

static struct RClass *VECTOR_CLASS = NULL;

static void
vector3_free(mrb_state *mrb, void *p)
{
  VECTOR_MOON_STRUCT *vec3 = (VECTOR_MOON_STRUCT*)p;
  if (vec3) {
    delete(vec3);
  }
}

struct mrb_data_type vector3_data_type = { "Vector3", vector3_free };

static inline void
mrb_set_vector3_value_xyz(mrb_state *mrb, mrb_value target, mrb_float x, mrb_float y, mrb_float z)
{
  VECTOR_MOON_STRUCT *vect = new VECTOR_MOON_STRUCT(new VECTOR_STRUCT(x, y, z));
  mrb_data_init(target, vect, VECTOR_DATA_TYPE);
}

mrb_value
mmrb_vector3_value(mrb_state *mrb, VECTOR_STRUCT vec)
{
  VECTOR_MOON_STRUCT *vec_ptr = new VECTOR_MOON_STRUCT(new VECTOR_STRUCT(vec));
  return mrb_obj_value(Data_Wrap_Struct(mrb, VECTOR_CLASS, VECTOR_DATA_TYPE, vec_ptr));
}

mrb_value
mmrb_vector3_wrap(mrb_state *mrb, VECTOR_STRUCT *ptr)
{
  VECTOR_MOON_STRUCT *vec_ptr = new VECTOR_MOON_STRUCT(ptr);
  return mrb_obj_value(Data_Wrap_Struct(mrb, VECTOR_CLASS, VECTOR_DATA_TYPE, vec_ptr));
}

mrb_value
mmrb_vector3_wrap(mrb_state *mrb, VECTOR_MOON_STRUCT moonv)
{
  VECTOR_MOON_STRUCT *vec_ptr = new VECTOR_MOON_STRUCT(moonv);
  return mrb_obj_value(Data_Wrap_Struct(mrb, VECTOR_CLASS, VECTOR_DATA_TYPE, vec_ptr));
}

mrb_value
mmrb_vector3_create(mrb_state *mrb, double x, double y, double z)
{
  return mmrb_vector3_wrap(mrb, new VECTOR_STRUCT(x, y, z));
}

/*
 * Black Magic
 */
VECTOR_STRUCT
mmrb_vector3_extract_mrb_vec3(mrb_state *mrb, mrb_value obj)
{
  m_vector_unwrap(obj, vec);
  return moon_vector_ref(vec);
}

static VECTOR_STRUCT
mmrb_vector3_extract_mrb_num(mrb_state *mrb, mrb_value obj)
{
  double i = mrb_to_flo(mrb, obj);
  return VECTOR_STRUCT(i, i, i);
}

static VECTOR_STRUCT
mmrb_vector3_extract_mrb_array(mrb_state *mrb, mrb_value obj)
{
  VECTOR_STRUCT result;
  int _ary_len = mrb_ary_len(mrb, obj);
  if (_ary_len != 3) {
    mrb_raisef(mrb, E_ARGUMENT_ERROR,
               "wrong array size %d (expected 3)", _ary_len);
  } else {
    result.x = mrb_to_flo(mrb, RARRAY_PTR(obj)[0]);
    result.y = mrb_to_flo(mrb, RARRAY_PTR(obj)[1]);
    result.z = mrb_to_flo(mrb, RARRAY_PTR(obj)[2]);
  }
  return result;
}

static VECTOR_STRUCT
mmrb_vector3_extract_mrb_to_vec3(mrb_state *mrb, mrb_value obj)
{
  return mmrb_vector3_extract_mrb_vec3(mrb, mrb_funcall(mrb, obj, "to_vec3", 0));
}

VECTOR_STRUCT
mmrb_vector3_extract_args(mrb_state *mrb, int argc, mrb_value *vals)
{
  VECTOR_STRUCT result;

  switch (argc) {
  case 1:
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
      if (DATA_TYPE(val) == VECTOR_DATA_TYPE) {
        result = mmrb_vector3_extract_mrb_vec3(mrb, val);
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
  case 2:
    int index;
    index = 0;
    for (int i=0; i < 2; ++i) {
      mrb_value val = vals[i];
      if (mrb_type(val) == MRB_TT_DATA) {
        if (DATA_TYPE(val) == &vector2_data_type) {
          glm::vec2 v2 = mmrb_vector2_extract_mrb_vec2(mrb, val);
          result[index++] = v2.x;
          if (index >= 3) { break; }
          result[index++] = v2.y;
        } else {
          mrb_raisef(mrb, E_TYPE_ERROR,
                     "wrong type %S (expected Vector2)",
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
  case 3:
    result.x = mrb_to_flo(mrb, vals[0]);
    result.y = mrb_to_flo(mrb, vals[1]);
    result.z = mrb_to_flo(mrb, vals[2]);
    break;
  default:
    mrb_raisef(mrb, E_ARGUMENT_ERROR,
               //"wrong number of arguments (%d for 1 or 3)", argc);
               "wrong number of arguments (%d for 1, 2, or 3)", argc);
  }
  return result;
}

static VECTOR_STRUCT
mmrb_vector3_extract_mrb_args(mrb_state *mrb)
{
  mrb_value *vals;
  int len;
  mrb_get_args(mrb, "*", &vals, &len);
  return mmrb_vector3_extract_args(mrb, len, vals);
}

static mrb_value
vector3_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_float x = 0.0;
  mrb_float y = 0.0;
  mrb_float z = 0.0;
  VECTOR_MOON_STRUCT *vec3;
  mrb_get_args(mrb, "|fff", &x, &y, &z);
  vec3 = (VECTOR_MOON_STRUCT*)DATA_PTR(self);
  if (vec3) {
    vector3_free(mrb, (void*)vec3);
  }
  mrb_set_vector3_value_xyz(mrb, self, x, y, z);
  return self;
};

static mrb_value
vector3_initialize_copy(mrb_state *mrb, mrb_value self)
{
  VECTOR_MOON_STRUCT* src_vec;
  mrb_get_args(mrb, "d", &src_vec, VECTOR_DATA_TYPE);
  mrb_set_vector3_value_xyz(mrb, self, moon_vector_ptr(src_vec)->x,
                                       moon_vector_ptr(src_vec)->y,
                                       moon_vector_ptr(src_vec)->z);
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
vector3_x_getter(mrb_state *mrb, mrb_value self)
{
  m_vector_unwrap_self(vec);
  return mrb_float_value(mrb, moon_vector_ptr(vec)->x);
}

static mrb_value
vector3_y_getter(mrb_state *mrb, mrb_value self)
{
  m_vector_unwrap_self(vec);
  return mrb_float_value(mrb, moon_vector_ptr(vec)->y);
}

static mrb_value
vector3_z_getter(mrb_state *mrb, mrb_value self)
{
  m_vector_unwrap_self(vec);
  return mrb_float_value(mrb, moon_vector_ptr(vec)->z);
}

static mrb_value
vector3_x_setter(mrb_state *mrb, mrb_value self)
{
  mrb_float x;
  m_vector_unwrap_self(vec);
  mrb_get_args(mrb, "f", &x);
  moon_vector_ptr(vec)->x = x;
  return mrb_nil_value();
}

static mrb_value
vector3_y_setter(mrb_state *mrb, mrb_value self)
{
  mrb_float y;
  m_vector_unwrap_self(vec);
  mrb_get_args(mrb, "f", &y);
  moon_vector_ptr(vec)->y = y;
  return mrb_nil_value();
}

static mrb_value
vector3_z_setter(mrb_state *mrb, mrb_value self)
{
  mrb_float z;
  m_vector_unwrap_self(vec);
  mrb_get_args(mrb, "f", &z);
  moon_vector_ptr(vec)->z = z;
  return mrb_nil_value();
}

static mrb_value
vector3_negate(mrb_state *mrb, mrb_value self)
{
  m_vector_mutate_copy(-(moon_vector_ref(result_vec)));
}

static mrb_value
vector3_identity(mrb_state *mrb, mrb_value self)
{
  return mrb_obj_dup(mrb, self);
}

static mrb_value
vector3_normalize(mrb_state *mrb, mrb_value self)
{
  m_vector_mutate_copy(glm::normalize(moon_vector_ref(result_vec)));
}

static mrb_value
vector3_length(mrb_state *mrb, mrb_value self)
{
  m_vector_unwrap_self(vec);
  return mrb_float_value(mrb, glm::length(moon_vector_ref(vec)));
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
  m_vector_operator_head(src_mvec, arg_vec);
  return mrb_float_value(mrb, glm::dot(moon_vector_ref(src_mvec), arg_vec));
}

static mrb_value
vector3_cross(mrb_state *mrb, mrb_value self)
{
  m_vector_operator_head(src_mvec, arg_vec);
  mrb_value dest_vec = mrb_obj_dup(mrb, self);
  moon_vector_ref((VECTOR_MOON_STRUCT*)DATA_PTR(dest_vec)) = glm::cross(moon_vector_ref(src_mvec), arg_vec);
  return dest_vec;
}

static mrb_value
vector3_distance(mrb_state *mrb, mrb_value self)
{
  m_vector_operator_head(src_mvec, arg_vec);
  VECTOR_STRUCT diff = moon_vector_ref(src_mvec) - arg_vec;
  return mrb_float_value(mrb, glm::dot(diff, diff));
}

static mrb_value
vector3_rotate(mrb_state *mrb, mrb_value self)
{
  VECTOR_MOON_STRUCT *other;
  mrb_float angle;
  mrb_get_args(mrb, "df", &other, VECTOR_DATA_TYPE, &angle);
  m_vector_unwrap_self(vec);
  return VECTOR_VALUE(mrb, glm::rotate(moon_vector_ref(vec), (float)angle, moon_vector_ref(other)));
}

static mrb_value
vector3_rotate_x(mrb_state *mrb, mrb_value self)
{
  mrb_float angle;
  mrb_get_args(mrb, "f", &angle);
  m_vector_unwrap_self(vec);
  return VECTOR_VALUE(mrb, glm::rotateX(moon_vector_ref(vec), (float)angle));
}

static mrb_value
vector3_rotate_y(mrb_state *mrb, mrb_value self)
{
  mrb_float angle;
  mrb_get_args(mrb, "f", &angle);
  m_vector_unwrap_self(vec);
  return VECTOR_VALUE(mrb, glm::rotateY(moon_vector_ref(vec), (float)angle));
}

static mrb_value
vector3_rotate_z(mrb_state *mrb, mrb_value self)
{
  mrb_float angle;
  mrb_get_args(mrb, "f", &angle);
  m_vector_unwrap_self(vec);
  return VECTOR_VALUE(mrb, glm::rotateZ(moon_vector_ref(vec), (float)angle));
}

static mrb_value
vector3_lerp(mrb_state *mrb, mrb_value self)
{
  VECTOR_MOON_STRUCT *other;
  mrb_float delta;
  mrb_get_args(mrb, "df", &other, VECTOR_DATA_TYPE, &delta);
  m_vector_unwrap_self(vec);
  return mmrb_vector3_value(mrb, glm::lerp(moon_vector_ref(vec), moon_vector_ref(other), (float)delta));
}

static mrb_value
vector3_slerp(mrb_state *mrb, mrb_value self)
{
  VECTOR_MOON_STRUCT *other;
  mrb_float delta;
  mrb_get_args(mrb, "df", &other, VECTOR_DATA_TYPE, &delta);
  m_vector_unwrap_self(vec);
  return mmrb_vector3_value(mrb, glm::slerp(moon_vector_ref(vec), moon_vector_ref(other), (float)delta));
}

static mrb_value
vector3_set(mrb_state *mrb, mrb_value self)
{
  m_vector_unwrap_self(mvec);
  m_vector_extract_to(moon_vector_ref(mvec));
  return self;
}

static mrb_value
vector3_to_a(mrb_state *mrb, mrb_value self)
{
  m_vector_unwrap_self(mvec);
  mrb_value argv[3] = { mrb_float_value(mrb, moon_vector_ptr(mvec)->x),
                        mrb_float_value(mrb, moon_vector_ptr(mvec)->y),
                        mrb_float_value(mrb, moon_vector_ptr(mvec)->z) };
  return mrb_ary_new_from_values(mrb, 3, argv);
}

static mrb_value
vector3_s_extract(mrb_state *mrb, mrb_value self)
{
  m_vector_extract(src_vect);
  mrb_value argv[3] = { mrb_float_value(mrb, src_vect.x),
                        mrb_float_value(mrb, src_vect.y),
                        mrb_float_value(mrb, src_vect.z) };
  return mrb_ary_new_from_values(mrb, 3, argv);
}

static mrb_value
vector3_s_cast(mrb_state *mrb, mrb_value klass)
{
  mrb_value result = mrb_obj_new(mrb, VECTOR_CLASS, 0, {});
  m_vector_unwrap(result, dest_vect);
  m_vector_extract(src_vect);
  moon_vector_ref(dest_vect) = src_vect;
  return result;
}

struct RClass*
mmrb_vector3_init(mrb_state *mrb)
{
  VECTOR_CLASS = mrb_define_class_under(mrb, mmrb_Moon, "Vector3", mrb->object_class);
  MRB_SET_INSTANCE_TT(VECTOR_CLASS, MRB_TT_DATA);

  mrb_define_method(mrb, VECTOR_CLASS, "initialize",      vector3_initialize,      MRB_ARGS_OPT(3));
  mrb_define_method(mrb, VECTOR_CLASS, "initialize_copy", vector3_initialize_copy, MRB_ARGS_REQ(1));
  /* coercion */
  mrb_define_method(mrb, VECTOR_CLASS, "coerce",          vector3_coerce,          MRB_ARGS_REQ(1));
  /* attribute setters */
  mrb_define_method(mrb, VECTOR_CLASS, "x",               vector3_x_getter,        MRB_ARGS_NONE());
  mrb_define_method(mrb, VECTOR_CLASS, "y",               vector3_y_getter,        MRB_ARGS_NONE());
  mrb_define_method(mrb, VECTOR_CLASS, "z",               vector3_z_getter,        MRB_ARGS_NONE());
  mrb_define_method(mrb, VECTOR_CLASS, "x=",              vector3_x_setter,        MRB_ARGS_REQ(1));
  mrb_define_method(mrb, VECTOR_CLASS, "y=",              vector3_y_setter,        MRB_ARGS_REQ(1));
  mrb_define_method(mrb, VECTOR_CLASS, "z=",              vector3_z_setter,        MRB_ARGS_REQ(1));
  mrb_define_method(mrb, VECTOR_CLASS, "set",             vector3_set,             MRB_ARGS_ANY());
  /* arithmetic */
  mrb_define_method(mrb, VECTOR_CLASS, "-@",              vector3_negate,          MRB_ARGS_NONE());
  mrb_define_method(mrb, VECTOR_CLASS, "+@",              vector3_identity,        MRB_ARGS_NONE());
  mrb_define_method(mrb, VECTOR_CLASS, "+",               vector3_add,             MRB_ARGS_REQ(1));
  mrb_define_method(mrb, VECTOR_CLASS, "-",               vector3_sub,             MRB_ARGS_REQ(1));
  mrb_define_method(mrb, VECTOR_CLASS, "*",               vector3_mul,             MRB_ARGS_REQ(1));
  mrb_define_method(mrb, VECTOR_CLASS, "/",               vector3_div,             MRB_ARGS_REQ(1));
  mrb_define_method(mrb, VECTOR_CLASS, "dot",             vector3_dot,             MRB_ARGS_REQ(1));
  mrb_define_method(mrb, VECTOR_CLASS, "cross",           vector3_cross,           MRB_ARGS_REQ(1));
  /* */
  mrb_define_method(mrb, VECTOR_CLASS, "normalize",       vector3_normalize,       MRB_ARGS_NONE());
  mrb_define_method(mrb, VECTOR_CLASS, "length",          vector3_length,          MRB_ARGS_NONE());
  mrb_define_method(mrb, VECTOR_CLASS, "distance",        vector3_distance,        MRB_ARGS_REQ(1));
  mrb_define_method(mrb, VECTOR_CLASS, "rotate",          vector3_rotate,          MRB_ARGS_REQ(2));
  mrb_define_method(mrb, VECTOR_CLASS, "rotate_x",        vector3_rotate_x,        MRB_ARGS_REQ(1));
  mrb_define_method(mrb, VECTOR_CLASS, "rotate_y",        vector3_rotate_y,        MRB_ARGS_REQ(1));
  mrb_define_method(mrb, VECTOR_CLASS, "rotate_z",        vector3_rotate_z,        MRB_ARGS_REQ(1));
  mrb_define_method(mrb, VECTOR_CLASS, "lerp",            vector3_lerp,            MRB_ARGS_REQ(2));
  mrb_define_method(mrb, VECTOR_CLASS, "slerp",           vector3_slerp,           MRB_ARGS_REQ(2));
  /* bitwise operators */
  /*mrb_define_method(mrb, VECTOR_CLASS, "~@",              vector3_not,             MRB_ARGS_NONE());*/
  /*mrb_define_method(mrb, VECTOR_CLASS, "%",               vector3_modulo,          MRB_ARGS_REQ(1));*/
  /*mrb_define_method(mrb, VECTOR_CLASS, "<<",              vector3_shl,             MRB_ARGS_REQ(1));*/
  /*mrb_define_method(mrb, VECTOR_CLASS, ">>",              vector3_shr,             MRB_ARGS_REQ(1));*/
  /*mrb_define_method(mrb, VECTOR_CLASS, "&",               vector3_and,             MRB_ARGS_REQ(1));*/
  /*mrb_define_method(mrb, VECTOR_CLASS, "|",               vector3_or,              MRB_ARGS_REQ(1));*/
  /*mrb_define_method(mrb, VECTOR_CLASS, "^",               vector3_xor,             MRB_ARGS_REQ(1));*/
  /* conversion */
  mrb_define_method(mrb, VECTOR_CLASS, "to_a",            vector3_to_a,            MRB_ARGS_NONE());
  /* cast */
  mrb_define_class_method(mrb, VECTOR_CLASS, "[]",        vector3_s_cast,          MRB_ARGS_ANY());
  mrb_define_class_method(mrb, VECTOR_CLASS, "extract",   vector3_s_extract,       MRB_ARGS_REQ(1));

  mrb_alias_method(mrb, VECTOR_CLASS, mrb_intern_cstr(mrb, "r"), mrb_intern_cstr(mrb, "x"));
  mrb_alias_method(mrb, VECTOR_CLASS, mrb_intern_cstr(mrb, "g"), mrb_intern_cstr(mrb, "y"));
  mrb_alias_method(mrb, VECTOR_CLASS, mrb_intern_cstr(mrb, "b"), mrb_intern_cstr(mrb, "z"));

  mrb_alias_method(mrb, VECTOR_CLASS, mrb_intern_cstr(mrb, "r="), mrb_intern_cstr(mrb, "x="));
  mrb_alias_method(mrb, VECTOR_CLASS, mrb_intern_cstr(mrb, "g="), mrb_intern_cstr(mrb, "y="));
  mrb_alias_method(mrb, VECTOR_CLASS, mrb_intern_cstr(mrb, "b="), mrb_intern_cstr(mrb, "z="));
  return VECTOR_CLASS;
}
