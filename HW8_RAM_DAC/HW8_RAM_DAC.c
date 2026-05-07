#include "HW8_RAM_DAC.h"

#define PIN_CS 17
#define CS_RAM 13

static inline void init(){
    stdio_init_all();
    spi_init(spi_default, 1000 * 1000 * 1); // the baud, or bits per second
    gpio_set_function(PICO_DEFAULT_SPI_RX_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_SCK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_TX_PIN, GPIO_FUNC_SPI);
    gpio_init(PIN_CS);
    gpio_set_dir(PIN_CS,1);
    gpio_put(PIN_CS,1);
    gpio_init(CS_RAM);
    gpio_set_dir(CS_RAM,1);
    gpio_put(CS_RAM,1);
}

int main()
{
    init();
    sleep_ms(3000);
    uint8_t buffer[3];
    buffer[0] = 0b00000011;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    uint8_t buf[2];

    uint8_t bufferrrr[2000];
    cs_select_ram();
    spi_write_blocking(spi_default, buffer, 3);
    sleep_us(1);
    spi_read_blocking(spi_default,0,buf,2000);
    cs_deselect_ram();

    for(int i = 0; i<1000; i++){
        printf("%x %x\n",bufferrrr[i],bufferrrr[i*2+1]);
    }

    while (true) {
        // cs_select_ram();
        // spi_write_blocking(spi_default, buffer, 3);
        // sleep_us(1);
        // spi_read_blocking(spi_default,0,buf,2);
        // cs_deselect_ram();

        // cs_select_dac();
        // spi_write_blocking(spi_default, buf, 2);
        // cs_deselect_dac();

        // buffer[2]++;
        // if(buffer[2]==0x00){
        //     buffer[1]++;
        //     if(buffer[1]==0x7F){
        //         buffer[1]=0x00;
        //     }
        // }
        sleep_us(1000);
    }
}

static inline void spi_ram_init(){
    //setup sequential
    cs_select_ram();
    uint8_t buft[2];
    buft[0]=0b00000001;
    buft[1] = 0b01000001;
    spi_write_blocking(spi_default, buft, 2);
    cs_deselect_ram();

    uint16_t voltages_1[1000]; //sin wave
    for (int i = 0; i<1000; i++) {
        voltages_1[i] = (uint16_t)(((sinf((2.0f * M_PI * i) / 1000.0f) * 0.5f) + 0.5f) * 1023.0f + 0.5f);
    }
    uint8_t buffer[2003]; //stores 8 bit instructions to be sent (easier)
    buffer[0] = 0b00000010;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    for(int i=0;i<1000;i++){
        buffer[i+3] = 0b00110000|(0b00001111 & (voltages_1[i] >> 6));
        buffer[i*2+4] = (voltages_1[i]<<2)&0b11111100;
    }
    cs_select_ram();
    spi_write_blocking(spi_default, buffer, 2003);
    cs_deselect_ram();
}
static inline void cs_select_dac() {
    asm volatile("nop \n nop \n nop"); // FIXME
    gpio_put(PIN_CS, 0);
    asm volatile("nop \n nop \n nop"); // FIXME
}
static inline void cs_deselect_dac() {
    asm volatile("nop \n nop \n nop"); // FIXME
    gpio_put(PIN_CS, 1);
    asm volatile("nop \n nop \n nop"); // FIXME
}
static inline void cs_select_ram() {
    asm volatile("nop \n nop \n nop"); // FIXME
    gpio_put(PIN_CS, 0);
    asm volatile("nop \n nop \n nop"); // FIXME
}
static inline void cs_deselect_ram() {
    asm volatile("nop \n nop \n nop"); // FIXME
    gpio_put(PIN_CS, 1);
    asm volatile("nop \n nop \n nop"); // FIXME
}
