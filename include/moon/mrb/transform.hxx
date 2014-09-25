#ifndef MMRB_TRANSFORM_H
#define MMRB_TRANSFORM_H

#include <memory>
#include "moon/mrb.hxx"
#include "moon/transform.hxx"

extern struct mrb_data_type transform_data_type;
extern struct RClass* mmrb_Transform;
struct RClass* mmrb_transform_init(mrb_state *mrb);

#endif
