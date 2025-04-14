// This file was originally part of picoruby-gpio:
// https://github.com/picoruby/picoruby/blob/5a31913e23c89c8dec8eca52b2d12ff15c42b74c/mrbgems/picoruby-gpio/ports/rp2040/gpio.c
//
// Copyright © 2020 HASUMI Hitoshi
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

#include <stdint.h>
#include <stdbool.h>
#include "hardware/gpio.h"

#include "../../include/gpio.h"

int
GPIO_pin_num_from_char(const uint8_t *str)
{
  /* Not supported */
  return -1;
}

void
GPIO_init(uint8_t pin)
{
  gpio_init(pin);
}

void
GPIO_set_dir(uint8_t pin, uint8_t dir)
{
  switch (dir) {
    case (IN):
      gpio_set_dir(pin, false);
      break;
    case (OUT):
      gpio_set_dir(pin, true);
      break;
    case (HIGH_Z):
      /* HIGH_Z is not supported */
      break;
  }
}

void
GPIO_open_drain(uint8_t pin)
{
  /* Not supported */
}

void
GPIO_pull_up(uint8_t pin)
{
  gpio_pull_up(pin);
}

void
GPIO_pull_down(uint8_t pin)
{
  gpio_pull_down(pin);
}

int
GPIO_read(uint8_t pin)
{
  return gpio_get(pin);
}

void
GPIO_write(uint8_t pin, uint8_t val)
{
  gpio_put(pin, val == 1);
}

void
GPIO_set_function(uint8_t pin, uint8_t function)
{
  gpio_set_function(pin, function);
}

