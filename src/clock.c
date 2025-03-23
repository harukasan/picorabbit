#include <stdio.h>
#include <stdlib.h>

#include "pico/stdlib.h"
#include "hardware/clocks.h"
#include "hardware/pll.h"
#include "hardware/uart.h"
#include "hardware/vreg.h"
#include "hardware/structs/hstx_ctrl.h"

#include "clock.h"
#include "dvi_defines.h"

// Find the nearest achievable clock frequency
// Ref: https://u-mid.hateblo.jp/entry/2024/11/09/175142
static bool check_near_clock_hz(uint32_t req_freq_hz, uint *freq_hz_out, uint *vco_out, uint *postdiv1_out, uint *postdiv2_out) {
    uint reference_freq_hz = XOSC_HZ / PLL_USB_REFDIV;
    uint32_t best_margin = 0xffffffff;
    for (uint fbdiv = 320; fbdiv >= 16; fbdiv--) {
        uint vco_hz = fbdiv * reference_freq_hz;
        if (vco_hz < PICO_PLL_VCO_MIN_FREQ_HZ || vco_hz > PICO_PLL_VCO_MAX_FREQ_HZ)
            continue;
        for (uint postdiv1 = 7; postdiv1 >= 1; postdiv1--) {
            for (uint postdiv2 = postdiv1; postdiv2 >= 1; postdiv2--) {
                uint freq_hz = vco_hz / (postdiv1 * postdiv2);
                uint margin = abs((int)(freq_hz - req_freq_hz));
                if (margin < best_margin) {
                    best_margin = margin;
                    *freq_hz_out = freq_hz;
                    *vco_out = vco_hz;
                    *postdiv1_out = postdiv1;
                    *postdiv2_out = postdiv2;
                }
            }
        }
    }
    return best_margin != 0xffffffff;
}

void set_hstx_clock(void) {
    uint hstx_clock, vco, postdiv1, postdiv2;

    // Find the nearest achievable clock frequency
    if (!check_near_clock_hz(DVI_HSTX_CLOCK_HZ, &hstx_clock, &vco, &postdiv1, &postdiv2)) {
        printf("Failed to find suitable HSTX clock settings\n");
        return;
    }

    // Initialize USB PLL with calculated parameters
    pll_init(pll_usb, PLL_USB_REFDIV, vco, postdiv1, postdiv2);

    // Configure HSTX to use USB PLL directly
    clock_configure_undivided(clk_hstx,
                            0,
                            CLOCKS_CLK_HSTX_CTRL_AUXSRC_VALUE_CLKSRC_PLL_USB,
                            hstx_clock);
}

void init_clock(void) {
    // Configure system and peripheral clocks
    int usb_clock = 48 * MHZ;    // USB PLL clock frequency
    int clock_ratio = 3;         // System PLL multiplier relative to USB PLL
    uint32_t system_clock = usb_clock * clock_ratio;

    // Set system clock to 144MHz (3 * 48MHz)
    set_sys_clock_hz(system_clock, true);
    clock_configure_int_divider(clk_usb,
                                0,
                                CLOCKS_CLK_USB_CTRL_AUXSRC_VALUE_CLKSRC_PLL_SYS,
                                system_clock,
                                clock_ratio);
    clock_configure_int_divider(clk_adc,
                                0,
                                CLOCKS_CLK_ADC_CTRL_AUXSRC_VALUE_CLKSRC_PLL_SYS,
                                system_clock,
                                clock_ratio);
    clock_configure_undivided(clk_peri,
                                0,
                                CLOCKS_CLK_PERI_CTRL_AUXSRC_VALUE_CLKSRC_PLL_SYS,
                                system_clock);


    // Configure UART0 with explicit baud rate
    uart_init(uart0, 115200);

    // Configure HSTX clock using USB PLL
    set_hstx_clock();

    uint hstx_clock = clock_get_hz(clk_hstx);
    double actual_refresh = hstx_clock / (double)(DVI_H_TOTAL * DVI_V_TOTAL * 10 / 2);

    // Print clock information
    printf("\nClock Status:\n");
    printf("  System clock: %lu KHz\n", clock_get_hz(clk_sys) / 1000);
    printf("  USB PLL clock: %lu KHz\n", clock_get_hz(clk_usb) / 1000);
    printf("  Peripheral clock: %lu KHz\n", clock_get_hz(clk_peri) / 1000);
    printf("  HSTX clock: %lu KHz\n", hstx_clock / 1000);
    printf("  Target HSTX clock: %llu KHz\n", DVI_HSTX_CLOCK_HZ / 1000);
    printf("  actual refresh rate: %.2f Hz\n", actual_refresh);
}
