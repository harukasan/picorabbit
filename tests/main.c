#include <stdio.h>
#include <assert.h>

#include "hardware_interface.h"

// Prototype declarations for test functions
void test_draw_background(void);
void test_draw_rect(void);
void test_draw_text(void);
void test_draw_text_fast(void);
void run_framebuffer_tests(void);

int main() {
    printf("Starting tests...\n");

    // Hardware initialization
    hw_init();

    test_draw_background();
    test_draw_rect();
    test_draw_text();
    test_draw_text_fast();
    run_framebuffer_tests();

    printf("All tests passed successfully!\n");
    return 0;
}
