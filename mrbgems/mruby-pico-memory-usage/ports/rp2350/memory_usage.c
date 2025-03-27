#include "../../include/memory_usage.h"
#include "pico/stdlib.h"
#include <malloc.h>

// Memory region boundary symbols defined in linker script
extern char __StackLimit;
extern char __bss_end__;

int pico_total_heap() {
    // difference between stack limit and BSS end
    return (int)(&__StackLimit - &__bss_end__);
}

int pico_used_heap() {
    struct mallinfo info = mallinfo();
    return info.uordblks;  // heap memory in use
}

int pico_free_heap() {
    struct mallinfo info = mallinfo();
    return pico_total_heap() - info.uordblks;
}

