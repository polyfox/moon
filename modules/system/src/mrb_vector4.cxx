/*
 * Moon Vector4, a wrapper around Moon::Vector4
 */
#include <mruby.h>
#include <mruby/array.h>
#include <mruby/class.h>
#include <mruby/data.h>
#include <mruby/numeric.h>
#include <glm/glm.hpp>
#include "moon/mrb/vector1.hxx"
#include "moon/mrb/vector2.hxx"
#include "moon/mrb/vector3.hxx"
#include "moon/mrb/vector4.hxx"
#include "vec_helper.h"

#define m_vector_operator(__op__) \
  return mmrb_vector4_value(mrb, get_vector4_value(mrb, self) __op__ vector4_from_mrb_args(mrb));

static struct RClass *vector4_class = NULL;

static void
vector4_free(mrb_state *mrb, void *p)
{
  Moon::Vector4 *vec4 = (Moon::Vector4*)p;
  if (vec4) {
    delete(vec4);
  }
}

const struct mrb_data_type vector4_data_type = { "Moon::Vector4", vector4_free };

DEF_VEC_HELPERS(vector4, Moon::Vector4, vector4_class, &vector4_data_type);

static inline Moon::Vector4
mmrb_vector4_extract_mrb_array(mrb_state *mrb, mrb_value obj)
{
  Moon::Vector4 result;
  int _ary_len = mrb_ary_len(mrb, obj);
  if (_ary_len != 4) {
    mrb_raisef(mrb, E_ARGUMENT_ERROR,
               "wrong array size %d (expected 4)", _ary_len);
  } else {
    result.x = mmrb_to_flo(mrb, RARRAY_PTR(obj)[0]);
    result.y = mmrb_to_flo(mrb, RARRAY_PTR(obj)[1]);
    result.z = mmrb_to_flo(mrb, RARRAY_PTR(obj)[2]);
    result.w = mmrb_to_flo(mrb, RARRAY_PTR(obj)[3]);
  }
  return result;
}

static inline Moon::Vector4
mmrb_vector4_extract_mrb_num(mrb_state *mrb, mrb_value obj)
{
  double i = mmrb_to_flo(mrb, obj);
  return Moon::Vector4(i, i, i, i);
}

static inline Moon::Vector4
mmrb_vector4_extract_mrb_to_vec4(mrb_state *mrb, mrb_value obj)
{
  return get_vector4_value(mrb, mrb_funcall(mrb, obj, "to_vec4", 0));
}

Moon::Vector4
mmrb_vector4_extract_args(mrb_state *mrb, int argc, mrb_value *vals)
{
  Moon::Vector4 result;

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
        result = *get_vector4(mrb, val);
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
  case 3: {
    int index = 0;
    for (int i = 0; i < argc; ++i) {
      mrb_value val = vals[i];
      if (mrb_type(val) == MRB_TT_DATA) {
        const mrb_data_type *t = (mrb_data_type*)DATA_TYPE(val);
        /* Vector unrolling */
        if (t == &vector1_data_type) {
          Moon::Vector1 vec1 = mmrb_to_vector1(mrb, val);
          result[index++] = vec1[0]; if (index >= 4) break;
        } else if (t == &vector2_data_type) {
          Moon::Vector2 vec2 = mmrb_to_vector2(mrb, val);
          result[index++] = vec2[0]; if (index >= 4) break;
          result[index++] = vec2[1];
        } else if (t == &vector3_data_type) {
          Moon::Vector3 vec3 = mmrb_to_vector3(mrb, val);
          result[index++] = vec3[0]; if (index >= 4) break;
          result[index++] = vec3[1]; if (index >= 4) break;
          result[index++] = vec3[2];
        //} else if (t == &vector4_data_type) {
        //  Moon::Vector4 vec4 = mmrb_to_vector4(mrb, val);
        //  result[index++] = vec4[0]; if (index >= 4) break;
        //  result[index++] = vec4[1]; if (index >= 4) break;
        //  result[index++] = vec4[2]; if (index >= 4) break;
        //  result[index++] = vec4[3];
        } else {
          mrb_raisef(mrb, E_TYPE_ERROR,
                     "wrong type %S (expected Vector1, Vector2 or Vector3)",
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
  } case 4: {
    result.x = mmrb_to_flo(mrb, vals[0]);
    result.y = mmrb_to_flo(mrb, vals[1]);
    result.z = mmrb_to_flo(mrb, vals[2]);
    result.w = mmrb_to_flo(mrb, vals[3]);
    break;
  } default:
    mrb_raisef(mrb, E_ARGUMENT_ERROR,
               //"wrong number of arguments (%d for 1 or 4)", argc);
               "wrong number of arguments (%d for 1, 2, 3, or 4)", argc);
  }

  return result;
}

static Moon::Vector4
vector4_from_mrb_args(mrb_state *mrb)
{
  mrb_value *vals;
  int len;
  mrb_get_args(mrb, "*", &vals, &len);
  return mmrb_vector4_extract_args(mrb, len, vals);
}

Moon::Vector4
mmrb_to_vector4(mrb_state *mrb, mrb_value obj)
{
  return mmrb_vector4_extract_args(mrb, 1, &obj);
}

mrb_value
mmrb_vector4_value(mrb_state *mrb, Moon::Vector4 vec)
{
  return set_vector4(mrb, new_vector4(mrb), vec);
}

static mrb_value
vector4_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_float x = 0.0;
  mrb_float y = 0.0;
  mrb_float z = 0.0;
  mrb_float w = 0.0;
  mrb_get_args(mrb, "|ffff", &x, &y, &z, &w);
  cleanup_vector4(mrb, self);
  mrb_data_init(self, new Moon::Vector4(x, y, z, w), &vector4_data_type);
  return self;
}

static mrb_value
vector4_initialize_copy(mrb_state *mrb, mrb_value self)
{
  cleanup_vector4(mrb, self);
  mrb_data_init(self, new Moon::Vector4(vector4_from_mrb_args(mrb)), &vector4_data_type);
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
vector4_get_x(mrb_state *mrb, mrb_value self)
{
  return mrb_float_value(mrb, get_vector4(mrb, self)->x);
}

static mrb_value
vector4_get_y(mrb_state *mrb, mrb_value self)
{
  return mrb_float_value(mrb, get_vector4(mrb, self)->y);
}

static mrb_value
vector4_get_z(mrb_state *mrb, mrb_value self)
{
  return mrb_float_value(mrb, get_vector4(mrb, self)->z);
}

static mrb_value
vector4_get_w(mrb_state *mrb, mrb_value self)
{
  return mrb_float_value(mrb, get_vector4(mrb, self)->w);
}

static mrb_value
vector4_set_x(mrb_state *mrb, mrb_value self)
{
  mrb_float x;
  mrb_get_args(mrb, "f", &x);
  get_vector4(mrb, self)->x = x;
  return self;
}

static mrb_value
vector4_set_y(mrb_state *mrb, mrb_value self)
{
  mrb_float y;
  mrb_get_args(mrb, "f", &y);
  get_vector4(mrb, self)->y = y;
  return self;
}

static mrb_value
vector4_set_z(mrb_state *mrb, mrb_value self)
{
  mrb_float z;
  mrb_get_args(mrb, "f", &z);
  get_vector4(mrb, self)->z = z;
  return self;
}

static mrb_value
vector4_set_w(mrb_state *mrb, mrb_value self)
{
  mrb_float w;
  mrb_get_args(mrb, "f", &w);
  get_vector4(mrb, self)->w = w;
  return self;
}

static mrb_value
vector4_negate(mrb_state *mrb, mrb_value self)
{
  return mmrb_vector4_value(mrb, -get_vector4_value(mrb, self));
}

static mrb_value
vector4_identity(mrb_state *mrb, mrb_value self)
{
  return mrb_obj_dup(mrb, self);
}

static mrb_value
vector4_normalize(mrb_state *mrb, mrb_value self)
{
  return mmrb_vector4_value(mrb, glm::normalize(get_vector4_value(mrb, self)));
}

static mrb_value
vector4_length(mrb_state *mrb, mrb_value self)
{
  return mrb_float_value(mrb, glm::length(get_vector4_value(mrb, self)));
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
  return mrb_float_value(mrb, glm::dot(get_vector4_value(mrb, self), vector4_from_mrb_args(mrb)));
}

static mrb_value
vector4_distance(mrb_state *mrb, mrb_value self)
{
  Moon::Vector4 diff = get_vector4_value(mrb, self) - vector4_from_mrb_args(mrb);
  return mrb_float_value(mrb, glm::dot(diff, diff));
}

static mrb_value
vector4_set(mrb_state *mrb, mrb_value self)
{
  return set_vector4(mrb, self, vector4_from_mrb_args(mrb));
}

static mrb_value
vector4_to_a(mrb_state *mrb, mrb_value self)
{
  Moon::Vector4* mvec4 = get_vector4(mrb, self);
  mrb_value argv[4] = { mrb_float_value(mrb, mvec4->x),
                        mrb_float_value(mrb, mvec4->y),
                        mrb_float_value(mrb, mvec4->z),
                        mrb_float_value(mrb, mvec4->w) };
  return mrb_ary_new_from_values(mrb, 4, argv);
}

static mrb_value
vector4_s_extract(mrb_state *mrb, mrb_value self)
{
  Moon::Vector4 v4a = vector4_from_mrb_args(mrb);
  mrb_value argv[4] = { mrb_float_value(mrb, v4a.x),
                        mrb_float_value(mrb, v4a.y),
                        mrb_float_value(mrb, v4a.z),
                        mrb_float_value(mrb, v4a.w) };
  return mrb_ary_new_from_values(mrb, 4, argv);
}

static mrb_value
vector4_s_cast(mrb_state *mrb, mrb_value klass)
{
  return set_vector4(mrb, new_vector4(mrb), vector4_from_mrb_args(mrb));
}

void
mmrb_vector4_init(mrb_state *mrb, struct RClass *mod)
{
  vector4_class = mrb_define_class_under(mrb, mod, "Vector4", mrb->object_class);
  MRB_SET_INSTANCE_TT(vector4_class, MRB_TT_DATA);

  mrb_define_method(mrb, vector4_class, "initialize",      vector4_initialize,      MRB_ARGS_OPT(4));
  mrb_define_method(mrb, vector4_class, "initialize_copy", vector4_initialize_copy, MRB_ARGS_REQ(1));
  /* coercion */
  mrb_define_method(mrb, vector4_class, "coerce",          vector4_coerce,          MRB_ARGS_REQ(1));
  /* attribute setters */
  mrb_define_method(mrb, vector4_class, "x",               vector4_get_x,        MRB_ARGS_NONE());
  mrb_define_method(mrb, vector4_class, "y",               vector4_get_y,        MRB_ARGS_NONE());
  mrb_define_method(mrb, vector4_class, "z",               vector4_get_z,        MRB_ARGS_NONE());
  mrb_define_method(mrb, vector4_class, "w",               vector4_get_w,        MRB_ARGS_NONE());
  mrb_define_method(mrb, vector4_class, "x=",              vector4_set_x,        MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vector4_class, "y=",              vector4_set_y,        MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vector4_class, "z=",              vector4_set_z,        MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vector4_class, "w=",              vector4_set_w,        MRB_ARGS_REQ(1));
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
}
