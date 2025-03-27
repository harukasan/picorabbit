#include <mruby.h>
#include <mruby/data.h>
#include <mruby/numeric.h>
#include "../include/memory_usage.h"


// Get total heap size
static mrb_value mrb_pico_total_heap(mrb_state *mrb, mrb_value self) {
    return mrb_fixnum_value(pico_total_heap());
}

// Get used heap size
static mrb_value mrb_pico_used_heap(mrb_state *mrb, mrb_value self) {
    return mrb_fixnum_value(pico_used_heap());
}

// Get free heap size
static mrb_value mrb_pico_free_heap(mrb_state *mrb, mrb_value self) {
    return mrb_fixnum_value(pico_free_heap());
}

void mrb_mruby_pico_memory_usage_gem_init(mrb_state *mrb) {
    struct RClass *memory_module;
    memory_module = mrb_define_module(mrb, "PicoMemoryUsage");

    mrb_define_module_function(mrb, memory_module, "total_heap", mrb_pico_total_heap, MRB_ARGS_NONE());
    mrb_define_module_function(mrb, memory_module, "used_heap", mrb_pico_used_heap, MRB_ARGS_NONE());
    mrb_define_module_function(mrb, memory_module, "free_heap", mrb_pico_free_heap, MRB_ARGS_NONE());
}

void mrb_mruby_pico_memory_usage_gem_final(mrb_state *mrb) {
    // Cleanup code if needed
}
