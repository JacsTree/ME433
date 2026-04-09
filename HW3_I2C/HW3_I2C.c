#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define LED_PIN 16

#define ADDR 0x20

//0b for binary?
void setReg(uint8_t reg ,uint8_t value){
    uint8_t buf[2];
    buf[0] = reg;
    buf[1] = value;
    i2c_write_blocking(I2C_PORT, ADDR, buf, 2, false);
}

uint8_t readReg(uint8_t reg){
    uint8_t buf;
    i2c_write_blocking(I2C_PORT, ADDR, &reg, 1, true);  // true to keep host control of bus
    i2c_read_blocking(I2C_PORT, ADDR, &buf, 1, false);  // false - finished with bus
    return buf;
}

uint8_t readPin(uint8_t reg, uint8_t pin){
    uint8_t buf=readReg(reg);
    // use bitwise AND to check if the specific pin is active
    uint8_t mask = 1 << pin;
    return (buf & mask) != 0;//returns 1 if active and 0 if not
}

int main()
{
    stdio_init_all();

    // I2C Initialisation. Using it at 400Khz.
    i2c_init(I2C_PORT, 400*1000);
    
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    // we are using external pullup resistors
    //gpio_pull_up(I2C_SDA);
    //gpio_pull_up(I2C_SCL);

    //write:
    //i2c_write_blocking(i2c_default, ADDR, buf, 2, false);

    // read:
    // i2c_write_blocking(i2c_default, ADDR, &reg, 1, true);  // true to keep host control of bus
    // i2c_read_blocking(i2c_default, ADDR, &buf, 1, false);  // false - finished with bus

    // setup
    setReg(0x00, 0x7F);// set IO direction to read except for GP7 write

    while (true) {
        uint8_t buffer = readPin(0x09,0);
        //printf("char= %d\n",buffer);
        
        if (buffer==0) setReg(0x09,0x80);
        else setReg(0x09,0x00);
        //sleep_ms(1);
    }
}
