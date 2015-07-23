#ifndef MOON_VEC_HELPERS_H
#define MOON_VEC_HELPERS_H

/*
 * DEF_VEC_HELPERS(vector2, Moon::Vector2, vector2_class, &vector2_data_type);
 * Moon::Vector2* get_vector2(mrb_state *mrb, mrb_value self)
 * Moon::Vector2 get_vector2_value(mrb_state *mrb, mrb_value self)
 * Moon::Vector2* get_vector2_from_args(mrb_state *mrb)
 * Moon::Vector2 get_vector2_from_args_value(mrb_state *mrb)
 * mrb_value set_vector2(mrb_state *mrb, mrb_value self, Moon::Vector2 v)
 */
/* BASENAME, TYPE, CLASS, MRUBY_TYPE */
#define DEF_VEC_HELPERS(N, T, C, MT)                                           \
/* get_BASENAME_value(mrb_state *mrb, mrb_value self) */                       \
static inline T                                                                \
get_ ## N ## _value(mrb_state *mrb, mrb_value self)                            \
{                                                                              \
  return *mmrb_ ## N ## _ptr(mrb, self);                                       \
}                                                                              \
/* get_BASENAME_from_args(mrb_state *mrb) */                                   \
static inline T*                                                               \
get_ ## N ## _from_args(mrb_state *mrb)                                        \
{                                                                              \
  T* other;                                                                    \
  mrb_get_args(mrb, "d", &other, MT);                                          \
  return other;                                                                \
}                                                                              \
/* get_BASENAME_from_args_value(mrb_state *mrb, mrb_value self) */             \
static inline T                                                                \
get_ ## N ## _from_args_value(mrb_state *mrb)                                  \
{                                                                              \
  return *get_ ## N ## _from_args(mrb);                                        \
}                                                                              \
/* set_BASENAME(mrb_state *mrb, mrb_value self, T v) */                        \
static inline mrb_value                                                        \
set_ ## N(mrb_state *mrb, mrb_value self, T v)                                 \
{                                                                              \
  (*(T*)DATA_PTR(self)) = v;                                                   \
  return self;                                                                 \
}                                                                              \
/* new_BASENAME(mrb_state *mrb) */                                             \
static inline mrb_value                                                        \
new_ ## N(mrb_state *mrb)                                                      \
{                                                                              \
  return mrb_obj_new(mrb, C, 0, NULL);                              \
}                                                                              \
/* cleanup_BASENAME(mrb_state *mrb, mrb_value self) */                         \
static inline void                                                             \
cleanup_ ## N(mrb_state *mrb, mrb_value self)                                  \
{                                                                              \
  void *ptr = DATA_PTR(self);                                                  \
  if (ptr) {                                                                   \
    N ## _free(mrb, ptr);                                                      \
  }                                                                            \
}                                                                              \
/* mmrb_BASENAME_dup_value(mrb_state *mrb, mrb_value self, T vec) */           \
static inline mrb_value                                                        \
mmrb_ ## N ## _dup_value(mrb_state *mrb, mrb_value self, T vec)                \
{                                                                              \
  mrb_value target = mrb_obj_dup(mrb, self);                                   \
  return set_ ## N(mrb, target, vec);                                          \
}

#endif
