#include <stdio.h>
#include <string.h>

#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "hardware/gpio.h"
#include "hardware/vreg.h"
#include "hardware/clocks.h"
#include "hardware/irq.h"
#include "dvi.h"
#include "clock.h"
#include "framebuffer.h"
#include "draw.h"

#include "mruby.h"
#include "mruby/irep.h"
#include "main_task.h"

// Core 1: DVI output and line buffer management
void core1_main()
{
    init_clock();
    sleep_ms(500);
    framebuffer_init();
    dvi_start();
}

// Core 0: Line generation and LED control
void core0_main()
{
    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    // Initialize mruby VM
    mrb_state *mrb = mrb_open();
    if (!mrb)
    {
        printf("Failed to open mrb\n");
        return;
    }

    // Call render function with line number
    mrb_value args[1];
    args[0] = mrb_fixnum_value(0); // line number
    mrb_value self = mrb_top_self(mrb);
    mrb_sym method = mrb_intern_lit(mrb, "render");

    // Wait for line buffer to be ready
    while (!framebuffer_wait_ready())
    {
        tight_loop_contents();
    }
    printf("framebuffer ready\n");

    // Wait for DVI to start
    dvi_wait_for_transfer();
    printf("dvi transfer started\n");

    // Load main task first to define the render function
    mrb_value ret = mrb_load_irep(mrb, main_task);
    if (mrb->exc)
    {
        printf("mruby execution failed:\n");
        mrb_print_error(mrb);
        while (true)
        {
            uint8_t *back_buffer = framebuffer_get_draw();
            draw_background(back_buffer, COLOR_RED);
            draw_text(back_buffer, 0, 8, "mruby execution failed!", COLOR_WHITE);
            framebuffer_commit();
            tight_loop_contents();
        }
    }

    mrb_close(mrb);
}

int main()
{
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
