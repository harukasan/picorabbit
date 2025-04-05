#include <mruby.h>

#include "../include/pico_time.h"

static mrb_value
mrb_pico_time_usec_32(mrb_state *mrb, mrb_value self)
{
    return mrb_fixnum_value(pico_time_us_32());
}

void mrb_mruby_pico_time_gem_init(mrb_state *mrb)
{
    mrb_define_method(mrb, mrb->kernel_module, "time_usec_32", mrb_pico_time_usec_32, MRB_ARGS_NONE());
}

void mrb_mruby_pico_time_gem_final(mrb_state *mrb) {}
