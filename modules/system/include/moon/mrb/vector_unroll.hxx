#ifndef MRB_MOON_VECTOR_UNROLL
#define MRB_MOON_VECTOR_UNROLL

#include <mruby.h>
#include <mruby/class.h>
#include <mruby/data.h>
#include <mruby/numeric.h>
#include "moon/mrb/vector1.hxx"
#include "moon/mrb/vector2.hxx"
#include "moon/mrb/vector3.hxx"
#include "moon/mrb/vector4.hxx"

/* Vector unrolling
 * @param [mrb_state*] mrb
 * @param [size_t] argc  number of values passing in
 * @param [mrb_value*] vals  list of values
 * @param [size_t] result_size  expected number of values after unrolling
 * @param [mrb_float] result  target buffer
 */
static inline int
mmrb_vector_unroll(mrb_state *mrb, size_t argc, mrb_value *vals, size_t result_size, mrb_float *result)
{
  if (argc < 1 || argc > result_size) {
    mrb_raisef(mrb, E_ARGUMENT_ERROR,
               "wrong number of arguments (%S for 1..%S)",
               mrb_fixnum_value(argc),
               mrb_fixnum_value(result_size));
  }

  size_t index = 0;
  for (size_t i = 0; i < argc; ++i) {
    mrb_value val = vals[i];
    switch (mrb_type(val)) {
      case MRB_TT_FIXNUM:
      case MRB_TT_FLOAT:
      {
        result[index++] = mrb_to_flo(mrb, val);
        break;
      }
      case MRB_TT_ARRAY:
      {
        size_t len = mrb_ary_len(mrb, val);
        for (size_t j = 0; j < len; ++j) {
          result[index++] = mmrb_to_flo(mrb, mrb_ary_entry(val, j));
          if (index >= 4) break;
        }
        break;
      }
      case MRB_TT_DATA:
      {
        const mrb_data_type *t = (mrb_data_type*)DATA_TYPE(val);
        if (t == &vector1_data_type) {
          result[index++] = mmrb_to_flo(mrb, val);
          break;
        } else if (t == &vector2_data_type) {
          Moon::Vector2 v2 = *mmrb_vector2_ptr(mrb, val);
          result[index++] = v2[0]; if (index >= result_size) break;
          result[index++] = v2[1];
          break;
        } else if (t == &vector3_data_type) {
          Moon::Vector3 v3 = *mmrb_vector3_ptr(mrb, val);
          result[index++] = v3[0]; if (index >= result_size) break;
          result[index++] = v3[1]; if (index >= result_size) break;
          result[index++] = v3[2];
          break;
        } else if (t == &vector4_data_type) {
          Moon::Vector4 v4 = *mmrb_vector4_ptr(mrb, val);
          result[index++] = v4[0]; if (index >= result_size) break;
          result[index++] = v4[1]; if (index >= result_size) break;
          result[index++] = v4[2]; if (index >= result_size) break;
          result[index++] = v4[3];
          break;
        } else {
          mrb_raisef(mrb, E_TYPE_ERROR,
                     "wrong type %S (expected a Vector*)",
                     mrb_str_new_cstr(mrb, mrb_obj_classname(mrb, val)));
        }
      }
      default: {
        mrb_raisef(mrb, E_TYPE_ERROR,
                   "wrong type %S (expected Array, Numeric, Vector1, Vector2, Vector3, or Vector4)",
                   mrb_str_new_cstr(mrb, mrb_obj_classname(mrb, val)));
      }
    }
    if (index >= result_size) break;
  };
  if (index == 1) {
    for (size_t i = 1; i < result_size; ++i) {
      result[i] = result[0];
    }
  } else if (index < result_size) {
    mrb_raisef(mrb, E_ARGUMENT_ERROR,
               "unrolled %S parameter(s) (required %S)",
               mrb_fixnum_value(index),
               mrb_fixnum_value(result_size));
  }
  return index;
}

#endif
