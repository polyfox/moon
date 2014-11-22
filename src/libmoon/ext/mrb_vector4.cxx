/*
 * Moon Vector4, a wrapper around glm::vec4
 */
#include "moon/mrb/vector4.hxx"
#include "moon/mrb/vector3.hxx"
#include "moon/mrb/vector2.hxx"
#include <glm/glm.hpp>
#include "moon/mrb/shared_types.hxx"

#define VECTOR_DATA_TYPE &vector4_data_type
#define VECTOR_MOON_STRUCT moon_vec4
#define VECTOR_STRUCT glm::vec4
#define VECTOR_EXTRACT_FUNC(_mrb_) mmrb_vector4_extract_mrb_args(_mrb_)
#define moon_vector_ptr moon_vec4_ptr
#define moon_vector_ref moon_vec4_ref
#include "moon/mrb/vector_private.hxx"

static struct RClass *vector4_class = NULL;

static void
vector4_free(mrb_state *mrb, void *p)
{
  moon_vec4 *vec4 = (moon_vec4*)p;
  if (vec4) {
    delete(vec4);
  }
}

struct mrb_data_type vector4_data_type = { "Vector4", vector4_free };

static inline void
mrb_set_vector4_value_xyzw(mrb_state *mrb, mrb_value target, mrb_float x, mrb_float y, mrb_float z, mrb_float w)
{
  moon_vec4 *vect = new moon_vec4(new glm::vec4(x, y, z, w));
  mrb_data_init(target, vect, &vector4_data_type);
}

/*
 * Black Magic
 */
glm::vec4
mmrb_vector4_extract_mrb_vec4(mrb_state *mrb, mrb_value obj)
{
  m_vector_unwrap(obj, vec);
  return moon_vector_ref(vec);
}

static glm::vec4
mmrb_vector4_extract_mrb_array(mrb_state *mrb, mrb_value obj)
{
  glm::vec4 result;
  int _ary_len = mrb_ary_len(mrb, obj);
  if (_ary_len != 4) {
    mrb_raisef(mrb, E_ARGUMENT_ERROR,
               "wrong array size %d (expected 4)", _ary_len);
  } else {
    result.x = mrb_to_flo(mrb, RARRAY_PTR(obj)[0]);
    result.y = mrb_to_flo(mrb, RARRAY_PTR(obj)[1]);
    result.z = mrb_to_flo(mrb, RARRAY_PTR(obj)[2]);
    result.w = mrb_to_flo(mrb, RARRAY_PTR(obj)[3]);
  }
  return result;
}

static glm::vec4
mmrb_vector4_extract_mrb_num(mrb_state *mrb, mrb_value obj)
{
  double i = mrb_to_flo(mrb, obj);
  return glm::vec4(i, i, i, i);
}

static glm::vec4
mmrb_vector4_extract_mrb_to_vec4(mrb_state *mrb, mrb_value obj)
{
  return mmrb_vector4_extract_mrb_vec4(mrb, mrb_funcall(mrb, obj, "to_vec4", 0));
}

glm::vec4
mmrb_vector4_extract_args(mrb_state *mrb, int argc, mrb_value *vals)
{
  glm::vec4 result;

  switch (argc) {
  case 1:
    mrb_value val;
    val = vals[0];
    switch (mrb_type(val)) {
    case MRB_TT_FIXNUM:
    case MRB_TT_FLOAT:
      result = mmrb_vector4_extract_mrb_num(mrb, val);
      break;
    case MRB_TT_ARRAY:
      result = mmrb_vector4_extract_mrb_array(mrb, val);
      break;
    case MRB_TT_DATA:
      if (DATA_TYPE(val) == &vector4_data_type) {
        result = mmrb_vector4_extract_mrb_vec4(mrb, val);
        break;
      }
    default:
      if (mrb_respond_to(mrb, val, mrb_intern_cstr(mrb, "to_vec4"))) {
        result = mmrb_vector4_extract_mrb_to_vec4(mrb, val);
        break;
      } else {
        mrb_raisef(mrb, E_TYPE_ERROR,
                   "wrong type %S (expected Numeric, Array or Vector4)",
                   mrb_str_new_cstr(mrb, mrb_obj_classname(mrb, val)));
      }
    }
    break;
  case 2:
  case 3:
    int index;
    index = 0;
    for (int i=0; i < argc; ++i) {
      mrb_value val = vals[i];

      if (mrb_type(val) == MRB_TT_DATA) {
        if (DATA_TYPE(val) == &vector2_data_type) {
          moon_vec2* vec2;
          vec2 = (moon_vec2*)mrb_data_get_ptr(mrb, val, &vector2_data_type);

          result[index++] = (**vec2)[0]; if (index >= 4) break;
          result[index++] = (**vec2)[1];
        } else if (DATA_TYPE(val) == &vector3_data_type) {
          moon_vec3* vec3;
          vec3 = (moon_vec3*)mrb_data_get_ptr(mrb, val, &vector3_data_type);

          result[index++] = (**vec3)[0]; if (index >= 4) break;
          result[index++] = (**vec3)[1]; if (index >= 4) break;
          result[index++] = (**vec3)[2];
        } else {
          mrb_raisef(mrb, E_TYPE_ERROR,
                     "wrong type %S (expected Vector2 or Vector3)",
                     mrb_str_new_cstr(mrb, mrb_obj_classname(mrb, val)));
        }
      } else if ((mrb_type(val) == MRB_TT_FIXNUM) || (mrb_type(val) == MRB_TT_FLOAT)) {
        result[index++] = mrb_to_flo(mrb, val);
      }
      if (index >= 4) break;
    };
    if (index < 4) {
      mrb_raisef(mrb, E_ARGUMENT_ERROR, "not enough parameters (required 4)");
    }
    break;
  case 4:
    result.x = mrb_to_flo(mrb, vals[0]);
    result.y = mrb_to_flo(mrb, vals[1]);
    result.z = mrb_to_flo(mrb, vals[2]);
    result.w = mrb_to_flo(mrb, vals[3]);
    break;
  default:
    mrb_raisef(mrb, E_ARGUMENT_ERROR,
               //"wrong number of arguments (%d for 1 or 4)", argc);
               "wrong number of arguments (%d for 1, 2, 3, or 4)", argc);
  }

  return result;
}

static glm::vec4
mmrb_vector4_extract_mrb_args(mrb_state *mrb)
{
  mrb_value *vals;
  int len;

  mrb_get_args(mrb, "*", &vals, &len);
  return mmrb_vector4_extract_args(mrb, len, vals);
}

static mrb_value
vector4_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_float x = 0.0;
  mrb_float y = 0.0;
  mrb_float z = 0.0;
  mrb_float w = 0.0;
  moon_vec4 *vec4;
  mrb_get_args(mrb, "|ffff", &x, &y, &z, &w);
  vec4 = (moon_vec4*)DATA_PTR(self);
  if (vec4) {
    vector4_free(mrb, (void*)vec4);
  }
  mrb_set_vector4_value_xyzw(mrb, self, x, y, z, w);
  return self;
}

static mrb_value
vector4_initialize_copy(mrb_state *mrb, mrb_value self)
{
  moon_vec4* src_vec;
  mrb_get_args(mrb, "d", &src_vec, &vector4_data_type);
  mrb_set_vector4_value_xyzw(mrb, self, moon_vector_ptr(src_vec)->x,
                                        moon_vector_ptr(src_vec)->y,
                                        moon_vector_ptr(src_vec)->z,
                                        moon_vector_ptr(src_vec)->w);
  return self;
}

static mrb_value
vector4_coerce(mrb_state *mrb, mrb_value self)
{
  mrb_value other;
  mrb_get_args(mrb, "o", &other);
  mrb_value argv[2] = { self, other };
  return mrb_ary_new_from_values(mrb, 2, argv);
}

static mrb_value
vector4_x_getter(mrb_state *mrb, mrb_value self)
{
  m_vector_unwrap_self(vec);
  return mrb_float_value(mrb, moon_vector_ptr(vec)->x);
}

static mrb_value
vector4_y_getter(mrb_state *mrb, mrb_value self)
{
  m_vector_unwrap_self(vec);
  return mrb_float_value(mrb, moon_vector_ptr(vec)->y);
}

static mrb_value
vector4_z_getter(mrb_state *mrb, mrb_value self)
{
  m_vector_unwrap_self(vec);
  return mrb_float_value(mrb, moon_vector_ptr(vec)->z);
}

static mrb_value
vector4_w_getter(mrb_state *mrb, mrb_value self)
{
  m_vector_unwrap_self(vec);
  return mrb_float_value(mrb, moon_vector_ptr(vec)->w);
}

static mrb_value
vector4_x_setter(mrb_state *mrb, mrb_value self)
{
  mrb_float x;
  m_vector_unwrap_self(vec);
  mrb_get_args(mrb, "f", &x);
  moon_vector_ptr(vec)->x = x;
  return mrb_nil_value();
}

static mrb_value
vector4_y_setter(mrb_state *mrb, mrb_value self)
{
  mrb_float y;
  m_vector_unwrap_self(vec);
  mrb_get_args(mrb, "f", &y);
  moon_vector_ptr(vec)->y = y;
  return mrb_nil_value();
}

static mrb_value
vector4_z_setter(mrb_state *mrb, mrb_value self)
{
  mrb_float z;
  m_vector_unwrap_self(vec);
  mrb_get_args(mrb, "f", &z);
  moon_vector_ptr(vec)->z = z;
  return mrb_nil_value();
}

static mrb_value
vector4_w_setter(mrb_state *mrb, mrb_value self)
{
  mrb_float w;
  m_vector_unwrap_self(vec);
  mrb_get_args(mrb, "f", &w);
  moon_vector_ptr(vec)->w = w;
  return mrb_nil_value();
}

static mrb_value
vector4_negate(mrb_state *mrb, mrb_value self)
{
  m_vector_mutate_copy(-(moon_vector_ref(result_vec)));
}

static mrb_value
vector4_identity(mrb_state *mrb, mrb_value self)
{
  return mrb_obj_dup(mrb, self);
}

static mrb_value
vector4_normalize(mrb_state *mrb, mrb_value self)
{
  m_vector_mutate_copy(glm::normalize(moon_vector_ref(result_vec)));
}

static mrb_value
vector4_length(mrb_state *mrb, mrb_value self)
{
  m_vector_unwrap_self(vec);
  return mrb_float_value(mrb, glm::length(moon_vector_ref(vec)));
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

//static mrb_value //vector4_mod(mrb_state *mrb, mrb_value self) {
//  m_vector_operator(%);
//};

static mrb_value
vector4_dot(mrb_state *mrb, mrb_value self)
{
  m_vector_operator_head(src_mvec, arg_vec);
  return mrb_float_value(mrb, glm::dot(moon_vector_ref(src_mvec), arg_vec));
}

static mrb_value
vector4_distance(mrb_state *mrb, mrb_value self)
{
  m_vector_operator_head(src_mvec, arg_vec);
  glm::vec4 diff = moon_vector_ref(src_mvec) - arg_vec;
  return mrb_float_value(mrb, glm::dot(diff, diff));
}

static mrb_value
vector4_set(mrb_state *mrb, mrb_value self)
{
  m_vector_unwrap_self(mvec);
  m_vector_extract_to(moon_vector_ref(mvec));
  return self;
}

static mrb_value
vector4_to_a(mrb_state *mrb, mrb_value self)
{
  moon_vec4* mvec4;
  mvec4 = (moon_vec4*)mrb_data_get_ptr(mrb, self, &vector4_data_type);
  mrb_value argv[4] = { mrb_float_value(mrb, moon_vec4_ptr(mvec4)->x),
                        mrb_float_value(mrb, moon_vec4_ptr(mvec4)->y),
                        mrb_float_value(mrb, moon_vec4_ptr(mvec4)->z),
                        mrb_float_value(mrb, moon_vec4_ptr(mvec4)->w) };
  return mrb_ary_new_from_values(mrb, 4, argv);
}

static mrb_value
vector4_s_extract(mrb_state *mrb, mrb_value self)
{
  glm::vec4 v4a = mmrb_vector4_extract_mrb_args(mrb);
  mrb_value argv[4] = { mrb_float_value(mrb, v4a.x),
                        mrb_float_value(mrb, v4a.y),
                        mrb_float_value(mrb, v4a.z),
                        mrb_float_value(mrb, v4a.w) };
  return mrb_ary_new_from_values(mrb, 4, argv);
}

static mrb_value
vector4_s_cast(mrb_state *mrb, mrb_value klass)
{
  mrb_value result = mrb_obj_new(mrb, vector4_class, 0, {});
  m_vector_unwrap(result, dest_vect);
  m_vector_extract(src_vect);
  moon_vector_ref(dest_vect) = src_vect;
  return result;
}

struct RClass*
mmrb_vector4_init(mrb_state *mrb)
{
  vector4_class = mrb_define_class_under(mrb, mmrb_Moon, "Vector4", mrb->object_class);
  MRB_SET_INSTANCE_TT(vector4_class, MRB_TT_DATA);

  mrb_define_method(mrb, vector4_class, "initialize",      vector4_initialize,      MRB_ARGS_OPT(4));
  mrb_define_method(mrb, vector4_class, "initialize_copy", vector4_initialize_copy, MRB_ARGS_REQ(1));
  /* coercion */
  mrb_define_method(mrb, vector4_class, "coerce",          vector4_coerce,          MRB_ARGS_REQ(1));
  /* attribute setters */
  mrb_define_method(mrb, vector4_class, "x",               vector4_x_getter,        MRB_ARGS_NONE());
  mrb_define_method(mrb, vector4_class, "y",               vector4_y_getter,        MRB_ARGS_NONE());
  mrb_define_method(mrb, vector4_class, "z",               vector4_z_getter,        MRB_ARGS_NONE());
  mrb_define_method(mrb, vector4_class, "w",               vector4_w_getter,        MRB_ARGS_NONE());
  mrb_define_method(mrb, vector4_class, "x=",              vector4_x_setter,        MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vector4_class, "y=",              vector4_y_setter,        MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vector4_class, "z=",              vector4_z_setter,        MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vector4_class, "w=",              vector4_w_setter,        MRB_ARGS_REQ(1));
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
  //mrb_define_method(mrb, vector4_class, "%",               vector4_mod,             MRB_ARGS_REQ(1));
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
  return vector4_class;
}

mrb_value
mmrb_vector4_wrap(mrb_state *mrb, glm::vec4 *ptr)
{
  moon_vec4 *vec4_ptr = new moon_vec4(ptr);
  mrb_value rvec4 = mrb_obj_value(Data_Wrap_Struct(mrb, vector4_class, &vector4_data_type, vec4_ptr));
  return rvec4;
}

mrb_value
mmrb_vector4_wrap(mrb_state *mrb, moon_vec4 moonv)
{
  moon_vec4 *vec4_ptr = new moon_vec4(moonv);
  mrb_value rvec4 = mrb_obj_value(Data_Wrap_Struct(mrb, vector4_class, &vector4_data_type, vec4_ptr));
  return rvec4;
}

mrb_value
mmrb_vector4_create(mrb_state *mrb, double x, double y, double z, double w)
{
  return mmrb_vector4_wrap(mrb, new glm::vec4(x, y, z, w));
}
