#ifdef PICO_DESKTOP_TEST

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "hardware_interface.h"

#define DISPLAY_WIDTH 320
#define DISPLAY_HEIGHT 240
#define BUFFER_SIZE (DISPLAY_WIDTH * DISPLAY_HEIGHT)

static uint8_t display_buffer[BUFFER_SIZE];
static uint32_t current_core = 0;
static uint32_t fifo_buffer[16];
static int fifo_read_pos = 0;
static int fifo_write_pos = 0;

void hw_init(void) {
    memset(display_buffer, 0, BUFFER_SIZE);
    memset(fifo_buffer, 0, sizeof(fifo_buffer));
    fifo_read_pos = 0;
    fifo_write_pos = 0;
}

uint8_t *hw_get_buffer(void) {
    return display_buffer;
}

void hw_update_display(void) {
    // デスクトップテストでは何もしない
}

// Multicore FIFO のモック実装
void multicore_fifo_push_blocking(uint32_t data) {
    fifo_buffer[fifo_write_pos] = data;
    fifo_write_pos = (fifo_write_pos + 1) % 16;
    printf("+ (mock) multicore_fifo_push_blocking: %d\n", data);
}

uint32_t multicore_fifo_pop_blocking(void) {
    uint32_t data = fifo_buffer[fifo_read_pos];
    fifo_read_pos = (fifo_read_pos + 1) % 16;
    printf("+ (mock) multicore_fifo_pop_blocking: %d\n", data);
    return data;
}

bool multicore_fifo_rvalid(void) {
    if (fifo_read_pos != fifo_write_pos) {
        printf("+ (mock) multicore_fifo_rvalid: true\n");
        return true;
    }
    printf("+ (mock) multicore_fifo_rvalid: false\n");
    return false;
}

uint32_t get_core_num(void) {
    return current_core;
}

// テスト用のヘルパー関数
void mock_set_core(uint32_t core_num) {
    current_core = core_num;
}

#endif // PICO_DESKTOP_TEST