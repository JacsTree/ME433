#ifndef AS5600_h
#define AS5600_h

#include <stdint.h>
#include <stdbool.h>

#ifdef PICO_BOARD
#include "hardware/i2c.h"
#endif

#ifdef STM32C092
#include "stm32f4xx_hal.h"
#endif

#define AS5600_DEFAULT_ADDRESS  0x36

struct AS5600{
    i2c_inst_t *i2c; 
    uint8_t SDA;
    uint8_t SCL;
    uint16_t offset;
};

struct AS5600 AS5600_create(uint8_t SDA_PIN, uint8_t SCL_PIN, i2c_inst_t *address);

bool AS5600_isConnected(struct AS5600 *enc);

uint16_t AS5600_rawAngle(struct AS5600 *encoder);
float AS5600_readAngle(struct AS5600 *encoder);

void AS5600_setOffset(struct AS5600 *encoder, float degrees);

bool AS5600_magnetDetected(struct AS5600 *encoder);
bool AS5600_magnetTooStrong(struct AS5600 *encoder);
bool AS5600_magnetTooWeak(struct AS5600 *encoder);

#endif
