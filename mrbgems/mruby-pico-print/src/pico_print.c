#include <mruby.h>
#include <mruby/string.h>
#include <stdio.h>

static mrb_value
mrb_pico_print(mrb_state *mrb, mrb_value self)
{
  mrb_value arg;
  mrb_get_args(mrb, "S", &arg);

  const char *str = mrb_string_value_cstr(mrb, &arg);
  printf("%s", str);

  return mrb_nil_value();
}

static mrb_value
mrb_pico_puts(mrb_state *mrb, mrb_value self)
{
  mrb_value arg;
  mrb_get_args(mrb, "S", &arg);

  const char *str = mrb_string_value_cstr(mrb, &arg);
  printf("%s\n", str);

  return mrb_nil_value();
}

void
mrb_mruby_pico_print_gem_init(mrb_state *mrb)
{
  struct RClass *krn = mrb->kernel_module;

  mrb_define_method(mrb, krn, "print", mrb_pico_print, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, krn, "puts", mrb_pico_puts, MRB_ARGS_REQ(1));
}

void
mrb_mruby_pico_print_gem_final(mrb_state *mrb)
{
}
