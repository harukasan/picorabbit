#include <string.h>
#include <stdio.h>

#include "hardware_interface.h"
#include "framebuffer.h"
#include "draw.h"

#define CORE_READY_FLAG 123

static framebuffer_t framebuffer;
static uint8_t framebuffer_buffers[2][FRAMEBUFFER_WIDTH * FRAMEBUFFER_HEIGHT];

void framebuffer_init(void)
{
    memset(&framebuffer, 0, sizeof(framebuffer));
    draw_background(framebuffer_buffers[0], COLOR_WHITE);
    draw_background(framebuffer_buffers[1], COLOR_WHITE);

    framebuffer.current_display_buffer = 0;

    multicore_fifo_push_blocking(CORE_READY_FLAG);
    printf("framebuffer initialized width=%d height=%d\n", FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT);
}

void framebuffer_deinit(void)
{
    // Nothing to do
}

const uint8_t *framebuffer_get_display(void)
{
    return (uint8_t *)&framebuffer_buffers[framebuffer.current_display_buffer];
}

uint8_t *framebuffer_get_draw(void)
{
    return (uint8_t *)&framebuffer_buffers[!framebuffer.current_display_buffer];
}

void framebuffer_commit(void)
{
    fifo_msg_t msg = {
        .buffer_index = !framebuffer.current_display_buffer // back buffer
    };
    multicore_fifo_push_blocking(msg.raw);
}

void framebuffer_get_line(uint8_t *line_buffer, uint32_t line)
{
    uint16_t *dst = (uint16_t *)line_buffer;
    // The framebuffer size is the half the screen size vertically.
    const uint8_t *src = &framebuffer_buffers[framebuffer.current_display_buffer][(line / 2) * FRAMEBUFFER_WIDTH];
    memcpy(line_buffer, src, FRAMEBUFFER_WIDTH);
}

// Wait for the next framebuffer to be committed
bool framebuffer_wait_ready(void)
{
    uint32_t flag = multicore_fifo_pop_blocking();
    return flag == CORE_READY_FLAG;
}

// Swap the front and back buffers
bool framebuffer_swap(void)
{
    // Check if there is new data in the FIFO
    if (multicore_fifo_rvalid())
    {
        uint32_t raw = multicore_fifo_pop_blocking();
        fifo_msg_t msg = {.raw = raw};

        if (msg.buffer_index != framebuffer.current_display_buffer)
        {
            framebuffer.current_display_buffer = msg.buffer_index;
            return true;
        }
    }
    return false;
}
