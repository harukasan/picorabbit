#ifndef _HARDWARE_INTERFACE_H_
#define _HARDWARE_INTERFACE_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef PICO_DESKTOP_TEST
    // デスクトップテスト用のモック関数
    void hw_init(void);
    uint8_t *hw_get_buffer(void);
    void hw_update_display(void);

    // multicore関連のモック
    void multicore_fifo_push_blocking(uint32_t data);
    uint32_t multicore_fifo_pop_blocking(void);
    bool multicore_fifo_rvalid(void);
    uint32_t get_core_num(void);
#else
    // 実機用の実装
    #include "pico/stdlib.h"
    #include "hardware/gpio.h"
    #include "pico/multicore.h"
#endif

#endif // _HARDWARE_INTERFACE_H_