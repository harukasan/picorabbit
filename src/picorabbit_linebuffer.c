#include <string.h>
#include "../mrbgems/mruby-picorabbit-linebuffer/include/picorabbit_linebuffer.h"
#include "line_buffer.h"
#include "pico/stdlib.h"
#include "dvi_defines.h"


bool picorabbit_linebuffer_init(void)
{
    return true;
}

bool picorabbit_linebuffer_add_line(const uint8_t *data, size_t len)
{
    if (!linebuffer_add_line(data)) {
        return false;
    }
    return true;
}

uint32_t picorabbit_linebuffer_current_line(void)
{
    return linebuffer_current_line();
}

uint32_t picorabbit_linebuffer_line_width(void)
{
    return DVI_H_ACTIVE;
}

uint32_t picorabbit_linebuffer_size(void)
{
    return DVI_V_ACTIVE;
}
