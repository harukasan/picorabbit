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

#include "mruby.h"
#include "mruby/irep.h"
#include "main_task.h"

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

    // Write blank lines to line buffer
    /* while (true){
        linebuffer_add_line(get_text_line(linebuffer_current_line()));
    }*/

    mrb_state *mrb = mrb_open();
    if (!mrb) {
        printf("Failed to open mrb\n");
        return;
    }
    mrb_load_irep(mrb, main_task);
    mrb_close(mrb);
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
