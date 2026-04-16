#include <stdio.h>
#include "pico/stdlib.h"
#include "MPU-6050.h"
#include "ssd1306.h"

#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15


int main()
{
    stdio_init_all();

    i2c_init(I2C_PORT, 100*1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);

    sleep_ms(1000);

    // setup
    sleep_ms(10);
    printf("0x%x\n",MPUcheck());
    MPUsetup();
    ssd1306_setup();

    // while (true) {
    //     MPUread();
    //     printf("accel_x: %f\n", imu.accel_x);
    //     printf("accel_y: %f\n", imu.accel_y);
    //     printf("accel_z: %f\n", imu.accel_z);
    //     printf("temp: %f\n", imu.temp);
    //     printf("gyro_x: %f\n", imu.gyro_x);
    //     printf("gyro_y: %f\n", imu.gyro_y);
    //     printf("gyro_z: %f\n", imu.gyro_z);
    //     sleep_ms(10);
    // }
    while(true){//64 16
        MPUread();
        int x = (int)(-1*imu.accel_x*64+0.5)+64;
        int y = (int)(imu.accel_y*16+0.5)+16;
        for (int i=0;i<=128;i++){
            if ((x<=i&&i<=64)||(64<=i&&i<=x)) ssd1306_drawPixel(i,16,1);
            else ssd1306_drawPixel(i,16,0);
        }
        for (int i=0;i<=32;i++){
            if ((y<=i&&i<=16)||(16<=i&&i<=y)) ssd1306_drawPixel(64,i,1);
            else ssd1306_drawPixel(64,i,0);
        }
        ssd1306_update();
        
    }

    
}
