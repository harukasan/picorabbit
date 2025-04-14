#include <mruby.h>
#include <mruby/string.h>
#include <mruby/data.h>
#include <string.h>
#include "../../../include/framebuffer.h"
#include "../../../include/draw.h"
#include "../../../include/image.h"

#include "../include/picorabbit_textbuf.h"

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
    mrb_get_args(mrb, "iiiii", &x, &y, &width, &height, &color);

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
mrb_draw_image(mrb_state *mrb, mrb_value self)
{
    mrb_value image_name;
    mrb_int x, y;
    mrb_float angle = 0.0;

    mrb_get_args(mrb, "oii|f", &image_name, &x, &y, &angle);

    if (!mrb_symbol_p(image_name)) {
        mrb_raise(mrb, E_TYPE_ERROR, "image name must be a symbol");
        return mrb_nil_value();
    }

    const uint8_t *image_data = NULL;
    const uint8_t *mask_data = NULL;
    const char *name = mrb_sym2name(mrb, mrb_symbol(image_name));

    int width = 0;
    int height = 0;
    bool has_mask = false;
    if (strcmp(name, "rubykaigi2025") == 0) {
        image_data = image_data_rubykaigi2025;
        has_mask = false;
        width = IMAGE_WIDTH_RUBYKAIGI2025;
        height = IMAGE_HEIGHT_RUBYKAIGI2025;
    } else if (strcmp(name, "kame") == 0) {
        image_data = image_data_kame;
        mask_data = mask_data_kame;
        has_mask = true;
        width = IMAGE_WIDTH_KAME;
        height = IMAGE_HEIGHT_KAME;
    } else if (strcmp(name, "usagi") == 0) {
        image_data = image_data_usagi;
        mask_data = mask_data_usagi;
        has_mask = true;
        width = IMAGE_WIDTH_USAGI;
        height = IMAGE_HEIGHT_USAGI;
    } else {
        mrb_raisef(mrb, E_ARGUMENT_ERROR, "unknown image name: %s", name);
        return mrb_nil_value();
    }

    if (has_mask) {
        if (angle != 0.0) {
            draw_image_masked_rotated(framebuffer_get_draw(), image_data, mask_data, x, y, width, height, angle);
        } else {
            draw_image_masked(framebuffer_get_draw(), image_data, mask_data, x, y, width, height);
        }
    } else {
        draw_image(framebuffer_get_draw(), image_data, x, y, width, height);
    }

    return mrb_nil_value();
}

static mrb_value
mrb_commit(mrb_state *mrb, mrb_value self)
{
    framebuffer_commit();
    return mrb_nil_value();
}

static mrb_value
mrb_draw_line(mrb_state *mrb, mrb_value self)
{
    mrb_int x0, y0, x1, y1, color;
    mrb_get_args(mrb, "iiiii", &x0, &y0, &x1, &y1, &color);

    draw_line(framebuffer_get_draw(), x0, y0, x1, y1, color);

    return mrb_nil_value();
}

void mrb_mruby_picorabbit_draw_gem_init_draw(mrb_state *mrb, struct RClass *module_picorabbit)
{
}

void mrb_mruby_picorabbit_draw_gem_init(mrb_state *mrb)
{
    struct RClass *module_picorabbit = mrb_define_module(mrb, "PicoRabbit");
    struct RClass *module_draw = mrb_define_module_under(mrb, module_picorabbit, "Draw");

    mrb_define_module_function(mrb, module_draw, "background", mrb_background, MRB_ARGS_REQ(1));
    mrb_define_module_function(mrb, module_draw, "draw_rect", mrb_draw_rect, MRB_ARGS_REQ(5));
    mrb_define_module_function(mrb, module_draw, "draw_text", mrb_draw_text_with_color, MRB_ARGS_REQ(4));
    mrb_define_module_function(mrb, module_draw, "draw_image", mrb_draw_image, MRB_ARGS_REQ(5));
    mrb_define_module_function(mrb, module_draw, "draw_line", mrb_draw_line, MRB_ARGS_REQ(5));
    mrb_define_module_function(mrb, module_draw, "commit", mrb_commit, MRB_ARGS_NONE());

    mrb_mruby_picorabbit_draw_gem_init_textbuf(mrb, module_draw);
}

void mrb_mruby_picorabbit_draw_gem_final(mrb_state *mrb) {
    mrb_mruby_picorabbit_draw_gem_final_textbuf(mrb);
}
