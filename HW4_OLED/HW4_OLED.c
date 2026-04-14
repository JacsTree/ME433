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

    i2c_init(I2C_PORT, 400*1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);

    // setup
    sleep_ms(10);
    ssd1306_setup();

    int i = 15;
    char message[50]; 
    sprintf(message, "my var = %d", i); 
 // draw starting at x=20,y=10  
    while (true) {
        draw_message(20,10,message);
        ssd1306_update();
        sleep_ms(500);
        
        ssd1306_clear();
        ssd1306_update();
        sleep_ms(500);
        

    }
}
