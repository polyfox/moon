/*
 * Moon Vector4, a wrapper around glm::vec4
 */
#include "mrb.hxx"
#include <memory>
#include <glm/glm.hpp>

namespace Moon {

  static struct RClass *vector4_class = NULL;

  typedef std::shared_ptr<glm::vec4> moon_vec4;

  struct float4a {
    mrb_float x;
    mrb_float y;
    mrb_float z;
    mrb_float w;
  };

  #define mrb_set_vector4_value_xyz(mrb, target, x, y, z, w)   \
    {                                                          \
      auto vector4 = new moon_vec4(new glm::vec4(x, y, z, w)); \
      DATA_TYPE(target) = &vector4_data_type;                  \
      DATA_PTR(target) = vector4;                              \
    }

  #define vec4_math_head(_func_)                                            \
    moon_vec4* src_vec4;                                                    \
    Data_Get_Struct(mrb, self, &vector4_data_type, src_vec4);               \
    float4a other_v4a = moon_vector4_extract_args(mrb);                     \
    glm::vec4 oth_vec4(other_v4a.x, other_v4a.y, other_v4a.z, other_v4a.w); \
    _func_

  #define vec4_math_op(_op_)                                           \
    vec4_math_head({                                                   \
      /*mrb_value dest_vec = mrb_obj_new(mrb, vector4_class, 0, {});*/ \
      mrb_value dest_vec = mrb_obj_dup(mrb, self);                     \
      **((moon_vec4*)DATA_PTR(dest_vec)) = **src_vec4 _op_ oth_vec4;   \
      return dest_vec;                                                 \
    })

  static void moon_mrb_vector4_deallocate(mrb_state *mrb, void *p) {
    delete((moon_vec4*)p);
  };

  const struct mrb_data_type vector4_data_type = {
    "Vector4", moon_mrb_vector4_deallocate,
  };

  /*
   * Black Magic
   */
  static float4a
  moon_vector4_extract_args(mrb_state *mrb) {
    mrb_value *vals;
    int len;

    float4a result = { 0.0f, 0.0f, 0.0f, 0.0f };

    mrb_get_args(mrb, "*", &vals, &len);

    switch (len) {
      case 1:
        mrb_value val;
        mrb_get_args(mrb, "o", &val);
        if (mrb_type(val) == MRB_TT_FIXNUM) {
          mrb_int i;
          mrb_get_args(mrb, "i", &i);
          result.x = (mrb_float)i;
          result.y = (mrb_float)i;
          result.z = (mrb_float)i;
          result.w = (mrb_float)i;
        } else if (mrb_type(val) == MRB_TT_FLOAT) {
          mrb_float f;
          mrb_get_args(mrb, "f", &f);
          result.x = (mrb_float)f;
          result.y = (mrb_float)f;
          result.z = (mrb_float)f;
          result.w = (mrb_float)f;
        } else if (mrb_type(val) == MRB_TT_ARRAY) {
          int _ary_len = mrb_ary_len(mrb, val);
          if (_ary_len != 4) {
            mrb_raisef(mrb, E_ARGUMENT_ERROR,
                       "wrong array size %d (expected 4)", _ary_len);
          } else {
            result.x = mrb_to_flo(mrb, RARRAY_PTR(val)[0]);
            result.y = mrb_to_flo(mrb, RARRAY_PTR(val)[1]);
            result.z = mrb_to_flo(mrb, RARRAY_PTR(val)[2]);
            result.w = mrb_to_flo(mrb, RARRAY_PTR(val)[3]);
          }
        } else if (mrb_type(val) == MRB_TT_DATA) {
          moon_vec4* vec4;
          Data_Get_Struct(mrb, val, &vector4_data_type, vec4);
          result.x = (*vec4)->x;
          result.y = (*vec4)->y;
          result.z = (*vec4)->z;
          result.w = (*vec4)->w;
        } else {
          mrb_raisef(mrb, E_TYPE_ERROR,
                     "wrong type %S (expected Numeric, Array or Vector4)",
                     mrb_obj_classname(mrb, val));
        }
        break;
      case 4:
        mrb_float x;
        mrb_float y;
        mrb_float z;
        mrb_float w;
        mrb_get_args(mrb, "ffff", &x, &y, &z, &w);
        result.x = x;
        result.y = y;
        result.z = z;
        result.w = w;
        break;
      default:
        mrb_raisef(mrb, E_ARGUMENT_ERROR,
                   "wrong number of arguments (%d for 1..4)", len);
    }

    return result;
  }

  static mrb_value
  moon_mrb_vector4_initialize(mrb_state *mrb, mrb_value self) {
    mrb_float x, y, z, w;
    mrb_get_args(mrb, "ffff", &x, &y, &z, &w);

    mrb_set_vector4_value_xyz(mrb, self, x, y, z, w)

    return mrb_nil_value();
  };

  static mrb_value
  moon_mrb_vector4_initialize_copy(mrb_state *mrb, mrb_value self) {
    mrb_value other;
    moon_vec4* src_vector4;

    mrb_get_args(mrb, "o", &other);

    Data_Get_Struct(mrb, other, &vector4_data_type, src_vector4);

    auto vector4 = new moon_vec4(new glm::vec4((*src_vector4)->r, (*src_vector4)->g, (*src_vector4)->b, (*src_vector4)->a));
    DATA_TYPE(self) = &vector4_data_type;
    DATA_PTR(self) = vector4;

    return mrb_nil_value();
  }

  static mrb_value
  moon_mrb_vector4_coerce(mrb_state *mrb, mrb_value self) {
    mrb_value other;
    mrb_get_args(mrb, "o", &other);
    mrb_value argv[2] = { self, other };
    return mrb_ary_new_from_values(mrb, 2, argv);
  }

  static mrb_value
  moon_mrb_vector4_x_setter(mrb_state *mrb, mrb_value self) {
    mrb_float x;
    mrb_get_args(mrb, "f", &x);

    moon_vec4* vector4;
    Data_Get_Struct(mrb, self, &vector4_data_type, vector4);

    (*vector4)->x = x;

    return mrb_nil_value();
  }

  static mrb_value
  moon_mrb_vector4_x_getter(mrb_state *mrb, mrb_value self) {
    moon_vec4* vector4;
    Data_Get_Struct(mrb, self, &vector4_data_type, vector4);

    return mrb_float_value(mrb, (*vector4)->x);
  }

  static mrb_value
  moon_mrb_vector4_y_setter(mrb_state *mrb, mrb_value self) {
    mrb_float y;
    mrb_get_args(mrb, "f", &y);

    moon_vec4* vector4;
    Data_Get_Struct(mrb, self, &vector4_data_type, vector4);

    (*vector4)->y = y;

    return mrb_nil_value();
  }

  static mrb_value
  moon_mrb_vector4_y_getter(mrb_state *mrb, mrb_value self) {
    moon_vec4* vector4;
    Data_Get_Struct(mrb, self, &vector4_data_type, vector4);

    return mrb_float_value(mrb, (*vector4)->y);
  }

  static mrb_value
  moon_mrb_vector4_z_setter(mrb_state *mrb, mrb_value self) {
    mrb_float z;
    mrb_get_args(mrb, "f", &z);

    moon_vec4* vector4;
    Data_Get_Struct(mrb, self, &vector4_data_type, vector4);

    (*vector4)->z = z;

    return mrb_nil_value();
  }

  static mrb_value
  moon_mrb_vector4_z_getter(mrb_state *mrb, mrb_value self) {
    moon_vec4* vector4;
    Data_Get_Struct(mrb, self, &vector4_data_type, vector4);

    return mrb_float_value(mrb, (*vector4)->z);
  }

  static mrb_value
  moon_mrb_vector4_w_setter(mrb_state *mrb, mrb_value self) {
    mrb_float w;
    mrb_get_args(mrb, "f", &w);

    moon_vec4* vector4;
    Data_Get_Struct(mrb, self, &vector4_data_type, vector4);

    (*vector4)->w = w;

    return mrb_float_value(mrb, (*vector4)->w);
  }

  static mrb_value
  moon_mrb_vector4_w_getter(mrb_state *mrb, mrb_value self) {
    moon_vec4* vector4;
    Data_Get_Struct(mrb, self, &vector4_data_type, vector4);

    return mrb_float_value(mrb, (*vector4)->w);
  }

  static mrb_value
  moon_mrb_vector4_negate(mrb_state *mrb, mrb_value self) {
    moon_vec4* dvec4;
    moon_vec4* svec4;

    //mrb_value dest_vec = mrb_obj_new(mrb, vector4_class, 0, {});
    mrb_value dest_vec = mrb_obj_dup(mrb, self);

    Data_Get_Struct(mrb, dest_vec, &vector4_data_type, dvec4);
    Data_Get_Struct(mrb, self, &vector4_data_type, svec4);

    (*dvec4)->x = -(*svec4)->x;
    (*dvec4)->y = -(*svec4)->y;
    (*dvec4)->z = -(*svec4)->z;
    (*dvec4)->w = -(*svec4)->w;

    return dest_vec;
  }

  static mrb_value
  moon_mrb_vector4_identity(mrb_state *mrb, mrb_value self) {
    return mrb_obj_dup(mrb, self);
  }

  static mrb_value
  moon_mrb_vector4_add(mrb_state *mrb, mrb_value self) {
    vec4_math_op(+)
  };

  static mrb_value
  moon_mrb_vector4_sub(mrb_state *mrb, mrb_value self) {
    vec4_math_op(-)
  };

  static mrb_value
  moon_mrb_vector4_mul(mrb_state *mrb, mrb_value self) {
    vec4_math_op(*)
  };

  static mrb_value
  moon_mrb_vector4_div(mrb_state *mrb, mrb_value self) {
    vec4_math_op(/)
  };

  static mrb_value
  moon_mrb_vector4_dot(mrb_state *mrb, mrb_value self) {
    vec4_math_head({
      return mrb_float_value(mrb, glm::dot(**src_vec4, oth_vec4));
    })
  };

  //static mrb_value
  //moon_mrb_vector4_cross(mrb_state *mrb, mrb_value self) {
  //  vec4_math_head({
  //    //mrb_value dest_vec = mrb_obj_new(mrb, vector4_class, 0, {});
  //    mrb_value dest_vec = mrb_obj_dup(mrb, self);
  //    **((moon_vec4*)DATA_PTR(dest_vec)) = glm::cross(**src_vec4, oth_vec4);
  //    return dest_vec;
  //  })
  //};

  static mrb_value
  moon_mrb_vector4_set(mrb_state *mrb, mrb_value self) {
    float4a v4a = moon_vector4_extract_args(mrb);

    moon_vec4* mvec4;
    Data_Get_Struct(mrb, self, &vector4_data_type, mvec4);

    (*mvec4)->x = v4a.x;
    (*mvec4)->y = v4a.y;
    (*mvec4)->z = v4a.z;
    (*mvec4)->w = v4a.w;

    return self;
  };

  static mrb_value
  moon_mrb_vector4_to_a(mrb_state *mrb, mrb_value self) {
    moon_vec4* mvec4;
    Data_Get_Struct(mrb, self, &vector4_data_type, mvec4);
    mrb_value argv[4] = { mrb_float_value(mrb, (*mvec4)->x),
                          mrb_float_value(mrb, (*mvec4)->y),
                          mrb_float_value(mrb, (*mvec4)->z),
                          mrb_float_value(mrb, (*mvec4)->w) };
    return mrb_ary_new_from_values(mrb, 4, argv);
  };

  static mrb_value
  moon_mrb_vector4_s_extract(mrb_state *mrb, mrb_value self) {
    float4a v4a = moon_vector4_extract_args(mrb);

    mrb_value argv[4] = { mrb_float_value(mrb, v4a.x),
                          mrb_float_value(mrb, v4a.y),
                          mrb_float_value(mrb, v4a.z),
                          mrb_float_value(mrb, v4a.w) };
    return mrb_ary_new_from_values(mrb, 4, argv);
  }

  static mrb_value
  moon_mrb_vector4_s_cast(mrb_state *mrb, mrb_value klass) {
    float4a v4a = moon_vector4_extract_args(mrb);

    mrb_value dest_vec = mrb_obj_new(mrb, vector4_class, 0, {});

    moon_vec4* _vec4;
    Data_Get_Struct(mrb, dest_vec, &vector4_data_type, _vec4);

    (*_vec4)->x = v4a.x;
    (*_vec4)->y = v4a.y;
    (*_vec4)->z = v4a.z;
    (*_vec4)->w = v4a.w;

    return dest_vec;
  }

  struct RClass*
  moon_mrb_vector4_init(mrb_state *mrb) {
    vector4_class = mrb_define_class_under(mrb, moon_module, "Vector4", mrb->object_class);
    MRB_SET_INSTANCE_TT(vector4_class, MRB_TT_DATA);

    mrb_define_method(mrb, vector4_class, "initialize",      moon_mrb_vector4_initialize,      MRB_ARGS_REQ(4));
    mrb_define_method(mrb, vector4_class, "initialize_copy", moon_mrb_vector4_initialize_copy, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, vector4_class, "coerce",          moon_mrb_vector4_coerce,          MRB_ARGS_REQ(1));
    mrb_define_method(mrb, vector4_class, "x=",              moon_mrb_vector4_x_setter,        MRB_ARGS_REQ(1));
    mrb_define_method(mrb, vector4_class, "x",               moon_mrb_vector4_x_getter,        MRB_ARGS_NONE());
    mrb_define_method(mrb, vector4_class, "y=",              moon_mrb_vector4_y_setter,        MRB_ARGS_REQ(1));
    mrb_define_method(mrb, vector4_class, "y",               moon_mrb_vector4_y_getter,        MRB_ARGS_NONE());
    mrb_define_method(mrb, vector4_class, "z=",              moon_mrb_vector4_z_setter,        MRB_ARGS_REQ(1));
    mrb_define_method(mrb, vector4_class, "z",               moon_mrb_vector4_z_getter,        MRB_ARGS_NONE());
    mrb_define_method(mrb, vector4_class, "w=",              moon_mrb_vector4_w_setter,        MRB_ARGS_REQ(1));
    mrb_define_method(mrb, vector4_class, "w",               moon_mrb_vector4_w_getter,        MRB_ARGS_NONE());

    mrb_define_method(mrb, vector4_class, "-@",              moon_mrb_vector4_negate,          MRB_ARGS_NONE());
    mrb_define_method(mrb, vector4_class, "+@",              moon_mrb_vector4_identity,        MRB_ARGS_NONE());
    mrb_define_method(mrb, vector4_class, "+",               moon_mrb_vector4_add,             MRB_ARGS_REQ(1));
    mrb_define_method(mrb, vector4_class, "-",               moon_mrb_vector4_sub,             MRB_ARGS_REQ(1));
    mrb_define_method(mrb, vector4_class, "*",               moon_mrb_vector4_mul,             MRB_ARGS_REQ(1));
    mrb_define_method(mrb, vector4_class, "/",               moon_mrb_vector4_div,             MRB_ARGS_REQ(1));
    mrb_define_method(mrb, vector4_class, "dot",             moon_mrb_vector4_dot,             MRB_ARGS_REQ(1));
    mrb_define_method(mrb, vector4_class, "set",             moon_mrb_vector4_set,             MRB_ARGS_ANY());
    //mrb_define_method(mrb, vector4_class, "cross",           moon_mrb_vector4_cross,           MRB_ARGS_REQ(1));
    mrb_define_method(mrb, vector4_class, "to_a",            moon_mrb_vector4_to_a,            MRB_ARGS_NONE());

    mrb_define_class_method(mrb, vector4_class, "[]",        moon_mrb_vector4_s_cast,          MRB_ARGS_ANY());
    mrb_define_class_method(mrb, vector4_class, "extract",   moon_mrb_vector4_s_extract,       MRB_ARGS_REQ(1));

    mrb_alias_method(mrb, vector4_class, mrb_intern_cstr(mrb, "r"), mrb_intern_cstr(mrb, "x"));
    mrb_alias_method(mrb, vector4_class, mrb_intern_cstr(mrb, "g"), mrb_intern_cstr(mrb, "y"));
    mrb_alias_method(mrb, vector4_class, mrb_intern_cstr(mrb, "b"), mrb_intern_cstr(mrb, "z"));
    mrb_alias_method(mrb, vector4_class, mrb_intern_cstr(mrb, "a"), mrb_intern_cstr(mrb, "w"));

    mrb_alias_method(mrb, vector4_class, mrb_intern_cstr(mrb, "r="), mrb_intern_cstr(mrb, "x="));
    mrb_alias_method(mrb, vector4_class, mrb_intern_cstr(mrb, "g="), mrb_intern_cstr(mrb, "y="));
    mrb_alias_method(mrb, vector4_class, mrb_intern_cstr(mrb, "b="), mrb_intern_cstr(mrb, "z="));
    mrb_alias_method(mrb, vector4_class, mrb_intern_cstr(mrb, "a="), mrb_intern_cstr(mrb, "w="));

    return vector4_class;
  };

}