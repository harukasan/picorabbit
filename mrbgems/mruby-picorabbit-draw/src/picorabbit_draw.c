#include <mruby.h>
#include <mruby/string.h>

#include "../../../include/line_buffer.h"
#include "../../../include/draw_text.h"

/**
 * Draw text with specified color at given coordinates
 * @param mrb   mruby state
 * @param str   text to draw
 * @param x     x coordinate
 * @param y     y coordinate
 * @param color color value
 */
static mrb_value
mrb_draw_text_with_color(mrb_state *mrb, mrb_value self)
{
    mrb_value str;
    mrb_int x, y, color;

    mrb_get_args(mrb, "Siii", &str, &x, &y, &color);

    uint8_t *buffer = line_buffer_get_back_buffer();
    uint32_t width = line_buffer_get_width();

    draw_text_line(buffer, width, y, x, y, mrb_string_value_cstr(mrb, &str), color);

    return mrb_nil_value();
}

static mrb_value
mrb_background(mrb_state *mrb, mrb_value self)
{
    mrb_int color;
    mrb_get_args(mrb, "i", &color);

    line_buffer_fill(color);

    return mrb_nil_value();
}

static mrb_value
mrb_commit_line(mrb_state *mrb, mrb_value self)
{
    mrb_int line_y;
    mrb_get_args(mrb, "i", &line_y);

    line_buffer_commit_line(line_y);
    return mrb_nil_value();
}

void
mrb_mruby_picorabbit_draw_gem_init(mrb_state *mrb)
{
    struct RClass *module_picorabbit = mrb_define_module(mrb, "PicoRabbit");
    struct RClass *module_draw = mrb_define_module_under(mrb, module_picorabbit, "Draw");

    mrb_define_module_function(mrb, module_draw, "background", mrb_background, MRB_ARGS_REQ(1));
    mrb_define_module_function(mrb, module_draw, "commit_line", mrb_commit_line, MRB_ARGS_REQ(1));

    mrb_define_module_function(mrb, module_draw, "_c_draw_text_with_color", mrb_draw_text_with_color, MRB_ARGS_REQ(4));

}

void
mrb_mruby_picorabbit_draw_gem_final(mrb_state *mrb)
{
    // Cleanup code if needed
}
