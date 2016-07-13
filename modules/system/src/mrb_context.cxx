#include <mruby.h>
#include <mruby/array.h>
#include <mruby/class.h>
#include <mruby/compile.h>
#include "moon/api.h"
#include "moon/mrb/context.hxx"

static void
context_free(mrb_state *mrb, void *p)
{
  mrbc_context *context = (mrbc_context*)p;
  if (context) {
    mrbc_context_free(mrb, context);
  }
}

static inline mrbc_context*
get_context(mrb_state *mrb, mrb_value self)
{
  return (mrbc_context*)mrb_data_get_ptr(mrb, self, &context_data_type);
}

const struct mrb_data_type context_data_type = { "Context", context_free };

static mrb_value
context_initialize(mrb_state *mrb, mrb_value self)
{
  mrbc_context *cxt;
  cxt = (mrbc_context*)DATA_PTR(self);
  if (cxt) {
    context_free(mrb, (void*)cxt);
  }
  cxt = mrbc_context_new(mrb);
  mrb_data_init(self, cxt, &context_data_type);
  return self;
}

/* Evaluates a given string inside the context.
 * @param [String] str
 * @return [Object] v value returned from executing the string
 */
static mrb_value
context_eval(mrb_state *mrb, mrb_value self)
{
  char* str;
  mrb_get_args(mrb, "z", &str);
  mrbc_context *cxt = get_context(mrb, self);
  auto v = mrb_load_string_cxt(mrb, str, cxt);
  if (mrb->exc) {
    mrb_print_error(mrb);
  }
  return v;
}

/**
 * Returns each defined symbol in the given context
 *
 * @yieldparam [Symbol] symbol
 * @return [self]
 */
static mrb_value
context_each_symbol(mrb_state *mrb, mrb_value self)
{
  mrb_value blk;
  mrbc_context *cxt = get_context(mrb, self);
  mrb_get_args(mrb, "&", &blk);
  for (int i = 0; i < cxt->slen; ++i) {
    mrb_yield(mrb, blk, mrb_symbol_value(cxt->syms[i]));
  }
  return self;
}

MOON_C_API void
mmrb_context_init(mrb_state *mrb)
{
  struct RClass *mod = mrb_define_module(mrb, "Moon");
  struct RClass *context_class = mrb_define_class_under(mrb, mod, "Context", mrb->object_class);
  MRB_SET_INSTANCE_TT(context_class, MRB_TT_DATA);

  mrb_define_method(mrb, context_class, "initialize",  context_initialize,  MRB_ARGS_NONE());
  mrb_define_method(mrb, context_class, "eval",        context_eval,        MRB_ARGS_REQ(1));
  mrb_define_method(mrb, context_class, "each_symbol", context_each_symbol, MRB_ARGS_NONE() | MRB_ARGS_BLOCK());
}
