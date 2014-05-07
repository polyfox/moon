/*
 * Moon Transform, a wrapper around glm::mat4
 */
#include "mrb.hxx"
#include <memory>
#include <glm/glm.hpp>
#include "shared_types.hxx"
#include "mrb_shared_types.hxx"

namespace Moon {
  namespace mrb_Transform {

#define def static mrb_value
#define math_op(__op__)                                                       \
  mrb_value rother;                                                           \
  mrb_get_args(mrb, "o", &rother);                                            \
  mrb_value rtarget = mrb_obj_dup(mrb, self);                                 \
  moon_mat4 *target_mat4;                                                     \
  Data_Get_Struct(mrb, rtarget, &data_type, target_mat4);                     \
  if (mrb_type(rother) == MRB_TT_DATA) {                                      \
    if (DATA_TYPE(rother) == &data_type) { /* Transform */                    \
      moon_mat4 *source_mat4;                                                 \
      Data_Get_Struct(mrb, rother, &data_type, source_mat4);                  \
      **target_mat4 __op__ ## = **source_mat4;                                \
    } else if (DATA_TYPE(rother) == &vector4_data_type) {                     \
      moon_vec4 *source_vec4;                                                 \
      Data_Get_Struct(mrb, rother, &vector4_data_type, source_vec4);          \
      **target_mat4 __op__ ## = **source_vec4;                                \
    }                                                                         \
  }                                                                           \
  return rtarget;

    static struct RClass *rclass = NULL;

    static void
    deallocate(mrb_state *mrb, void *p) {
      delete((moon_mat4*)p);
    };

    const struct mrb_data_type data_type = { "Transform", deallocate };

    def initialize(mrb_state *mrb, mrb_value self) {
      mrb_value *args;
      int argc;
      mrb_get_args(mrb, "*", &args, &argc);

      moon_mat4 *mat = NULL;

      if (argc == 0){
        mat = new moon_mat4(new glm::mat4());
      } else if (argc == 1) {
        if (mrb_type(args[0]) == MRB_TT_DATA) {
          if (DATA_TYPE(args[0]) == &data_type) { /* Transform */
            moon_mat4 *source_mat4;
            Data_Get_Struct(mrb, args[0], &data_type, source_mat4);
            mat = new moon_mat4(new glm::mat4(**source_mat4));
          } else {
            mrb_raisef(mrb, E_TYPE_ERROR,
                       "wrong argument type %S (expected Transform)",
                       mrb_obj_classname(mrb, args[0]));
            return mrb_nil_value();
          }
        } else {
          double value = mrb_to_flo(mrb, args[0]);
          mat = new moon_mat4(new glm::mat4(value));
        }
      } else if (argc == 4) {
        glm::vec4 row1 = moon_mrb_to_vec4(mrb, args[0]);
        glm::vec4 row2 = moon_mrb_to_vec4(mrb, args[1]);
        glm::vec4 row3 = moon_mrb_to_vec4(mrb, args[2]);
        glm::vec4 row4 = moon_mrb_to_vec4(mrb, args[3]);

        mat = new moon_mat4(new glm::mat4(row1, row2, row3, row4));
      } else if (argc == 16) {
        mat = new moon_mat4(new glm::mat4(
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
        ));
      } else {
        mrb_raisef(mrb, E_ARGUMENT_ERROR,
                   "wrong argument count %d (expected 0, 1, 4, or 16)",
                   argc);
      }

      DATA_TYPE(self) = &data_type;
      DATA_PTR(self) = mat;

      return mrb_nil_value();
    };

    def initialize_copy(mrb_state *mrb, mrb_value self) {
      mrb_value other;
      mrb_get_args(mrb, "o", &other);

      moon_mat4 *source_mat;
      Data_Get_Struct(mrb, other, &data_type, source_mat);

      auto mat = new moon_mat4(new glm::mat4(**source_mat));
      DATA_TYPE(self) = &data_type;
      DATA_PTR(self) = mat;

      return mrb_nil_value();
    };

    def coerce(mrb_state *mrb, mrb_value self) {
      mrb_value other;
      mrb_get_args(mrb, "o", &other);
      mrb_value argv[2] = { self, other };
      return mrb_ary_new_from_values(mrb, 2, argv);
    };

    def entry_get(mrb_state *mrb, mrb_value self) {
      mrb_value *vals;
      int len;
      mrb_get_args(mrb, "*", &vals, &len);

      moon_mat4 *mat4;
      Data_Get_Struct(mrb, self, &data_type, mat4);

      if (len == 1) {
        mrb_int x;

        mrb_get_args(mrb, "i", &x);

        if (x < 0 || x >= 4) {
          mrb_raisef(mrb, E_INDEX_ERROR,
                     "x %d is out of range (expected 0...4)", x);
          return mrb_nil_value();
        }

        mrb_value rvec4 = mrb_obj_new(mrb, moon_cVector4, 0, {});

        moon_vec4 *vec4;
        Data_Get_Struct(mrb, rvec4, &vector4_data_type, vec4);

        **vec4 = (**mat4)[x];

        return rvec4;
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

        return mrb_float_value(mrb, (**mat4)[x][y]);
      } else {
        mrb_raisef(mrb, E_ARGUMENT_ERROR,
                   "wrong number of arguments (%d for 1, or 2)", len);
      }
      return mrb_nil_value();
    }

    def entry_set(mrb_state *mrb, mrb_value self) {
      mrb_value *vals;
      int len;
      mrb_get_args(mrb, "*", &vals, &len);

      moon_mat4 *mat4;
      Data_Get_Struct(mrb, self, &data_type, mat4);

      if (len == 2) {
        mrb_int x;
        mrb_value rvec4;

        mrb_get_args(mrb, "io", &x, &rvec4);

        if (x < 0 || x >= 4) {
          mrb_raisef(mrb, E_INDEX_ERROR,
                     "x %d is out of range (expected 0...4)", x);
          return mrb_nil_value();
        }

        moon_vec4 *vec4;
        Data_Get_Struct(mrb, rvec4, &vector4_data_type, vec4);

        (**mat4)[x] = **vec4;
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

        (**mat4)[x][y] = (double)v;
      } else {
        mrb_raisef(mrb, E_ARGUMENT_ERROR,
                   "wrong number of arguments (%d for 2, or 3)", len);
      }
      return mrb_nil_value();
    }

    def op_add(mrb_state *mrb, mrb_value self) {
      math_op(+)
    }

    def op_sub(mrb_state *mrb, mrb_value self) {
      math_op(-)
    }

    def op_mul(mrb_state *mrb, mrb_value self) {
      math_op(*)
    }

    def op_div(mrb_state *mrb, mrb_value self) {
      math_op(/)
    }

    //def op_mod(mrb_state *mrb, mrb_value self) {
    //  math_op(%)
    //}

    def to_a16(mrb_state *mrb, mrb_value self) {
      moon_mat4 *mat4;
      Data_Get_Struct(mrb, self, &data_type, mat4);

      glm::vec4 row1 = (**mat4)[0];
      glm::vec4 row2 = (**mat4)[1];
      glm::vec4 row3 = (**mat4)[2];
      glm::vec4 row4 = (**mat4)[3];

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

    def to_a(mrb_state *mrb, mrb_value self) {
      moon_mat4 *mat4;
      Data_Get_Struct(mrb, self, &data_type, mat4);

      mrb_value rrow1 = mrb_obj_new(mrb, moon_cVector4, 0, {});
      moon_vec4 *row1;
      Data_Get_Struct(mrb, rrow1, &vector4_data_type, row1);
      **row1 = (**mat4)[0];

      mrb_value rrow2 = mrb_obj_new(mrb, moon_cVector4, 0, {});
      moon_vec4 *row2;
      Data_Get_Struct(mrb, rrow2, &vector4_data_type, row2);
      **row2 = (**mat4)[1];

      mrb_value rrow3 = mrb_obj_new(mrb, moon_cVector4, 0, {});
      moon_vec4 *row3;
      Data_Get_Struct(mrb, rrow3, &vector4_data_type, row3);
      **row3 = (**mat4)[2];

      mrb_value rrow4 = mrb_obj_new(mrb, moon_cVector4, 0, {});
      moon_vec4 *row4;
      Data_Get_Struct(mrb, rrow4, &vector4_data_type, row4);
      **row4 = (**mat4)[3];

      mrb_value argv[4] = { rrow1, rrow2, rrow3, rrow4 };

      return mrb_ary_new_from_values(mrb, 4, argv);
    }

    static struct RClass*
    Init(mrb_state *mrb) {
      rclass = mrb_define_class_under(mrb, moon_module, "Transform", mrb->object_class);
      MRB_SET_INSTANCE_TT(rclass, MRB_TT_DATA);

      mrb_define_method(mrb, rclass, "initialize",      initialize,      MRB_ARGS_ANY());
      mrb_define_method(mrb, rclass, "initialize_copy", initialize_copy, MRB_ARGS_REQ(1));

      mrb_define_method(mrb, rclass, "coerce",          coerce,          MRB_ARGS_REQ(1));

      mrb_define_method(mrb, rclass, "[]",              entry_get,       MRB_ARGS_ARG(1,1));
      mrb_define_method(mrb, rclass, "[]=",             entry_set,       MRB_ARGS_ARG(2,1));

      mrb_define_method(mrb, rclass, "+",               op_add,          MRB_ARGS_REQ(1));
      mrb_define_method(mrb, rclass, "-",               op_sub,          MRB_ARGS_REQ(1));
      mrb_define_method(mrb, rclass, "*",               op_mul,          MRB_ARGS_REQ(1));
      mrb_define_method(mrb, rclass, "/",               op_div,          MRB_ARGS_REQ(1));

      mrb_define_method(mrb, rclass, "to_a16",          to_a16,          MRB_ARGS_NONE());
      mrb_define_method(mrb, rclass, "to_a",            to_a,            MRB_ARGS_NONE());
      //mrb_define_method(mrb, rclass, "%",               op_mod,          MRB_ARGS_REQ(1));

      return rclass;
    };

  };

  struct RClass*
  moon_mrb_transform_init(mrb_state *mrb) {
    return mrb_Transform::Init(mrb);
  };

}