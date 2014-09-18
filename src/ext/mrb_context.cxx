#include "mrb.hxx"
#include <memory>
#include "graphics.hxx"

namespace Moon {

  static void moon_mrb_context_deallocate(mrb_state *mrb, void *p) {
    mrbc_context_free(mrb, (mrbc_context*)p);
  };

  const struct mrb_data_type context_data_type = {
    "Context", moon_mrb_context_deallocate,
  };

  static mrb_value
  moon_mrb_context_initialize(mrb_state *mrb, mrb_value self) {
    auto cxt = mrbc_context_new(mrb);

    DATA_TYPE(self) = &context_data_type;
    DATA_PTR(self) = cxt;

    return mrb_nil_value();
  };

  static mrb_value
  moon_mrb_context_eval(mrb_state *mrb, mrb_value self) {
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
  moon_mrb_context_symbols(mrb_state * mrb, mrb_value self) {
    mrbc_context * cxt;
    Data_Get_Struct(mrb, self, &context_data_type, cxt);

    mrb_value ary = mrb_ary_new_capa(mrb, cxt->slen);
    for (int i=0; i < cxt->slen; i++) {
      mrb_ary_push(mrb, ary, mrb_symbol_value(cxt->syms[i]));
    }
    return ary;
  }

  struct RClass*
  moon_mrb_context_init(mrb_state *mrb) {
    struct RClass *context_class;
    context_class = mrb_define_class_under(mrb, moon_module, "Context", mrb->object_class);
    MRB_SET_INSTANCE_TT(context_class, MRB_TT_DATA);

    mrb_define_method(mrb, context_class, "initialize", moon_mrb_context_initialize, MRB_ARGS_NONE());
    mrb_define_method(mrb, context_class, "eval",       moon_mrb_context_eval,       MRB_ARGS_REQ(1));
    mrb_define_method(mrb, context_class, "symbols",    moon_mrb_context_symbols,    MRB_ARGS_NONE());

    return context_class;
  };

}
