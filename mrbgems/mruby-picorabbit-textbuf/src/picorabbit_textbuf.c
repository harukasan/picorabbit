#include <stdint.h>
#include <mruby.h>
#include <mruby/data.h>

#define TEXT_COLS 45
#define TEXT_ROWS 30
#define TEXT_SIZE (TEXT_COLS * TEXT_ROWS)

static uint16_t text_buf[TEXT_ROWS][TEXT_COLS];

static mrb_value
textbuf_get(mrb_state *mrb, mrb_value self)
{
  mrb_int x, y;
  mrb_get_args(mrb, "ii", &x, &y);

  if (x < 0 || x >= TEXT_COLS || y < 0 || y >= TEXT_ROWS)
    return mrb_nil_value();

  uint16_t val = text_buf[y][x];
  return mrb_fixnum_value(val);
}

static mrb_value
textbuf_set(mrb_state *mrb, mrb_value self)
{
  mrb_int x, y, val;
  mrb_get_args(mrb, "iii", &x, &y, &val);

  if (x < 0 || x >= TEXT_COLS || y < 0 || y >= TEXT_ROWS)
    return mrb_nil_value();

  text_buf[y][x] = (uint16_t)(val & 0xFFFF);
  return mrb_fixnum_value(val);
}

void
mrb_mruby_picorabbit_textbuf_gem_init(mrb_state* mrb)
{
  struct RClass *module_picorabbit = mrb_define_module(mrb, "PicoRabbit");
  struct RClass *module_textbuf = mrb_define_module_under(mrb, module_picorabbit, "TextBuf");
  
  mrb_define_module_function(mrb, module_textbuf, "get", textbuf_get, MRB_ARGS_REQ(2));
  mrb_define_module_function(mrb, module_textbuf, "set", textbuf_set, MRB_ARGS_REQ(3));
}

void
mrb_mruby_picorabbit_textbuf_gem_final(mrb_state* mrb) {}