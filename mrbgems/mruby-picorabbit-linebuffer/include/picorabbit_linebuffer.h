#ifndef PICORABBIT_LINEBUFFER_H
#define PICORABBIT_LINEBUFFER_H

#include <stdint.h>
#include <stdbool.h>

bool picorabbit_linebuffer_init(void);
bool picorabbit_linebuffer_add_line(const uint8_t *data, size_t len);
uint32_t picorabbit_linebuffer_current_line(void);
uint32_t picorabbit_linebuffer_line_width(void);
uint32_t picorabbit_linebuffer_size(void);

#endif // PICORABBIT_LINEBUFFER_H
