#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

// I2C defines
// This example will use I2C0 on GPIO8 (SDA) and GPIO9 (SCL) running at 400KHz.
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define LED_PIN 16

#define ADDR 0x20


void setReg(uint8_t reg ,uint8_t value){
    uint8_t buf[2];
    buf[0] = reg;
    buf[1] = value;
    i2c_write_blocking(I2C_PORT, ADDR, buf, 2, false);
}

unsigned char readPin(unsigned char address, unsigned char register){

}

int main()
{
    stdio_init_all();

    // I2C Initialisation. Using it at 400Khz.
    i2c_init(I2C_PORT, 400*1000);
    
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    //gpio_pull_up(I2C_SDA);
    //gpio_pull_up(I2C_SCL);
    // For more examples of I2C use see https://github.com/raspberrypi/pico-examples/tree/master/i2c

    //write:
    //i2c_write_blocking(i2c_default, ADDR, buf, 2, false);

    // read:
    // i2c_write_blocking(i2c_default, ADDR, &reg, 1, true);  // true to keep host control of bus
    // i2c_read_blocking(i2c_default, ADDR, &buf, 1, false);  // false - finished with bus

    // setup
    setReg(0x00, 0x7F);// set IO direction to read except for GP7 write

    setReg(0x09,0x80);// set GP7 to high

    while (true) {
        //printf("Hello, world!\n");
        
        sleep_ms(1000);
    }
}
