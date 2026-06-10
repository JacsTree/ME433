#include <stdio.h>
#include "pico/stdlib.h"
#include "AS5600.h"


#define I2C_USED i2c1
#define Data_pin 14
#define Clock_Pin 15



int main()
{
    stdio_init_all();

    i2c_init(I2C_USED, 400*1000);
    gpio_set_function(Data_pin, GPIO_FUNC_I2C);
    gpio_set_function(Clock_Pin, GPIO_FUNC_I2C);
    gpio_pull_up(Data_pin);
    gpio_pull_up(Clock_Pin);

    struct AS5600 encoder = AS5600_create(I2C_USED);
    if(AS5600_isConnected(&encoder)){
        printf("connected\n");
    }
    else return 0;

    AS5600_setOffset(&encoder, 275.0);

    


    while (true) {
        if(!AS5600_magnetDetected(&encoder)){
            printf("Magnet Not Detected!\n");
        }
        else if(AS5600_magnetTooStrong(&encoder)){
            printf("Magnet too close!\n");
        }
        else if(AS5600_magnetTooWeak(&encoder)){
            printf("Magnet too far!\n");
        }
        else {
            printf("Raw angle: %d, Read Angle: %f\n",AS5600_rawAngle(&encoder),AS5600_readAngle(&encoder));
        }

        sleep_ms(100);
    }
}
