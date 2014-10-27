#ifndef MOON_MRB_H
#define MOON_MRB_H

#include <mruby.h>
#include <mruby/array.h>
#include <mruby/class.h>
#include <mruby/compile.h>
#include <mruby/data.h>
#include <mruby/hash.h>
#include <mruby/numeric.h>
#include <mruby/proc.h>
#include <mruby/string.h>
#include <mruby/variable.h>

#include "moon/stdh_c.hxx"

extern struct RClass* mmrb_Moon;

void mmrb_init(mrb_state *mrb);

#endif
