#include <mruby.h>
#include <mruby/data.h>
#include <mruby/class.h>
#include <mruby/variable.h>
#include <mruby/error.h>
#include "../include/gpio.h"

typedef struct
{
  uint pin;
  uint mode;
} mrb_gpio_data;

static struct RClass *mrb_gpio_class = NULL;

static void mrb_gpio_free(mrb_state *mrb, void *p)
{
  mrb_gpio_data *data = (mrb_gpio_data *)p;
  if (data != NULL)
  {
    mrb_free(mrb, data);
  }
}

static const struct mrb_data_type mrb_gpio_type = {
    "GPIO", mrb_gpio_free};

static mrb_value mrb_gpio_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_gpio_data *data;
  mrb_int pin;
  mrb_int mode;

  data = (mrb_gpio_data *)DATA_PTR(self);
  if (data)
  {
    mrb_free(mrb, data);
  }
  DATA_TYPE(self) = &mrb_gpio_type;
  DATA_PTR(self) = NULL;

  mrb_get_args(mrb, "ii", &pin, &mode);

  data = (mrb_gpio_data *)mrb_malloc(mrb, sizeof(mrb_gpio_data));
  data->pin = (uint)pin;
  data->mode = (uint)mode;

  // Initialize GPIO
  GPIO_init(data->pin);

  // Set GPIO direction and mode
  switch (mode) {
    case IN:
    case OUT:
      GPIO_set_dir(data->pin, mode);
      break;
    case PULL_UP:
      GPIO_set_dir(data->pin, IN);
      GPIO_pull_up(data->pin);
      break;
    case PULL_DOWN:
      GPIO_set_dir(data->pin, IN);
      GPIO_pull_down(data->pin);
      break;
    case OPEN_DRAIN:
      GPIO_set_dir(data->pin, OUT);
      GPIO_open_drain(data->pin);
      break;
    case ALT:
      GPIO_set_function(data->pin, mode);
      break;
    default:
      mrb_raise(mrb, E_ARGUMENT_ERROR, "invalid GPIO mode");
  }

  DATA_PTR(self) = data;
  return self;
}

static mrb_value mrb_gpio_read(mrb_state *mrb, mrb_value self)
{
  mrb_gpio_data *data = DATA_GET_PTR(mrb, self, &mrb_gpio_type, mrb_gpio_data);
  return mrb_fixnum_value(GPIO_read(data->pin));
}

static mrb_value mrb_gpio_write(mrb_state *mrb, mrb_value self)
{
  mrb_gpio_data *data = DATA_GET_PTR(mrb, self, &mrb_gpio_type, mrb_gpio_data);
  mrb_int value;

  mrb_get_args(mrb, "i", &value);

  if (value != 0 && value != 1)
  {
    mrb_raise(mrb, E_RANGE_ERROR, "GPIO value must be 0 or 1");
  }

  GPIO_write(data->pin, (uint8_t)value);
  return mrb_nil_value();
}

void mrb_mruby_pico_gpio_gem_init(mrb_state *mrb)
{
  mrb_gpio_class = mrb_define_class(mrb, "GPIO", mrb->object_class);
  MRB_SET_INSTANCE_TT(mrb_gpio_class, MRB_TT_DATA);

  mrb_define_const(mrb, mrb_gpio_class, "IN", mrb_fixnum_value(IN));
  mrb_define_const(mrb, mrb_gpio_class, "OUT", mrb_fixnum_value(OUT));
  mrb_define_const(mrb, mrb_gpio_class, "HIGH_Z", mrb_fixnum_value(HIGH_Z));
  mrb_define_const(mrb, mrb_gpio_class, "PULL_UP", mrb_fixnum_value(PULL_UP));
  mrb_define_const(mrb, mrb_gpio_class, "PULL_DOWN", mrb_fixnum_value(PULL_DOWN));
  mrb_define_const(mrb, mrb_gpio_class, "OPEN_DRAIN", mrb_fixnum_value(OPEN_DRAIN));
  mrb_define_const(mrb, mrb_gpio_class, "ALT", mrb_fixnum_value(ALT));

  mrb_define_method(mrb, mrb_gpio_class, "initialize", mrb_gpio_initialize, MRB_ARGS_REQ(2));
  mrb_define_method(mrb, mrb_gpio_class, "read", mrb_gpio_read, MRB_ARGS_NONE());
  mrb_define_method(mrb, mrb_gpio_class, "write", mrb_gpio_write, MRB_ARGS_REQ(1));
}

void mrb_mruby_pico_gpio_gem_final(mrb_state *mrb) {}
