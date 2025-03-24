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
#include "text_renderer.h"

// Core 1: DVI output and line buffer management
void core1_main() {
    init_clock();
    linebuffer_init(DVI_H_ACTIVE);
    dvi_start();
}

// Core 0: Line generation and LED control
void core0_main() {
    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    // Initialize text renderer
    text_renderer_init();

    // Wait for line buffer to be ready
    while (!linebuffer_wait_ready()) {
        tight_loop_contents();
    }

    // Wait for DVI to start
    dvi_wait_for_transfer();

    // Render colorful text
    render_text(2, 2, "Hello", COLOR_RED);
    render_text(8, 2, "World", COLOR_BLUE);
    render_text(14, 2, "!", COLOR_GREEN);

    // Custom color example
    render_text(2, 4, "RGB332 Colors:", COLOR_WHITE);
    render_text(2, 5, "Custom Color", RGB332(7,4,2)); // Orange-like color

    uint32_t frame_count = 0;
    while (true) {
        // Send lines to Core 1
        for (uint line = 0; line < DVI_V_ACTIVE; line++) {
            linebuffer_write_line(get_text_line(line), line);
        }

        // Blink LED every 30 frames (about 0.5 seconds at 60Hz)
        if (++frame_count >= 30) {
            gpio_xor_mask(1u << LED_PIN);
            frame_count = 0;
        }
    }
}

int main() {
    // Configure voltage for high-speed operation
    vreg_set_voltage(VREG_VOLTAGE_1_30);
    sleep_ms(100);

    stdio_init_all();

    printf("PicoRabbit DVI - RGB332 Text Test\n");

    // Launch core 1
    printf("\nStarting Core 1...\n");
    multicore_launch_core1(core1_main);

    // Run core 0 main loop
    core0_main();

    return 0;
}
