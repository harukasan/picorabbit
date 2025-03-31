
#include <mruby.h>
#include <mruby/string.h>

#include "../../../include/framebuffer.h"
#include "../../../include/draw.h"

static mrb_value
mrb_background(mrb_state *mrb, mrb_value self)
{
    mrb_int color;
    mrb_get_args(mrb, "i", &color);

    draw_background(framebuffer_get_draw(), color);

    return mrb_nil_value();
}

static mrb_value
mrb_draw_rect(mrb_state *mrb, mrb_value self)
{
    mrb_int x, y, width, height, color;
    mrb_get_args(mrb, "iiii", &x, &y, &width, &height, &color);

    draw_rect(framebuffer_get_draw(), x, y, width, height, color);

    return mrb_nil_value();
}

static mrb_value
mrb_draw_text_with_color(mrb_state *mrb, mrb_value self)
{
    mrb_value str;
    mrb_int x, y, color;

    mrb_get_args(mrb, "Siii", &str, &x, &y, &color);

    uint8_t *buffer = framebuffer_get_draw();

    draw_text(buffer, x, y, mrb_string_value_cstr(mrb, &str), color);

    return mrb_nil_value();
}

static mrb_value
mrb_commit(mrb_state *mrb, mrb_value self)
{
    framebuffer_commit();
    return mrb_nil_value();
}

void mrb_mruby_picorabbit_draw_gem_init(mrb_state *mrb)
{
    struct RClass *module_picorabbit = mrb_define_module(mrb, "PicoRabbit");
    struct RClass *module_draw = mrb_define_module_under(mrb, module_picorabbit, "Draw");

    mrb_define_module_function(mrb, module_draw, "background", mrb_background, MRB_ARGS_REQ(1));
    mrb_define_module_function(mrb, module_draw, "draw_rect", mrb_draw_rect, MRB_ARGS_REQ(5));
    mrb_define_module_function(mrb, module_draw, "draw_text", mrb_draw_text_with_color, MRB_ARGS_REQ(4));

    mrb_define_module_function(mrb, module_draw, "commit", mrb_commit, MRB_ARGS_NONE());
}

void mrb_mruby_picorabbit_draw_gem_final(mrb_state *mrb) {}
