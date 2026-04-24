#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "math.h"

#define PIN_CS 17

static inline void cs_select() {
    asm volatile("nop \n nop \n nop"); // FIXME
    gpio_put(PIN_CS, 0);
    asm volatile("nop \n nop \n nop"); // FIXME
}

static inline void cs_deselect() {
    asm volatile("nop \n nop \n nop"); // FIXME
    gpio_put(PIN_CS, 1);
    asm volatile("nop \n nop \n nop"); // FIXME
}

/**
 * Writes to the DAC
 * @param adc_select (0 or 1)
 * @param voltage (0-1023)
 */
static inline void put_dac(uint8_t adc_select, uint16_t voltage){
    uint8_t buf[2];
    buf[0] = (0b10000000&((adc_select)<<7))|0b00110000|(0b00001111 & (voltage >> 6));
    buf[1] = (voltage<<2)&0b11111100;
    cs_select();
    spi_write_blocking(spi_default, buf, 2);
    cs_deselect();
}

int main()
{
    stdio_init_all();
    spi_init(spi_default, 1000 * 1000 * 50); // the baud, or bits per second
    gpio_set_function(PICO_DEFAULT_SPI_RX_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_SCK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_TX_PIN, GPIO_FUNC_SPI);
    gpio_init(PIN_CS);
    gpio_set_dir(PIN_CS,1);
    gpio_put(PIN_CS,1);

    uint16_t voltages_0[400];//could do more samples but the plots look good
    uint16_t voltages_1[400];
    for(int i = 0; i<200;i++){
        voltages_0[i]=(uint16_t)(1023.9999*(i/199.0));
        voltages_0[399-i]=(uint16_t)(1023.9999*(i/199.0));
    }
    for (int i = 0; i<400; i++) {
        voltages_1[i] = (uint16_t)(((sinf((2.0f * M_PI * i) / 400.0f) * 0.5f) + 0.5f) * 1023.0f + 0.5f);
    }

    int idx0 = 0, idx1 = 0;
    while (true) {
        put_dac(0, voltages_0[idx0]);
        put_dac(1, voltages_1[idx1]);
        sleep_us(2500);
        idx0 = (idx0 + 1) % 400;
        idx1 = (idx1 + 2) % 400;
    }
}
