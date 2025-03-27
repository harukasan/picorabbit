#include <mruby.h>
#include <mruby/data.h>
#include <mruby/class.h>
#include <mruby/string.h>
#include "../include/picorabbit_linebuffer.h"

static mrb_value
mrb_picorabbit_linebuffer_init(mrb_state *mrb, mrb_value self)
{
  if (!picorabbit_linebuffer_init()) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "Failed to initialize line buffer");
  }
  return self;
}

static mrb_value
mrb_picorabbit_linebuffer_add_line(mrb_state *mrb, mrb_value self)
{
  mrb_value data;
  mrb_get_args(mrb, "S", &data);

  if (!picorabbit_linebuffer_add_line((const uint8_t *)RSTRING_PTR(data), RSTRING_LEN(data))) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "Failed to add line to buffer");
  }

  return mrb_nil_value();
}

static mrb_value
mrb_picorabbit_linebuffer_current_line(mrb_state *mrb, mrb_value self)
{
  return mrb_fixnum_value(picorabbit_linebuffer_current_line());
}

static mrb_value
mrb_picorabbit_linebuffer_line_width(mrb_state *mrb, mrb_value self)
{
  return mrb_fixnum_value(picorabbit_linebuffer_line_width());
}

static mrb_value
mrb_picorabbit_linebuffer_size(mrb_state *mrb, mrb_value self)
{
  return mrb_fixnum_value(picorabbit_linebuffer_size());
}

void
mrb_mruby_picorabbit_linebuffer_gem_init(mrb_state *mrb)
{
  struct RClass *module = mrb_define_module(mrb, "PicoRabbit");
  struct RClass *linebuffer = mrb_define_class_under(mrb, module, "LineBuffer", mrb->object_class);

  mrb_define_method(mrb, linebuffer, "initialize", mrb_picorabbit_linebuffer_init, MRB_ARGS_NONE());
  mrb_define_method(mrb, linebuffer, "add_line", mrb_picorabbit_linebuffer_add_line, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, linebuffer, "current_line", mrb_picorabbit_linebuffer_current_line, MRB_ARGS_NONE());
  mrb_define_method(mrb, linebuffer, "width", mrb_picorabbit_linebuffer_line_width, MRB_ARGS_NONE());
  mrb_define_method(mrb, linebuffer, "size", mrb_picorabbit_linebuffer_size, MRB_ARGS_NONE());
}

void
mrb_mruby_picorabbit_linebuffer_gem_final(mrb_state *mrb)
{
}
