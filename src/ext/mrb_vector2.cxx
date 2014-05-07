/*
 * Moon Vector2, a wrapper around glm::vec2
 */
#include "mrb.hxx"
#include <memory>
#include <glm/glm.hpp>
#include "mrb_shared_types.hxx"

namespace Moon {

  static struct RClass *vector2_class = NULL;

  #define mrb_set_vector2_value_xy(mrb, target, x, y)    \
    {                                                    \
      moon_vec2 *vector2 = new moon_vec2(new glm::vec2(x, y)); \
      DATA_TYPE(target) = &vector2_data_type;            \
      DATA_PTR(target) = vector2;                        \
    }

  #define vec2_math_head(_func_)                               \
    moon_vec2* src_vec2;                                       \
    Data_Get_Struct(mrb, self, &vector2_data_type, src_vec2);  \
    glm::vec2 oth_vec2 = moon_vector2_extract_mrb_args(mrb);       \
    _func_

  #define vec2_math_op(_op_)                                   \
    glm::vec2 oth_vec2 = moon_vector2_extract_mrb_args(mrb);       \
    mrb_value dest_vec = mrb_obj_dup(mrb, self);               \
    **((moon_vec2*)DATA_PTR(dest_vec)) _op_ ## = oth_vec2;     \
    return dest_vec;

  static void moon_mrb_vector2_deallocate(mrb_state *mrb, void *p) {
    delete((moon_vec2*)p);
  };

  const struct mrb_data_type vector2_data_type = {
    "Vector2", moon_mrb_vector2_deallocate,
  };

  /*
   * Black Magic
   */
  glm::vec2
  moon_vector2_extract_args(mrb_state *mrb, int argc, mrb_value *vals) {
    glm::vec2 result;

    switch (argc) {
      case 1:
        mrb_value val;
        val = vals[0];
        if ((mrb_type(val) == MRB_TT_FIXNUM) || (mrb_type(val) == MRB_TT_FLOAT)) {
          double i = mrb_to_flo(mrb, val);
          result.x = i;
          result.y = i;
        } else if (mrb_type(val) == MRB_TT_ARRAY) {
          int _ary_len = mrb_ary_len(mrb, val);
          if (_ary_len != 2) {
            mrb_raisef(mrb, E_ARGUMENT_ERROR,
                       "wrong array size %d (expected 2)", _ary_len);
          } else {
            result.x = mrb_to_flo(mrb, RARRAY_PTR(val)[0]);
            result.y = mrb_to_flo(mrb, RARRAY_PTR(val)[1]);
          }
        } else if (mrb_type(val) == MRB_TT_DATA) {
          moon_vec2* _vec2;
          Data_Get_Struct(mrb, val, &vector2_data_type, _vec2);
          result = **_vec2;
        } else {
          mrb_raisef(mrb, E_TYPE_ERROR,
                     "wrong type %S (expected Numeric, Array or Vector2)",
                     mrb_obj_classname(mrb, val));
        }
        break;
      case 2:
        result.x = mrb_to_flo(mrb, vals[0]);
        result.y = mrb_to_flo(mrb, vals[1]);
        break;
      default:
        mrb_raisef(mrb, E_ARGUMENT_ERROR,
                   "wrong number of arguments (%d for 1..2)", argc);
    }

    return result;
  }

  static glm::vec2
  moon_vector2_extract_mrb_args(mrb_state *mrb) {
    mrb_value *vals;
    int len;

    mrb_get_args(mrb, "*", &vals, &len);
    return moon_vector2_extract_args(mrb, len, vals);
  }

  static mrb_value
  moon_mrb_vector2_initialize(mrb_state *mrb, mrb_value self) {
    mrb_float x = 0.0;
    mrb_float y = 0.0;
    mrb_get_args(mrb, "|ff", &x, &y);

    mrb_set_vector2_value_xy(mrb, self, x, y);

    return mrb_nil_value();
  };

  static mrb_value
  moon_mrb_vector2_initialize_copy(mrb_state *mrb, mrb_value self) {
    mrb_value other;
    moon_vec2* src_vec2;

    mrb_get_args(mrb, "o", &other);

    Data_Get_Struct(mrb, other, &vector2_data_type, src_vec2);

    mrb_set_vector2_value_xy(mrb, self, (*src_vec2)->x, (*src_vec2)->y);

    return self;
  }

  static mrb_value
  moon_mrb_vector2_coerce(mrb_state *mrb, mrb_value self) {
    mrb_value other;
    mrb_get_args(mrb, "o", &other);
    mrb_value argv[2] = { self, other };
    return mrb_ary_new_from_values(mrb, 2, argv);
  }

  static mrb_value
  moon_mrb_vector2_x_getter(mrb_state *mrb, mrb_value self) {
    moon_vec2* vec2;
    Data_Get_Struct(mrb, self, &vector2_data_type, vec2);

    return mrb_float_value(mrb, (*vec2)->x);
  };

  static mrb_value
  moon_mrb_vector2_y_getter(mrb_state *mrb, mrb_value self) {
    moon_vec2* vec2;
    Data_Get_Struct(mrb, self, &vector2_data_type, vec2);

    return mrb_float_value(mrb, (*vec2)->y);
  };

  static mrb_value
  moon_mrb_vector2_x_setter(mrb_state *mrb, mrb_value self) {
    moon_vec2* vec2;
    Data_Get_Struct(mrb, self, &vector2_data_type, vec2);

    mrb_float x;
    mrb_get_args(mrb, "f", &x);

    (*vec2)->x = x;

    return mrb_nil_value();
  };

  static mrb_value
  moon_mrb_vector2_y_setter(mrb_state *mrb, mrb_value self) {
    moon_vec2* vec2;
    Data_Get_Struct(mrb, self, &vector2_data_type, vec2);

    mrb_float y;
    mrb_get_args(mrb, "f", &y);

    (*vec2)->y = y;

    return mrb_nil_value();
  };

  static mrb_value
  moon_mrb_vector2_negate(mrb_state *mrb, mrb_value self) {
    moon_vec2* dvec2;
    moon_vec2* svec2;

    //mrb_value dest_vec = mrb_obj_new(mrb, vector2_class, 0, {});
    mrb_value dest_vec = mrb_obj_dup(mrb, self);

    Data_Get_Struct(mrb, dest_vec, &vector2_data_type, dvec2);
    Data_Get_Struct(mrb, self, &vector2_data_type, svec2);

    (**dvec2) = -(**svec2);

    return dest_vec;
  }

  static mrb_value
  moon_mrb_vector2_identity(mrb_state *mrb, mrb_value self) {
    return mrb_obj_dup(mrb, self);
  }

  static mrb_value
  moon_mrb_vector2_add(mrb_state *mrb, mrb_value self) {
    vec2_math_op(+)
  };

  static mrb_value
  moon_mrb_vector2_sub(mrb_state *mrb, mrb_value self) {
    vec2_math_op(-)
  };

  static mrb_value
  moon_mrb_vector2_mul(mrb_state *mrb, mrb_value self) {
    vec2_math_op(*)
  };

  static mrb_value
  moon_mrb_vector2_div(mrb_state *mrb, mrb_value self) {
    vec2_math_op(/)
  };

  //static mrb_value
  //moon_mrb_vector2_mod(mrb_state *mrb, mrb_value self) {
  //  vec2_math_op(%)
  //};

  static mrb_value
  moon_mrb_vector2_dot(mrb_state *mrb, mrb_value self) {
    vec2_math_head({
      return mrb_float_value(mrb, glm::dot(**src_vec2, oth_vec2));
    })
  };

  //static mrb_value
  //moon_mrb_vector2_cross(mrb_state *mrb, mrb_value self) {
  //  vec2_math_head({
  //    //mrb_value dest_vec = mrb_obj_new(mrb, vector2_class, 0, {});
  //    mrb_value dest_vec = mrb_obj_dup(mrb, self);
  //    **((moon_vec2*)DATA_PTR(dest_vec)) = glm::cross(**src_vec2, oth_vec2);
  //    return dest_vec;
  //  })
  //};

  static mrb_value
  moon_mrb_vector2_set(mrb_state *mrb, mrb_value self) {
    glm::vec2 v2a = moon_vector2_extract_mrb_args(mrb);

    moon_vec2* mvec2;
    Data_Get_Struct(mrb, self, &vector2_data_type, mvec2);

    (*mvec2)->x = v2a.x;
    (*mvec2)->y = v2a.y;

    return self;
  };

  static mrb_value
  moon_mrb_vector2_to_a(mrb_state *mrb, mrb_value self) {
    moon_vec2* mvec2;
    Data_Get_Struct(mrb, self, &vector2_data_type, mvec2);
    mrb_value argv[2] = { mrb_float_value(mrb, (*mvec2)->x),
                          mrb_float_value(mrb, (*mvec2)->y) };
    return mrb_ary_new_from_values(mrb, 2, argv);
  };

  static mrb_value
  moon_mrb_vector2_s_extract(mrb_state *mrb, mrb_value self) {
    glm::vec2 v2a = moon_vector2_extract_mrb_args(mrb);

    mrb_value argv[2] = { mrb_float_value(mrb, v2a.x),
                          mrb_float_value(mrb, v2a.y) };
    return mrb_ary_new_from_values(mrb, 2, argv);
  }

  static mrb_value
  moon_mrb_vector2_s_cast(mrb_state *mrb, mrb_value klass) {
    glm::vec2 v2a = moon_vector2_extract_mrb_args(mrb);

    mrb_value dest_vec = mrb_obj_new(mrb, vector2_class, 0, {});

    moon_vec2* _vec2;
    Data_Get_Struct(mrb, dest_vec, &vector2_data_type, _vec2);

    (*_vec2)->x = v2a.x;
    (*_vec2)->y = v2a.y;
    return dest_vec;
  }

  struct RClass*
  moon_mrb_vector2_init(mrb_state *mrb) {
    vector2_class = mrb_define_class_under(mrb, moon_module, "Vector2", mrb->object_class);
    MRB_SET_INSTANCE_TT(vector2_class, MRB_TT_DATA);

    mrb_define_method(mrb, vector2_class, "initialize",      moon_mrb_vector2_initialize,      MRB_ARGS_OPT(2));
    mrb_define_method(mrb, vector2_class, "initialize_copy", moon_mrb_vector2_initialize_copy, MRB_ARGS_REQ(1));

    mrb_define_method(mrb, vector2_class, "coerce",          moon_mrb_vector2_coerce,          MRB_ARGS_REQ(1));

    mrb_define_method(mrb, vector2_class, "x",               moon_mrb_vector2_x_getter,        MRB_ARGS_NONE());
    mrb_define_method(mrb, vector2_class, "y",               moon_mrb_vector2_y_getter,        MRB_ARGS_NONE());
    mrb_define_method(mrb, vector2_class, "x=",              moon_mrb_vector2_x_setter,        MRB_ARGS_REQ(1));
    mrb_define_method(mrb, vector2_class, "y=",              moon_mrb_vector2_y_setter,        MRB_ARGS_REQ(1));

    mrb_define_method(mrb, vector2_class, "-@",              moon_mrb_vector2_negate,          MRB_ARGS_NONE());
    mrb_define_method(mrb, vector2_class, "+@",              moon_mrb_vector2_identity,        MRB_ARGS_NONE());

    mrb_define_method(mrb, vector2_class, "+",               moon_mrb_vector2_add,             MRB_ARGS_REQ(1));
    mrb_define_method(mrb, vector2_class, "-",               moon_mrb_vector2_sub,             MRB_ARGS_REQ(1));
    mrb_define_method(mrb, vector2_class, "*",               moon_mrb_vector2_mul,             MRB_ARGS_REQ(1));
    mrb_define_method(mrb, vector2_class, "/",               moon_mrb_vector2_div,             MRB_ARGS_REQ(1));
    //mrb_define_method(mrb, vector2_class, "%",               moon_mrb_vector2_mod,             MRB_ARGS_REQ(1));

    mrb_define_method(mrb, vector2_class, "dot",             moon_mrb_vector2_dot,             MRB_ARGS_REQ(1));
    //mrb_define_method(mrb, vector2_class, "cross",           moon_mrb_vector2_cross,           MRB_ARGS_REQ(1));

    mrb_define_method(mrb, vector2_class, "set",             moon_mrb_vector2_set,             MRB_ARGS_ANY());

    mrb_define_method(mrb, vector2_class, "to_a",            moon_mrb_vector2_to_a,            MRB_ARGS_NONE());

    mrb_define_class_method(mrb, vector2_class, "[]",        moon_mrb_vector2_s_cast,          MRB_ARGS_ANY());
    mrb_define_class_method(mrb, vector2_class, "extract",   moon_mrb_vector2_s_extract,       MRB_ARGS_REQ(1));

    return vector2_class;
  };
}