#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/adc.h"
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

void draw_message(int x, int y, char *str){
    int counter = 0;
    while(str[counter]!='\0'){
        draw_letter(x+counter*5,y,str[counter]);
        counter++;
    }
}

int main()
{
    stdio_init_all();

    i2c_init(I2C_PORT, 2100*1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    adc_init();
    adc_gpio_init(26);
    adc_select_input(0);

    // setup
    sleep_ms(10);
    ssd1306_setup();

    unsigned int time1 = to_us_since_boot(get_absolute_time());
    float frames = 0;
    while (true) {
        float result = (adc_read() / (float)4095) * 3.3;
        char message[50]; 
        sprintf(message, "Voltage = %.2f", result); 
        draw_message(0, 0, message);

        unsigned int time2 = to_us_since_boot(get_absolute_time());
        frames++;

        if (time2 - time1 > 500000) {
            float elapsed_time = (time2 - time1) / 1000000.0;
            float fps = frames / elapsed_time; 
            time1 = time2;
            frames = 0;
            
            char message2[50]; 
            sprintf(message2, "FPS = %.2f", fps); 
            draw_message(0, 25, message2);
        }

        ssd1306_update();
    }
}
