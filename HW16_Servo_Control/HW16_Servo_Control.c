#include <pico/time.h>
#include <stdint.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
//#include "ina219.h"
#include "hardware/i2c.h"
#include "AS5600.h"

#define I2C_USED i2c0
#define Data_pin 16
#define Clock_Pin 17

int main()
{
    stdio_init_all();
    adc_init();
    adc_gpio_init(26);//adc 0
    adc_select_input(0);

    i2c_init(I2C_USED, 400*1000);
    gpio_set_function(Data_pin, GPIO_FUNC_I2C);
    gpio_set_function(Clock_Pin, GPIO_FUNC_I2C);
    gpio_pull_up(Data_pin);
    gpio_pull_up(Clock_Pin);

    while (!stdio_usb_connected())
    {
        sleep_ms(100);
    }
    //init_ina219();

    struct AS5600 encoder = AS5600_create(Data_pin, Clock_Pin, I2C_USED);
    if(AS5600_isConnected(&encoder)){
        printf("connected\n");
    }
    else return 0;
    
    AS5600_setOffset(&encoder, 10.0);

    while (true) {
        // HX711
        // uint16_t raw_adc = adc_read();
        // printf("Raw ADC:%d\n",raw_adc);
        // sleep_ms(300);

        sleep_ms(100);
        if(!AS5600_magnetDetected(&encoder)){
            printf("Magnet Not Detected!\n");
        }
        else if(AS5600_magnetTooStrong(&encoder)){
            printf("Magnet too close!\n");
        }
        else if(AS5600_magnetTooWeak(&encoder)){
            printf("Magnet too far!\n");
        }
        else{
            printf("Raw angle: %d, Read Angle: %f\n",AS5600_rawAngle(&encoder),AS5600_readAngle(&encoder));
        }
        
    }
}
