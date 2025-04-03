#include <stdint.h>
#include <string.h>
#include <mruby.h>
#include <mruby/data.h>
#include <mruby/class.h>

#include "../include/picorabbit_textbuf.h"

#include "../../../include/draw.h"
#include "../../../include/framebuffer.h"

#define TEXT_COLS 45
#define TEXT_ROWS 30

typedef struct
{
    uint16_t data[TEXT_ROWS][TEXT_COLS]; // character (8bit) and color (8bit)
} text_buf_t;

static const struct mrb_data_type textbuf_type = {
    "TextBuf",
    mrb_free,
};

// initialize
static mrb_value
textbuf_initialize(mrb_state *mrb, mrb_value self)
{
    text_buf_t *buf = (text_buf_t *)mrb_malloc(mrb, sizeof(text_buf_t));
    memset(buf, 0, sizeof(text_buf_t));
    DATA_TYPE(self) = &textbuf_type;
    DATA_PTR(self) = buf;
    return self;
}

// get(x, y)
static mrb_value
textbuf_get(mrb_state *mrb, mrb_value self)
{
    mrb_int x, y;
    mrb_get_args(mrb, "ii", &x, &y);

    text_buf_t *buf = DATA_PTR(self);
    if (!buf || x < 0 || x >= TEXT_COLS || y < 0 || y >= TEXT_ROWS)
        return mrb_nil_value();

    return mrb_fixnum_value(buf->data[y][x]);
}

// set(x, y, val)
static mrb_value
textbuf_set(mrb_state *mrb, mrb_value self)
{
    mrb_int x, y, val;
    mrb_get_args(mrb, "iii", &x, &y, &val);

    text_buf_t *buf = DATA_PTR(self);
    if (!buf || x < 0 || x >= TEXT_COLS || y < 0 || y >= TEXT_ROWS)
        return mrb_nil_value();

    buf->data[y][x] = (uint16_t)(val & 0xFFFF);
    return mrb_fixnum_value(val);
}

// render(fb)
extern void draw_text(uint8_t *buf, int x, int y, const char *text, int color);

static mrb_value
textbuf_render(mrb_state *mrb, mrb_value self)
{
    text_buf_t *buf = DATA_PTR(self);
    if (!buf)
        return mrb_nil_value();

    uint8_t *buffer = framebuffer_get_draw();

    char str[2] = {0};
    for (int y = 0; y < TEXT_ROWS; y++)
    {
        for (int x = 0; x < TEXT_COLS; x++)
        {
            uint16_t val = buf->data[y][x];
            uint8_t ch = val & 0xFF;
            uint8_t color = val >> 8;

            if (ch != 0 && ch != ' ')
            {
                str[0] = ch;
                draw_text(buffer, x * 8, y * 8, str, color);
            }
        }
    }

    return mrb_nil_value();
}

void mrb_mruby_picorabbit_draw_gem_init_textbuf(mrb_state *mrb, struct RClass *module_draw)
{
    struct RClass *tc = mrb_define_class_under(mrb, module_draw, "TextBuf", mrb->object_class);
    MRB_SET_INSTANCE_TT(tc, MRB_TT_DATA);

    mrb_define_method(mrb, tc, "initialize", textbuf_initialize, MRB_ARGS_NONE());
    mrb_define_method(mrb, tc, "get", textbuf_get, MRB_ARGS_REQ(2));
    mrb_define_method(mrb, tc, "set", textbuf_set, MRB_ARGS_REQ(3));
    mrb_define_method(mrb, tc, "render", textbuf_render, MRB_ARGS_NONE());
}

void mrb_mruby_picorabbit_draw_gem_final_textbuf(mrb_state *mrb) {}
