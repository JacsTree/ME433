#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "ssd1306.h"
#include "font.h"

#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15

void draw_letter(int x, int y, char c){
    if (c < 32 || c > 127) return;
        
    for (int i = 0; i<=4;i++){
        for (int j =  0; j < 8; j++){
            if ((ASCII[c-32][i]>>j)&1) 
                ssd1306_drawPixel(i+x,y+j,1);
            else 
                ssd1306_drawPixel(i+x,y+j,0);
        }
    }   
}

int main()
{
    stdio_init_all();

    i2c_init(I2C_PORT, 400*1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);

    // setup
    sleep_ms(10);
    ssd1306_setup();

    while (true) {
        draw_letter(10,20,'j');
        ssd1306_update();
        sleep_ms(500);
        
        ssd1306_clear();
        ssd1306_update();
        sleep_ms(500);
        

    }
}
