/*
 * Moon Vector1, a wrapper around glm::vec1
 */
#include <mruby.h>
#include <mruby/array.h>
#include <mruby/class.h>
#include <mruby/data.h>
#include <mruby/numeric.h>
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtx/compatibility.hpp>
#include "moon/mrb/vector1.hxx"

static struct RClass *vector1_class = NULL;

static void
vector1_free(mrb_state *mrb, void *p)
{
  moon_vec1 *vec = (moon_vec1*)p;
  if (vec) {
    delete(vec);
  }
}

const struct mrb_data_type vector1_data_type = { "Vector1", vector1_free };

static inline void
mrb_set_vector1_value_x(mrb_state *mrb, mrb_value target, mrb_float x)
{
  moon_vec1 *vect = new moon_vec1(new glm::highp_vec1_t(x));
  mrb_data_init(target, vect, &vector1_data_type);
}

mrb_value
mmrb_vector1_value(mrb_state *mrb, glm::highp_vec1_t vec)
{
  moon_vec1 *vec_ptr = new moon_vec1(new glm::highp_vec1_t(vec));
  return mrb_obj_value(Data_Wrap_Struct(mrb, vector1_class, &vector1_data_type, vec_ptr));
}

mrb_value
mmrb_vector1_wrap(mrb_state *mrb, glm::highp_vec1_t *ptr)
{
  moon_vec1 *vec_ptr = new moon_vec1(ptr);
  return mrb_obj_value(Data_Wrap_Struct(mrb, vector1_class, &vector1_data_type, vec_ptr));
}

mrb_value
mmrb_vector1_wrap(mrb_state *mrb, moon_vec1 moonv)
{
  moon_vec1 *vec_ptr = new moon_vec1(moonv);
  return mrb_obj_value(Data_Wrap_Struct(mrb, vector1_class, &vector1_data_type, vec_ptr));
}

mrb_value
mmrb_vector1_create(mrb_state *mrb, double x)
{
  return mmrb_vector1_wrap(mrb, new glm::highp_vec1_t(x));
}

/*
 * Black Magic
 */
glm::highp_vec1_t
mmrb_vector1_extract_mrb_vec1(mrb_state *mrb, mrb_value obj)
{
  m_vector_unwrap(obj, vec);
  return moon_vector_ref(vec);
}

glm::highp_vec1_t
mmrb_to_vector1(mrb_state *mrb, mrb_value obj)
{
  return mmrb_vector1_extract_mrb_vec1(mrb, obj);
}

static glm::highp_vec1_t
mmrb_vector1_extract_mrb_num(mrb_state *mrb, mrb_value obj)
{
  double i = mrb_to_flo(mrb, obj);
  return glm::highp_vec1_t(i);
}

static glm::highp_vec1_t
mmrb_vector1_extract_mrb_array(mrb_state *mrb, mrb_value obj)
{
  glm::highp_vec1_t result;
  int _ary_len = mrb_ary_len(mrb, obj);
  if (_ary_len != 1) {
    mrb_raisef(mrb, E_ARGUMENT_ERROR,
               "wrong array size %d (expected 1)", _ary_len);
  } else {
    result.x = mrb_to_flo(mrb, RARRAY_PTR(obj)[0]);
  }
  return result;
}

static glm::highp_vec1_t
mmrb_vector1_extract_mrb_to_vec1(mrb_state *mrb, mrb_value obj)
{
  return mmrb_vector1_extract_mrb_vec1(mrb, mrb_funcall(mrb, obj, "to_vec1", 0));
}

glm::highp_vec1_t
mmrb_vector1_extract_args(mrb_state *mrb, int argc, mrb_value *vals)
{
  glm::highp_vec1_t result;
  switch (argc) {
  case 1:
    mrb_value val;
    val = vals[0];
    switch (mrb_type(val)) {
    case MRB_TT_FIXNUM:
    case MRB_TT_FLOAT:
      result = mmrb_vector1_extract_mrb_num(mrb, val);
      break;
    case MRB_TT_ARRAY:
      result = mmrb_vector1_extract_mrb_array(mrb, val);
      break;
    case MRB_TT_DATA:
      if (DATA_TYPE(val) == &vector1_data_type) {
        result = mmrb_vector1_extract_mrb_vec1(mrb, val);
        break;
      }
    default:
      if (mrb_respond_to(mrb, val, mrb_intern_cstr(mrb, "to_vec1"))) {
        result = mmrb_vector1_extract_mrb_to_vec1(mrb, val);
        break;
      } else {
        mrb_raisef(mrb, E_TYPE_ERROR,
                   "wrong type %S (expected Numeric, Array or Vector1)",
                   mrb_str_new_cstr(mrb, mrb_obj_classname(mrb, val)));
      }
    }
    break;
  default:
    mrb_raisef(mrb, E_ARGUMENT_ERROR,
               "wrong number of arguments (%d for 1)", argc);
  }
  return result;
}

static glm::highp_vec1_t
mmrb_vector1_extract_mrb_args(mrb_state *mrb)
{
  mrb_value *vals;
  int len;
  mrb_get_args(mrb, "*", &vals, &len);
  return mmrb_vector1_extract_args(mrb, len, vals);
}

static mrb_value
vector1_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_float x = 0.0;
  moon_vec1 *vec;
  mrb_get_args(mrb, "|f", &x);
  vec = (moon_vec1*)DATA_PTR(self);
  if (vec) {
    vector1_free(mrb, (void*)vec);
  }
  mrb_set_vector1_value_x(mrb, self, x);
  return self;
}

static mrb_value
vector1_initialize_copy(mrb_state *mrb, mrb_value self)
{
  moon_vec1* src_vec;
  mrb_get_args(mrb, "d", &src_vec, &vector1_data_type);
  mrb_set_vector1_value_x(mrb, self, moon_vector_ptr(src_vec)->x);
  return self;
}

static mrb_value
vector1_coerce(mrb_state *mrb, mrb_value self)
{
  mrb_value other;
  mrb_get_args(mrb, "o", &other);
  mrb_value argv[2] = { self, other };
  return mrb_ary_new_from_values(mrb, 2, argv);
}

static mrb_value
vector1_x_getter(mrb_state *mrb, mrb_value self)
{
  m_vector_unwrap_self(vec);
  return mrb_float_value(mrb, moon_vector_ptr(vec)->x);
}

static mrb_value
vector1_x_setter(mrb_state *mrb, mrb_value self)
{
  mrb_float x;
  m_vector_unwrap_self(vec);
  mrb_get_args(mrb, "f", &x);
  moon_vector_ptr(vec)->x = x;
  return mrb_nil_value();
}

static mrb_value
vector1_negate(mrb_state *mrb, mrb_value self)
{
  m_vector_mutate_copy(-(moon_vector_ref(result_vec)));
}

static mrb_value
vector1_identity(mrb_state *mrb, mrb_value self)
{
  return mrb_obj_dup(mrb, self);
}

static mrb_value
vector1_add(mrb_state *mrb, mrb_value self)
{
  m_vector_operator(+);
}

static mrb_value
vector1_sub(mrb_state *mrb, mrb_value self)
{
  m_vector_operator(-);
}

static mrb_value
vector1_mul(mrb_state *mrb, mrb_value self)
{
  m_vector_operator(*);
}

static mrb_value
vector1_div(mrb_state *mrb, mrb_value self)
{
  m_vector_operator(/);
}

/* // Uncomment this when integer vectors are supported
static mrb_value
vector1_not(mrb_state *mrb, mrb_value self)
{
  m_vector_mutate_copy(~(moon_vector_ref(result_vec)));
}

static mrb_value
vector1_modulo(mrb_state *mrb, mrb_value self)
{
  m_vector_operator(%);
}

static mrb_value
vector1_shl(mrb_state *mrb, mrb_value self)
{
  m_vector_operator(<<);
}

static mrb_value
vector1_shr(mrb_state *mrb, mrb_value self)
{
  m_vector_operator(>>);
}

static mrb_value
vector1_and(mrb_state *mrb, mrb_value self)
{
  m_vector_operator(&);
}

static mrb_value
vector1_or(mrb_state *mrb, mrb_value self)
{
  m_vector_operator(|);
}

static mrb_value
vector1_xor(mrb_state *mrb, mrb_value self)
{
  m_vector_operator(^);
}
*/

static mrb_value
vector1_dot(mrb_state *mrb, mrb_value self)
{
  m_vector_operator_head(src_mvec, arg_vec);
  return mrb_float_value(mrb, glm::dot(moon_vector_ref(src_mvec), arg_vec));
}

static mrb_value
vector1_normalize(mrb_state *mrb, mrb_value self)
{
  m_vector_mutate_copy(glm::normalize(moon_vector_ref(result_vec)));
}

static mrb_value
vector1_length(mrb_state *mrb, mrb_value self)
{
  m_vector_unwrap_self(vec);
  return mrb_float_value(mrb, glm::length(moon_vector_ref(vec)));
}

static mrb_value
vector1_distance(mrb_state *mrb, mrb_value self)
{
  m_vector_operator_head(src_mvec, arg_vec);
  glm::highp_vec1_t diff = moon_vector_ref(src_mvec) - arg_vec;
  return mrb_float_value(mrb, glm::dot(diff, diff));
}

static mrb_value
vector1_lerp(mrb_state *mrb, mrb_value self)
{
  moon_vec1 *other;
  mrb_float delta;
  mrb_get_args(mrb, "df", &other, &vector1_data_type, &delta);
  m_vector_unwrap_self(vec);
  //return mmrb_vector1_value(mrb, glm::lerp(moon_vector_ref(vec), moon_vector_ref(other), (float)delta));
  return mrb_nil_value();
}

static mrb_value
vector1_set(mrb_state *mrb, mrb_value self)
{
  m_vector_unwrap_self(mvec);
  m_vector_extract_to(moon_vector_ref(mvec));
  return self;
}

static mrb_value
vector1_to_a(mrb_state *mrb, mrb_value self)
{
  m_vector_unwrap_self(mvec1);
  mrb_value argv[1] = { mrb_float_value(mrb, moon_vector_ptr(mvec1)->x) };
  return mrb_ary_new_from_values(mrb, 1, argv);
}

static mrb_value
vector1_s_extract(mrb_state *mrb, mrb_value self)
{
  m_vector_extract(src_vect);
  mrb_value argv[1] = { mrb_float_value(mrb, src_vect.x) };
  return mrb_ary_new_from_values(mrb, 1, argv);
}

static mrb_value
vector1_s_cast(mrb_state *mrb, mrb_value klass)
{
  mrb_value result = mrb_obj_new(mrb, vector1_class, 0, {});
  m_vector_unwrap(result, dest_vect);
  m_vector_extract(src_vect);
  moon_vector_ref(dest_vect) = src_vect;
  return result;
}

void
mmrb_vector1_init(mrb_state *mrb, struct RClass *mod)
{
  vector1_class = mrb_define_class_under(mrb, mod, "Vector1", mrb->object_class);
  MRB_SET_INSTANCE_TT(vector1_class, MRB_TT_DATA);

  mrb_define_method(mrb, vector1_class, "initialize",      vector1_initialize,      MRB_ARGS_OPT(1));
  mrb_define_method(mrb, vector1_class, "initialize_copy", vector1_initialize_copy, MRB_ARGS_REQ(1));
  /* coercion */
  mrb_define_method(mrb, vector1_class, "coerce",          vector1_coerce,          MRB_ARGS_REQ(1));
  /* attribute setters */
  mrb_define_method(mrb, vector1_class, "x",               vector1_x_getter,        MRB_ARGS_NONE());
  mrb_define_method(mrb, vector1_class, "x=",              vector1_x_setter,        MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vector1_class, "set",             vector1_set,             MRB_ARGS_ANY());
  /* arithmetic */
  mrb_define_method(mrb, vector1_class, "-@",              vector1_negate,          MRB_ARGS_NONE());
  mrb_define_method(mrb, vector1_class, "+@",              vector1_identity,        MRB_ARGS_NONE());
  mrb_define_method(mrb, vector1_class, "+",               vector1_add,             MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vector1_class, "-",               vector1_sub,             MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vector1_class, "*",               vector1_mul,             MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vector1_class, "/",               vector1_div,             MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vector1_class, "dot",             vector1_dot,             MRB_ARGS_REQ(1));
  /*mrb_define_method(mrb, vector1_class, "cross",           vector1_cross,           MRB_ARGS_REQ(1));*/
  /* */
  mrb_define_method(mrb, vector1_class, "normalize",       vector1_normalize,       MRB_ARGS_NONE());
  mrb_define_method(mrb, vector1_class, "length",          vector1_length,          MRB_ARGS_NONE());
  mrb_define_method(mrb, vector1_class, "distance",        vector1_distance,        MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vector1_class, "lerp",            vector1_lerp,            MRB_ARGS_REQ(2));
  /* bitwise operators */ /* If and only if we ever support integer based vectors */
  /*mrb_define_method(mrb, vector1_class, "~@",              vector1_not,             MRB_ARGS_NONE());*/
  /*mrb_define_method(mrb, vector1_class, "%",               vector1_modulo,          MRB_ARGS_REQ(1));*/
  /*mrb_define_method(mrb, vector1_class, "<<",              vector1_shl,             MRB_ARGS_REQ(1));*/
  /*mrb_define_method(mrb, vector1_class, ">>",              vector1_shr,             MRB_ARGS_REQ(1));*/
  /*mrb_define_method(mrb, vector1_class, "&",               vector1_and,             MRB_ARGS_REQ(1));*/
  /*mrb_define_method(mrb, vector1_class, "|",               vector1_or,              MRB_ARGS_REQ(1));*/
  /*mrb_define_method(mrb, vector1_class, "^",               vector1_xor,             MRB_ARGS_REQ(1));*/
  /* conversion */
  mrb_define_method(mrb, vector1_class, "to_a",            vector1_to_a,            MRB_ARGS_NONE());
  /* cast */
  mrb_define_class_method(mrb, vector1_class, "[]",        vector1_s_cast,          MRB_ARGS_ANY());
  mrb_define_class_method(mrb, vector1_class, "extract",   vector1_s_extract,       MRB_ARGS_REQ(1));
}
