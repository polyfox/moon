/*
 * Moon Vector2, a wrapper around glm::vec2
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
#include "moon/mrb/vector2.hxx"

static struct RClass *vector2_class = NULL;

static void
vector2_free(mrb_state *mrb, void *p)
{
  glm::vec2 *vec2 = (glm::vec2*)p;
  if (vec2) {
    delete(vec2);
  }
}

const struct mrb_data_type vector2_data_type = { "Vector2", vector2_free };

static inline void
mrb_set_vector2_value_xy(mrb_state *mrb, mrb_value target, mrb_float x, mrb_float y)
{
  glm::vec2 *vect = new glm::vec2(x, y);
  mrb_data_init(target, vect, &vector2_data_type);
}

/*
 * Black Magic
 */
glm::vec2
mmrb_vector2_extract_mrb_vec2(mrb_state *mrb, mrb_value obj)
{
  m_vector_unwrap(obj, vec);
  return moon_vector_ref(vec);
}

glm::vec2
mmrb_to_vector2(mrb_state *mrb, mrb_value obj)
{
  return mmrb_vector2_extract_mrb_vec2(mrb, obj);
}

static glm::vec2
mmrb_vector2_extract_mrb_num(mrb_state *mrb, mrb_value obj)
{
  double i = mrb_to_flo(mrb, obj);
  return glm::vec2(i, i);
}

static glm::vec2
mmrb_vector2_extract_mrb_array(mrb_state *mrb, mrb_value obj)
{
  glm::vec2 result;
  int _ary_len = mrb_ary_len(mrb, obj);
  if (_ary_len != 2) {
    mrb_raisef(mrb, E_ARGUMENT_ERROR,
               "wrong array size %d (expected 2)", _ary_len);
  } else {
    result.x = mrb_to_flo(mrb, RARRAY_PTR(obj)[0]);
    result.y = mrb_to_flo(mrb, RARRAY_PTR(obj)[1]);
  }
  return result;
}

static glm::vec2
mmrb_vector2_extract_mrb_to_vec2(mrb_state *mrb, mrb_value obj)
{
  return mmrb_vector2_extract_mrb_vec2(mrb, mrb_funcall(mrb, obj, "to_vec2", 0));
}

glm::vec2
mmrb_vector2_extract_args(mrb_state *mrb, int argc, mrb_value *vals)
{
  glm::vec2 result;
  switch (argc) {
  case 1:
    mrb_value val;
    val = vals[0];
    switch (mrb_type(val)) {
    case MRB_TT_FIXNUM:
    case MRB_TT_FLOAT:
      result = mmrb_vector2_extract_mrb_num(mrb, val);
      break;
    case MRB_TT_ARRAY:
      result = mmrb_vector2_extract_mrb_array(mrb, val);
      break;
    case MRB_TT_DATA:
      if (DATA_TYPE(val) == &vector2_data_type) {
        result = mmrb_vector2_extract_mrb_vec2(mrb, val);
        break;
      }
    default:
      if (mrb_respond_to(mrb, val, mrb_intern_cstr(mrb, "to_vec2"))) {
        result = mmrb_vector2_extract_mrb_to_vec2(mrb, val);
        break;
      } else {
        mrb_raisef(mrb, E_TYPE_ERROR,
                   "wrong type %S (expected Numeric, Array or Vector2)",
                   mrb_str_new_cstr(mrb, mrb_obj_classname(mrb, val)));
      }
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
mmrb_vector2_extract_mrb_args(mrb_state *mrb)
{
  mrb_value *vals;
  int len;
  mrb_get_args(mrb, "*", &vals, &len);
  return mmrb_vector2_extract_args(mrb, len, vals);
}

static mrb_value
vector2_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_float x = 0.0;
  mrb_float y = 0.0;
  moon_vec2 *vec2;
  mrb_get_args(mrb, "|ff", &x, &y);
  vec2 = (moon_vec2*)DATA_PTR(self);
  if (vec2) {
    vector2_free(mrb, (void*)vec2);
  }
  mrb_set_vector2_value_xy(mrb, self, x, y);
  return self;
}

static mrb_value
vector2_initialize_copy(mrb_state *mrb, mrb_value self)
{
  moon_vec2* src_vec;
  mrb_get_args(mrb, "d", &src_vec, &vector2_data_type);
  mrb_set_vector2_value_xy(mrb, self, moon_vector_ptr(src_vec)->x,
                                      moon_vector_ptr(src_vec)->y);
  return self;
}

static mrb_value
vector2_coerce(mrb_state *mrb, mrb_value self)
{
  mrb_value other;
  mrb_get_args(mrb, "o", &other);
  mrb_value argv[2] = { self, other };
  return mrb_ary_new_from_values(mrb, 2, argv);
}

static mrb_value
vector2_x_getter(mrb_state *mrb, mrb_value self)
{
  m_vector_unwrap_self(vec);
  return mrb_float_value(mrb, moon_vector_ptr(vec)->x);
}

static mrb_value
vector2_y_getter(mrb_state *mrb, mrb_value self)
{
  m_vector_unwrap_self(vec);
  return mrb_float_value(mrb, moon_vector_ptr(vec)->y);
}

static mrb_value
vector2_x_setter(mrb_state *mrb, mrb_value self)
{
  mrb_float x;
  m_vector_unwrap_self(vec);
  mrb_get_args(mrb, "f", &x);
  moon_vector_ptr(vec)->x = x;
  return mrb_nil_value();
}

static mrb_value
vector2_y_setter(mrb_state *mrb, mrb_value self)
{
  mrb_float y;
  m_vector_unwrap_self(vec);
  mrb_get_args(mrb, "f", &y);
  moon_vector_ptr(vec)->y = y;
  return mrb_nil_value();
}

static mrb_value
vector2_negate(mrb_state *mrb, mrb_value self)
{
  m_vector_mutate_copy(-(moon_vector_ref(result_vec)));
}

static mrb_value
vector2_identity(mrb_state *mrb, mrb_value self)
{
  return mrb_obj_dup(mrb, self);
}

static mrb_value
vector2_add(mrb_state *mrb, mrb_value self)
{
  m_vector_operator(+);
}

static mrb_value
vector2_sub(mrb_state *mrb, mrb_value self)
{
  m_vector_operator(-);
}

static mrb_value
vector2_mul(mrb_state *mrb, mrb_value self)
{
  m_vector_operator(*);
}

static mrb_value
vector2_div(mrb_state *mrb, mrb_value self)
{
  m_vector_operator(/);
}

/* // Uncomment this when integer vectors are supported
static mrb_value
vector2_not(mrb_state *mrb, mrb_value self)
{
  m_vector_mutate_copy(~(moon_vector_ref(result_vec)));
}

static mrb_value
vector2_modulo(mrb_state *mrb, mrb_value self)
{
  m_vector_operator(%);
}

static mrb_value
vector2_shl(mrb_state *mrb, mrb_value self)
{
  m_vector_operator(<<);
}

static mrb_value
vector2_shr(mrb_state *mrb, mrb_value self)
{
  m_vector_operator(>>);
}

static mrb_value
vector2_and(mrb_state *mrb, mrb_value self)
{
  m_vector_operator(&);
}

static mrb_value
vector2_or(mrb_state *mrb, mrb_value self)
{
  m_vector_operator(|);
}

static mrb_value
vector2_xor(mrb_state *mrb, mrb_value self)
{
  m_vector_operator(^);
}
*/

static mrb_value
vector2_dot(mrb_state *mrb, mrb_value self)
{
  m_vector_operator_head(src_mvec, arg_vec);
  return mrb_float_value(mrb, glm::dot(moon_vector_ref(src_mvec), arg_vec));
}

static mrb_value
vector2_normalize(mrb_state *mrb, mrb_value self)
{
  m_vector_mutate_copy(glm::normalize(moon_vector_ref(result_vec)));
}

static mrb_value
vector2_length(mrb_state *mrb, mrb_value self)
{
  m_vector_unwrap_self(vec);
  return mrb_float_value(mrb, glm::length(moon_vector_ref(vec)));
}

static mrb_value
vector2_distance(mrb_state *mrb, mrb_value self)
{
  m_vector_operator_head(src_mvec, arg_vec);
  glm::vec2 diff = moon_vector_ref(src_mvec) - arg_vec;
  return mrb_float_value(mrb, glm::dot(diff, diff));
}

static mrb_value
vector2_rotate(mrb_state *mrb, mrb_value self)
{
  mrb_float angle;
  mrb_get_args(mrb, "f", &angle);
  m_vector_unwrap_self(vec);
  return mmrb_vector2_value(mrb, glm::rotate(moon_vector_ref(vec), (float)angle));
}

static mrb_value
vector2_lerp(mrb_state *mrb, mrb_value self)
{
  moon_vec2 *other;
  mrb_float delta;
  mrb_get_args(mrb, "df", &other, &vector2_data_type, &delta);
  m_vector_unwrap_self(vec);
  return mmrb_vector2_value(mrb, glm::lerp(moon_vector_ref(vec), moon_vector_ref(other), (float)delta));
}

static mrb_value
vector2_set(mrb_state *mrb, mrb_value self)
{
  m_vector_unwrap_self(mvec);
  m_vector_extract_to(moon_vector_ref(mvec));
  return self;
}

static mrb_value
vector2_to_a(mrb_state *mrb, mrb_value self)
{
  m_vector_unwrap_self(mvec2);
  mrb_value argv[2] = { mrb_float_value(mrb, moon_vector_ptr(mvec2)->x),
                        mrb_float_value(mrb, moon_vector_ptr(mvec2)->y) };
  return mrb_ary_new_from_values(mrb, 2, argv);
}

static mrb_value
vector2_s_extract(mrb_state *mrb, mrb_value self)
{
  m_vector_extract(src_vect);
  mrb_value argv[2] = { mrb_float_value(mrb, src_vect.x),
                        mrb_float_value(mrb, src_vect.y) };
  return mrb_ary_new_from_values(mrb, 2, argv);
}

static mrb_value
vector2_s_cast(mrb_state *mrb, mrb_value klass)
{
  mrb_value result = mrb_obj_new(mrb, vector2_class, 0, {});
  m_vector_unwrap(result, dest_vect);
  m_vector_extract(src_vect);
  moon_vector_ref(dest_vect) = src_vect;
  return result;
}

static mrb_value
vector2_s_circular_rand(mrb_state *mrb, mrb_value klass)
{
  mrb_float radius;
  mrb_get_args(mrb, "f", &radius);
  return mmrb_vector2_value(mrb, glm::circularRand((float)radius));
}

static mrb_value
vector2_s_disk_rand(mrb_state *mrb, mrb_value klass)
{
  mrb_float radius;
  mrb_get_args(mrb, "f", &radius);
  return mmrb_vector2_value(mrb, glm::diskRand((float)radius));
}

void
mmrb_vector2_init(mrb_state *mrb, struct RClass *mod)
{
  vector2_class = mrb_define_class_under(mrb, mod, "Vector2", mrb->object_class);
  MRB_SET_INSTANCE_TT(vector2_class, MRB_TT_DATA);

  mrb_define_method(mrb, vector2_class, "initialize",      vector2_initialize,      MRB_ARGS_OPT(2));
  mrb_define_method(mrb, vector2_class, "initialize_copy", vector2_initialize_copy, MRB_ARGS_REQ(1));
  /* coercion */
  mrb_define_method(mrb, vector2_class, "coerce",          vector2_coerce,          MRB_ARGS_REQ(1));
  /* attribute setters */
  mrb_define_method(mrb, vector2_class, "x",               vector2_x_getter,        MRB_ARGS_NONE());
  mrb_define_method(mrb, vector2_class, "y",               vector2_y_getter,        MRB_ARGS_NONE());
  mrb_define_method(mrb, vector2_class, "x=",              vector2_x_setter,        MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vector2_class, "y=",              vector2_y_setter,        MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vector2_class, "set",             vector2_set,             MRB_ARGS_ANY());
  /* arithmetic */
  mrb_define_method(mrb, vector2_class, "-@",              vector2_negate,          MRB_ARGS_NONE());
  mrb_define_method(mrb, vector2_class, "+@",              vector2_identity,        MRB_ARGS_NONE());
  mrb_define_method(mrb, vector2_class, "+",               vector2_add,             MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vector2_class, "-",               vector2_sub,             MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vector2_class, "*",               vector2_mul,             MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vector2_class, "/",               vector2_div,             MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vector2_class, "dot",             vector2_dot,             MRB_ARGS_REQ(1));
  /*mrb_define_method(mrb, vector2_class, "cross",           vector2_cross,           MRB_ARGS_REQ(1));*/
  /* */
  mrb_define_method(mrb, vector2_class, "normalize",       vector2_normalize,       MRB_ARGS_NONE());
  mrb_define_method(mrb, vector2_class, "length",          vector2_length,          MRB_ARGS_NONE());
  mrb_define_method(mrb, vector2_class, "distance",        vector2_distance,        MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vector2_class, "rotate",          vector2_rotate,          MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vector2_class, "lerp",            vector2_lerp,            MRB_ARGS_REQ(2));
  /* bitwise operators */ /* If and only if we ever support integer based vectors */
  /*mrb_define_method(mrb, vector2_class, "~@",              vector2_not,             MRB_ARGS_NONE());*/
  /*mrb_define_method(mrb, vector2_class, "%",               vector2_modulo,          MRB_ARGS_REQ(1));*/
  /*mrb_define_method(mrb, vector2_class, "<<",              vector2_shl,             MRB_ARGS_REQ(1));*/
  /*mrb_define_method(mrb, vector2_class, ">>",              vector2_shr,             MRB_ARGS_REQ(1));*/
  /*mrb_define_method(mrb, vector2_class, "&",               vector2_and,             MRB_ARGS_REQ(1));*/
  /*mrb_define_method(mrb, vector2_class, "|",               vector2_or,              MRB_ARGS_REQ(1));*/
  /*mrb_define_method(mrb, vector2_class, "^",               vector2_xor,             MRB_ARGS_REQ(1));*/
  /* conversion */
  mrb_define_method(mrb, vector2_class, "to_a",            vector2_to_a,            MRB_ARGS_NONE());
  /* cast */
  mrb_define_class_method(mrb, vector2_class, "[]",        vector2_s_cast,          MRB_ARGS_ANY());
  mrb_define_class_method(mrb, vector2_class, "extract",   vector2_s_extract,       MRB_ARGS_REQ(1));
  /* gen */
  mrb_define_method(mrb, vector2_class, "circular_rand",   vector2_s_circular_rand, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vector2_class, "disk_rand",       vector2_s_disk_rand,     MRB_ARGS_REQ(1));
}
