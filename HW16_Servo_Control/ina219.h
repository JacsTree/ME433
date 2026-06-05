// function prototypes for the INA219 current sensor
// TODO: this does not conform with my standards
#ifndef INA219_H
#define INA219_H

#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/pwm.h"

void init_ina219();
float read_ina219();

#endif