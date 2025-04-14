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

#ifndef GPIO_DEFINED_H_
#define GPIO_DEFINED_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define IN         0b0000001
#define OUT        0b0000010
#define HIGH_Z     0b0000100
#define PULL_UP    0b0001000
#define PULL_DOWN  0b0010000
#define OPEN_DRAIN 0b0100000
#define ALT        0b1000000

int GPIO_pin_num_from_char(const uint8_t *);

void GPIO_init(uint8_t);
void GPIO_set_dir(uint8_t, uint8_t);
void GPIO_pull_up(uint8_t);
void GPIO_pull_down(uint8_t);
void GPIO_open_drain(uint8_t);
int GPIO_read(uint8_t);
void GPIO_write(uint8_t, uint8_t);
void GPIO_set_function(uint8_t, uint8_t);

#ifdef __cplusplus
}
#endif

#endif /* GPIO_DEFINED_H_ */
