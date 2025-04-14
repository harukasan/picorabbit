#ifndef MRB_GPIO_H
#define MRB_GPIO_H

#include <stdint.h>

#define IN 0
#define OUT 1
#define HIGH_Z 2
#define PULL_UP 3
#define PULL_DOWN 4
#define OPEN_DRAIN 5
#define ALT 6

int GPIO_pin_num_from_char(const uint8_t *str);
void GPIO_init(uint8_t pin);
void GPIO_set_dir(uint8_t pin, uint8_t dir);
void GPIO_open_drain(uint8_t pin);
void GPIO_pull_up(uint8_t pin);
void GPIO_pull_down(uint8_t pin);
int GPIO_read(uint8_t pin);
void GPIO_write(uint8_t pin, uint8_t val);
void GPIO_set_function(uint8_t pin, uint8_t function);

#endif // MRB_GPIO_H

