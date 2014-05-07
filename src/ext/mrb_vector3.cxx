/*
 * Moon Vector3, a wrapper around glm::vec3
 */
#include "mrb.hxx"
#include <memory>
#include <glm/glm.hpp>
#include "mrb_shared_types.hxx"

namespace Moon {

  static struct RClass *vector3_class = NULL;

  #define mrb_set_vector3_value_xyz(mrb, target, x, y, z)    \
    {                                                        \
      moon_vec3 *vector3 = new moon_vec3(new glm::vec3(x, y, z));  \
      DATA_TYPE(target) = &vector3_data_type;                \
      DATA_PTR(target) = vector3;                            \
    }

  #define vec3_math_head(_func_)                               \
    moon_vec3* src_vec3;                                       \
    Data_Get_Struct(mrb, self, &vector3_data_type, src_vec3);  \
    glm::vec3 oth_vec3 = moon_vector3_extract_mrb_args(mrb);       \
    _func_

  #define vec3_math_op(_op_)                                         \
    glm::vec3 oth_vec3 = moon_vector3_extract_mrb_args(mrb);             \
    mrb_value dest_vec = mrb_obj_dup(mrb, self);                     \
    **((moon_vec3*)DATA_PTR(dest_vec)) _op_ ## = oth_vec3;           \
    return dest_vec;

  static void moon_mrb_vector3_deallocate(mrb_state *mrb, void *p) {
    delete((moon_vec3*)p);
  };

  const struct mrb_data_type vector3_data_type = {
    "Vector3", moon_mrb_vector3_deallocate,
  };

  /*
   * Black Magic
   */
  glm::vec3
  moon_vector3_extract_args(mrb_state *mrb, int argc, mrb_value *vals) {
    glm::vec3 result;

    switch (argc) {
      case 1:
        mrb_value val;
        val = vals[0];
        if ((mrb_type(val) == MRB_TT_FIXNUM) || (mrb_type(val) == MRB_TT_FLOAT)) {
          double i = mrb_to_flo(mrb, val);
          result.x = i;
          result.y = i;
          result.z = i;
        } else if (mrb_type(val) == MRB_TT_ARRAY) {
          int _ary_len = mrb_ary_len(mrb, val);
          if (_ary_len != 3) {
            mrb_raisef(mrb, E_ARGUMENT_ERROR,
                       "wrong array size %d (expected 3)", _ary_len);
          } else {
            result.x = mrb_to_flo(mrb, RARRAY_PTR(val)[0]);
            result.y = mrb_to_flo(mrb, RARRAY_PTR(val)[1]);
            result.z = mrb_to_flo(mrb, RARRAY_PTR(val)[2]);
          }
        } else if (mrb_type(val) == MRB_TT_DATA) {
          moon_vec3* vec3;
          Data_Get_Struct(mrb, val, &vector3_data_type, vec3);
          result = **vec3;
        } else {
          mrb_raisef(mrb, E_TYPE_ERROR,
                     "wrong type %S (expected Numeric, Array or Vector3)",
                     mrb_obj_classname(mrb, val));
        }
        break;
      case 2:
        int index;
        index = 0;
        for (int i=0; i < 2; ++i) {
          mrb_value val = vals[i];

          if (mrb_type(val) == MRB_TT_DATA) {
            if (DATA_TYPE(val) == &vector2_data_type) {
              moon_vec2* vec2;
              Data_Get_Struct(mrb, val, &vector2_data_type, vec2);

              result[index++] = (**vec2)[0]; if (index >= 3) break;
              result[index++] = (**vec2)[1];
            } else {
              mrb_raisef(mrb, E_TYPE_ERROR,
                         "wrong type %S (expected Vector2)",
                         mrb_obj_classname(mrb, val));
            }
          } else if ((mrb_type(val) == MRB_TT_FIXNUM) || (mrb_type(val) == MRB_TT_FLOAT)) {
            result[index++] = mrb_to_flo(mrb, val);
          }
          if (index >= 3) break;
        };
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

  static glm::vec3
  moon_vector3_extract_mrb_args(mrb_state *mrb) {
    mrb_value *vals;
    int len;

    mrb_get_args(mrb, "*", &vals, &len);
    return moon_vector3_extract_args(mrb, len, vals);
  }

  static mrb_value
  moon_mrb_vector3_initialize(mrb_state *mrb, mrb_value self) {
    mrb_float x = 0.0;
    mrb_float y = 0.0;
    mrb_float z = 0.0;
    mrb_get_args(mrb, "|fff", &x, &y, &z);

    mrb_set_vector3_value_xyz(mrb, self, x, y, z);

    return mrb_nil_value();
  };

  static mrb_value
  moon_mrb_vector3_initialize_copy(mrb_state *mrb, mrb_value self) {
    mrb_value other;
    moon_vec3* src_vec3;

    mrb_get_args(mrb, "o", &other);

    Data_Get_Struct(mrb, other, &vector3_data_type, src_vec3);

    mrb_set_vector3_value_xyz(mrb, self,
                              (*src_vec3)->x,
                              (*src_vec3)->y,
                              (*src_vec3)->z);

    return self;
  }

  static mrb_value
  moon_mrb_vector3_coerce(mrb_state *mrb, mrb_value self) {
    mrb_value other;
    mrb_get_args(mrb, "o", &other);
    mrb_value argv[2] = { self, other };
    return mrb_ary_new_from_values(mrb, 2, argv);
  }

  static mrb_value
  moon_mrb_vector3_x_getter(mrb_state *mrb, mrb_value self) {
    moon_vec3* vec3;
    Data_Get_Struct(mrb, self, &vector3_data_type, vec3);

    return mrb_float_value(mrb, (*vec3)->x);
  };

  static mrb_value
  moon_mrb_vector3_y_getter(mrb_state *mrb, mrb_value self) {
    moon_vec3* vec3;
    Data_Get_Struct(mrb, self, &vector3_data_type, vec3);

    return mrb_float_value(mrb, (*vec3)->y);
  };

  static mrb_value
  moon_mrb_vector3_z_getter(mrb_state *mrb, mrb_value self) {
    moon_vec3* vec3;
    Data_Get_Struct(mrb, self, &vector3_data_type, vec3);

    return mrb_float_value(mrb, (*vec3)->z);
  };

  static mrb_value
  moon_mrb_vector3_x_setter(mrb_state *mrb, mrb_value self) {
    moon_vec3* vec3;
    Data_Get_Struct(mrb, self, &vector3_data_type, vec3);

    mrb_float x;
    mrb_get_args(mrb, "f", &x);

    (*vec3)->x = x;

    return mrb_nil_value();
  };

  static mrb_value
  moon_mrb_vector3_y_setter(mrb_state *mrb, mrb_value self) {
    moon_vec3* vec3;
    Data_Get_Struct(mrb, self, &vector3_data_type, vec3);

    mrb_float y;
    mrb_get_args(mrb, "f", &y);

    (*vec3)->y = y;

    return mrb_nil_value();
  };

  static mrb_value
  moon_mrb_vector3_z_setter(mrb_state *mrb, mrb_value self) {
    moon_vec3* vec3;
    Data_Get_Struct(mrb, self, &vector3_data_type, vec3);

    mrb_float z;
    mrb_get_args(mrb, "f", &z);

    (*vec3)->z = z;

    return mrb_nil_value();
  };

  static mrb_value
  moon_mrb_vector3_negate(mrb_state *mrb, mrb_value self) {
    moon_vec3* dvec3;
    moon_vec3* svec3;

    //mrb_value dest_vec = mrb_obj_new(mrb, vector3_class, 0, {});
    mrb_value dest_vec = mrb_obj_dup(mrb, self);

    Data_Get_Struct(mrb, dest_vec, &vector3_data_type, dvec3);
    Data_Get_Struct(mrb, self, &vector3_data_type, svec3);

    (**dvec3) = -(**svec3);

    return dest_vec;
  }

  static mrb_value
  moon_mrb_vector3_identity(mrb_state *mrb, mrb_value self) {
    return mrb_obj_dup(mrb, self);
  }

  static mrb_value
  moon_mrb_vector3_add(mrb_state *mrb, mrb_value self) {
    vec3_math_op(+)
  };

  static mrb_value
  moon_mrb_vector3_sub(mrb_state *mrb, mrb_value self) {
    vec3_math_op(-)
  };

  static mrb_value
  moon_mrb_vector3_mul(mrb_state *mrb, mrb_value self) {
    vec3_math_op(*)
  };

  static mrb_value
  moon_mrb_vector3_div(mrb_state *mrb, mrb_value self) {
    vec3_math_op(/)
  };

  //static mrb_value
  //moon_mrb_vector3_mod(mrb_state *mrb, mrb_value self) {
  //  vec3_math_op(%)
  //};

  static mrb_value
  moon_mrb_vector3_dot(mrb_state *mrb, mrb_value self) {
    vec3_math_head({
      return mrb_float_value(mrb, glm::dot(**src_vec3, oth_vec3));
    })
  };

  static mrb_value
  moon_mrb_vector3_cross(mrb_state *mrb, mrb_value self) {
    vec3_math_head({
      //mrb_value dest_vec = mrb_obj_new(mrb, vector3_class, 0, {});
      mrb_value dest_vec = mrb_obj_dup(mrb, self);
      **((moon_vec3*)DATA_PTR(dest_vec)) = glm::cross(**src_vec3, oth_vec3);
      return dest_vec;
    })
  };

  static mrb_value
  moon_mrb_vector3_set(mrb_state *mrb, mrb_value self) {
    glm::vec3 v3a = moon_vector3_extract_mrb_args(mrb);

    moon_vec3* mvec3;
    Data_Get_Struct(mrb, self, &vector3_data_type, mvec3);

    **mvec3 = v3a;

    return self;
  };

  static mrb_value
  moon_mrb_vector3_to_a(mrb_state *mrb, mrb_value self) {
    moon_vec3* mvec3;
    Data_Get_Struct(mrb, self, &vector3_data_type, mvec3);
    mrb_value argv[3] = { mrb_float_value(mrb, (*mvec3)->x),
                          mrb_float_value(mrb, (*mvec3)->y),
                          mrb_float_value(mrb, (*mvec3)->z) };
    return mrb_ary_new_from_values(mrb, 3, argv);
  };

  static mrb_value
  moon_mrb_vector3_s_extract(mrb_state *mrb, mrb_value self) {
    glm::vec3 v3a = moon_vector3_extract_mrb_args(mrb);

    mrb_value argv[3] = { mrb_float_value(mrb, v3a.x),
                          mrb_float_value(mrb, v3a.y),
                          mrb_float_value(mrb, v3a.z) };
    return mrb_ary_new_from_values(mrb, 3, argv);
  }

  static mrb_value
  moon_mrb_vector3_s_cast(mrb_state *mrb, mrb_value klass) {
    glm::vec3 v3a = moon_vector3_extract_mrb_args(mrb);

    mrb_value dest_vec = mrb_obj_new(mrb, vector3_class, 0, {});

    moon_vec3* _vec3;
    Data_Get_Struct(mrb, dest_vec, &vector3_data_type, _vec3);

    **_vec3 = v3a;

    return dest_vec;
  }

  struct RClass* moon_mrb_vector3_init(mrb_state *mrb) {
    vector3_class = mrb_define_class_under(mrb, moon_module, "Vector3", mrb->object_class);
    MRB_SET_INSTANCE_TT(vector3_class, MRB_TT_DATA);

    mrb_define_method(mrb, vector3_class, "initialize",      moon_mrb_vector3_initialize,      MRB_ARGS_OPT(3));
    mrb_define_method(mrb, vector3_class, "initialize_copy", moon_mrb_vector3_initialize_copy, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, vector3_class, "coerce",          moon_mrb_vector3_coerce,          MRB_ARGS_REQ(1));
    mrb_define_method(mrb, vector3_class, "x",               moon_mrb_vector3_x_getter,        MRB_ARGS_NONE());
    mrb_define_method(mrb, vector3_class, "y",               moon_mrb_vector3_y_getter,        MRB_ARGS_NONE());
    mrb_define_method(mrb, vector3_class, "z",               moon_mrb_vector3_z_getter,        MRB_ARGS_NONE());
    mrb_define_method(mrb, vector3_class, "x=",              moon_mrb_vector3_x_setter,        MRB_ARGS_REQ(1));
    mrb_define_method(mrb, vector3_class, "y=",              moon_mrb_vector3_y_setter,        MRB_ARGS_REQ(1));
    mrb_define_method(mrb, vector3_class, "z=",              moon_mrb_vector3_z_setter,        MRB_ARGS_REQ(1));

    mrb_define_method(mrb, vector3_class, "-@",              moon_mrb_vector3_negate,          MRB_ARGS_NONE());
    mrb_define_method(mrb, vector3_class, "+@",              moon_mrb_vector3_identity,        MRB_ARGS_NONE());
    mrb_define_method(mrb, vector3_class, "+",               moon_mrb_vector3_add,             MRB_ARGS_REQ(1));
    mrb_define_method(mrb, vector3_class, "-",               moon_mrb_vector3_sub,             MRB_ARGS_REQ(1));
    mrb_define_method(mrb, vector3_class, "*",               moon_mrb_vector3_mul,             MRB_ARGS_REQ(1));
    mrb_define_method(mrb, vector3_class, "/",               moon_mrb_vector3_div,             MRB_ARGS_REQ(1));
    //mrb_define_method(mrb, vector3_class, "%",               moon_mrb_vector3_mod,             MRB_ARGS_REQ(1));
    mrb_define_method(mrb, vector3_class, "dot",             moon_mrb_vector3_dot,             MRB_ARGS_REQ(1));
    mrb_define_method(mrb, vector3_class, "set",             moon_mrb_vector3_set,             MRB_ARGS_ANY());
    mrb_define_method(mrb, vector3_class, "cross",           moon_mrb_vector3_cross,           MRB_ARGS_REQ(1));
    mrb_define_method(mrb, vector3_class, "to_a",            moon_mrb_vector3_to_a,            MRB_ARGS_NONE());

    mrb_define_class_method(mrb, vector3_class, "[]",        moon_mrb_vector3_s_cast,          MRB_ARGS_ANY());
    mrb_define_class_method(mrb, vector3_class, "extract",   moon_mrb_vector3_s_extract,       MRB_ARGS_REQ(1));

    mrb_alias_method(mrb, vector3_class, mrb_intern_cstr(mrb, "r"), mrb_intern_cstr(mrb, "x"));
    mrb_alias_method(mrb, vector3_class, mrb_intern_cstr(mrb, "g"), mrb_intern_cstr(mrb, "y"));
    mrb_alias_method(mrb, vector3_class, mrb_intern_cstr(mrb, "b"), mrb_intern_cstr(mrb, "z"));

    mrb_alias_method(mrb, vector3_class, mrb_intern_cstr(mrb, "r="), mrb_intern_cstr(mrb, "x="));
    mrb_alias_method(mrb, vector3_class, mrb_intern_cstr(mrb, "g="), mrb_intern_cstr(mrb, "y="));
    mrb_alias_method(mrb, vector3_class, mrb_intern_cstr(mrb, "b="), mrb_intern_cstr(mrb, "z="));
    return vector3_class;
  };
}