#include <stdio.h>
#include <string.h>

#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "hardware/gpio.h"
#include "hardware/vreg.h"
#include "hardware/clocks.h"
#include "hardware/irq.h"
#include "dvi.h"
#include "linebuffer.h"
#include "clock.h"

#define CORE_READY_FLAG 123

// Core 1: DVI output and line buffer management
void core1_main() {
    linebuffer_init(DVI_H_ACTIVE);
    dvi_start();
}

static uint8_t line_data[DVI_H_ACTIVE];

// Core 0: Line generation and LED control
void core0_main() {
    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    // Wait for Core 1 to be ready
    if (!linebuffer_wait_ready()) {
        printf("Core 1 startup failed!\n");
        return;
    }

    // Wait for DVI to start
    dvi_wait_for_transfer();

    bool is_white = false;

    while (true) {
        // Toggle LED
        gpio_xor_mask(1u << LED_PIN);

        // Fill line data
        memset(line_data, is_white ? COLOR_WHITE : COLOR_BLACK, DVI_H_ACTIVE);

        // Send lines to Core 1
        for (uint line = 0; line < DVI_V_ACTIVE; line++) {
            linebuffer_write_line(line_data, line);
        }

        is_white = !is_white;
        printf("%s\n", is_white ? "white" : "black");
        sleep_ms(500);
    }
}

int main() {
    // Configure voltage for high-speed operation
    vreg_set_voltage(VREG_VOLTAGE_1_30);
    sleep_ms(10);

    init_clock();

    stdio_init_all();

    printf("PicoRabbit DVI\n");

    uint hstx_clock = clock_get_hz(clk_hstx);
    double actual_refresh = hstx_clock / (double)(DVI_H_TOTAL * DVI_V_TOTAL * 10 / 2);

    // Print clock information
    printf("\nClock Status:\n");
    printf("  System clock: %lu KHz\n", clock_get_hz(clk_sys));
    printf("  USB PLL clock: %lu Hz\n", clock_get_hz(clk_usb));
    printf("  Peripheral clock: %lu Hz\n", clock_get_hz(clk_peri));
    printf("  HSTX clock: %lu Hz\n", hstx_clock);
    printf("  actual refresh rate: %.2f Hz\n", actual_refresh);

    // Launch core 1
    printf("\nStarting Core 1...\n");
    multicore_launch_core1(core1_main);

    // Run core 0 main loop
    core0_main();

    return 0;
}
