#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "hardware_interface.h"
#include "framebuffer.h"

// Prototype declaration for test helper functions
extern void mock_set_core(uint32_t core_num);

void test_framebuffer_init(void) {
    framebuffer_init();

    // After initialization, display buffer should be 0
    const uint8_t *display_buffer = framebuffer_get_display();
    assert(display_buffer != NULL);

    // Draw buffer should be 1
    uint8_t *draw_buffer = framebuffer_get_draw();
    assert(draw_buffer != NULL);
    assert(draw_buffer != display_buffer);
}

void test_framebuffer_swap(void) {
    framebuffer_init();
    framebuffer_wait_ready();

    // Save initial state
    const uint8_t *initial_display = framebuffer_get_display();
    uint8_t *initial_draw = framebuffer_get_draw();

    framebuffer_swap();

    // Ensure buffers are swapped
    const uint8_t *new_display = framebuffer_get_display();
    uint8_t *new_draw = framebuffer_get_draw();

    printf("initial_display: %p\n", initial_display);
    printf("initial_draw: %p\n", initial_draw);
    printf("new_display: %p\n", new_display);
    printf("new_draw: %p\n", new_draw);

    assert(new_display == initial_display);
    assert(new_draw == initial_draw);
}

void test_framebuffer_commit_swap(void) {
    framebuffer_init();
    mock_set_core(0);  // Run as Core 0

    // Save state before commit
    const uint8_t *initial_display = framebuffer_get_display();

    // Draw to back buffer
    uint8_t *draw_buffer = framebuffer_get_draw();
    memset(draw_buffer, 0xAA, FRAMEBUFFER_WIDTH * FRAMEBUFFER_HEIGHT);

    // Commit
    framebuffer_commit();
    framebuffer_swap();

    const uint8_t *new_display = framebuffer_get_display();
    uint8_t *new_draw = framebuffer_get_draw();
    assert(new_display == draw_buffer);
    assert(new_draw == initial_display);
}

void test_framebuffer_scaled_line(void) {
    framebuffer_init();

    // Draw test pattern
    uint8_t *draw_buffer = framebuffer_get_draw();
    for (int i = 0; i < FRAMEBUFFER_WIDTH; i++) {
        draw_buffer[i] = i & 0xFF;
    }

    framebuffer_commit();
    framebuffer_swap();

    // Get scaled line and verify
    uint16_t line_buffer[FRAMEBUFFER_WIDTH];
    frame_buffer_get_scaled_line(line_buffer, 0);

    assert(line_buffer != NULL);

    // Ensure line is scaled horizontally by 2
    for (int i = 0; i < FRAMEBUFFER_WIDTH; i++) {
        assert(line_buffer[i * 2] == (i & 0xFF));
        assert(line_buffer[i * 2 + 1] == (i & 0xFF));
    }
}

// Main test file function
void run_framebuffer_tests(void) {
    printf("Starting framebuffer tests...\n");

    test_framebuffer_init();
    printf("Initialization test ........... OK\n");

    test_framebuffer_swap();
    printf("Buffer swap test .............. OK\n");

    test_framebuffer_commit_swap();
    printf("Commit test ................... OK\n");

    test_framebuffer_scaled_line();
    printf("Scaling test .................. OK\n");

    printf("All framebuffer tests passed successfully!\n");
}
