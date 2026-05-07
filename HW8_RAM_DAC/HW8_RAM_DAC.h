#ifndef HW8_RAM_DAC_H_
#define HW8_RAM_DAC_H_

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/dma.h"
#include "math.h"

/**
 * Writes to the DAC
 * @param adc_select (0 or 1)
 * @param voltage (0-1023)
 */
static inline void put_dac(uint8_t adc_select, uint16_t voltage);

/// @brief  Initilizes spi ram with 2000 bytes of sin data
static inline void spi_ram_init();
/// DAC Chip Select
static inline void cs_select_dac();
/// DAC Chip Deselect
static inline void cs_deselect_dac();
/// RAM Chip Select
static inline void cs_select_ram();
/// RAM Chip De-select
static inline void cs_deselect_ram();

#endif