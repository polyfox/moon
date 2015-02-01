#ifndef MOON_VEC_HELPERS_H
#define MOON_VEC_HELPERS_H

// BASENAME, TYPE, CLASS, MRUBY_TYPE
#define DEF_VEC_HELPERS(N, T, C, MT) \
static inline T* \
get_ ## N(mrb_state *mrb, mrb_value self) \
{ \
  return (T*)mrb_data_get_ptr(mrb, self, MT); \
} \
 \
static inline T* \
get_ ## N ## _unsafe(mrb_state *mrb, mrb_value self) \
{ \
  return (T*)DATA_PTR(self); \
} \
 \
static inline T \
get_ ## N ## _value(mrb_state *mrb, mrb_value self) \
{ \
  return *get_ ## N(mrb, self); \
} \
 \
static inline T* \
get_ ## N ## _from_args(mrb_state *mrb) \
{ \
  T* other; \
  mrb_get_args(mrb, "d", &other, MT); \
  return other; \
} \
 \
static inline T \
get_ ## N ## _from_args_value(mrb_state *mrb) \
{ \
  return *get_ ## N ## _from_args(mrb); \
} \
 \
static inline mrb_value \
set_ ## N(mrb_state *mrb, mrb_value self, T v) \
{ \
  (*(T*)DATA_PTR(self)) = v; \
  return self; \
} \
 \
static inline mrb_value \
new_ ## N(mrb_state *mrb) \
{ \
  return mrb_obj_new(mrb,  N ## _class, 0, NULL); \
} \
 \
static inline void \
cleanup_ ## N(mrb_state *mrb, mrb_value self) \
{ \
  T *v = get_ ## N ## _unsafe(mrb, self); \
  if (v) { \
     N ## _free(mrb, (void*)v); \
  } \
}

#endif
