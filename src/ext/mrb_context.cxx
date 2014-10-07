#include "moon/mrb/context.hxx"
#include "moon/graphics.hxx"

static void
context_free(mrb_state *mrb, void *p)
{
  mrbc_context *context = (mrbc_context*)p;
  if (context) {
    mrbc_context_free(mrb, context);
  }
}

struct mrb_data_type context_data_type = { "Context", context_free };

static mrb_value
context_initialize(mrb_state *mrb, mrb_value self)
{
  auto cxt = mrbc_context_new(mrb);

  DATA_TYPE(self) = &context_data_type;
  DATA_PTR(self) = cxt;

  return self;
}

static mrb_value
context_eval(mrb_state *mrb, mrb_value self)
{
  char* str;
  mrb_get_args(mrb, "z", &str);

  mrbc_context* cxt;
  Data_Get_Struct(mrb, self, &context_data_type, cxt);

  auto v = mrb_load_string_cxt(mrb, str, cxt);

  if (mrb->exc) {
    mrb_print_error(mrb);
  }
  return v;
}

static mrb_value
context_symbols(mrb_state * mrb, mrb_value self)
{
  mrbc_context * cxt;
  Data_Get_Struct(mrb, self, &context_data_type, cxt);

  mrb_value ary = mrb_ary_new_capa(mrb, cxt->slen);
  for (int i=0; i < cxt->slen; i++) {
    mrb_ary_push(mrb, ary, mrb_symbol_value(cxt->syms[i]));
  }
  return ary;
}

struct RClass*
mmrb_context_init(mrb_state *mrb)
{
  struct RClass *context_class;
  context_class = mrb_define_class_under(mrb, mmrb_Moon, "Context", mrb->object_class);
  MRB_SET_INSTANCE_TT(context_class, MRB_TT_DATA);

  mrb_define_method(mrb, context_class, "initialize", context_initialize, MRB_ARGS_NONE());
  mrb_define_method(mrb, context_class, "eval",       context_eval,       MRB_ARGS_REQ(1));
  mrb_define_method(mrb, context_class, "symbols",    context_symbols,    MRB_ARGS_NONE());

  return context_class;
}
