#include <pico/time.h>
#include <stdint.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
//#include "ina219.h"
#include "hardware/i2c.h"
#include "AS5600.h"
#include "HX711.h"
#include "ina219.h"
#include <math.h>

#define DATA_PIN 16
#define CLOCK_PIN 17

#define GRAMS_PER_COUNT -0.01 // TODO: update


#define I2C_USED i2c1
#define Data_pin 14
#define Clock_Pin 15

#define IN1 18
#define IN2 19

uint slice_num;

struct repeating_timer upd_timer;

volatile uint16_t raw_adc;

void init_hbridge(){
    gpio_set_function(IN1, GPIO_FUNC_PWM);
    gpio_set_function(IN2, GPIO_FUNC_PWM);
    slice_num = pwm_gpio_to_slice_num(IN1);
    pwm_set_wrap(slice_num, 7499);
    pwm_set_chan_level(slice_num, PWM_CHAN_A, 7499); //16
    pwm_set_chan_level(slice_num, PWM_CHAN_B, 7499); //17
    pwm_set_enabled(slice_num, true);
}

void set_duty_cycle(float duty_cycle){ // -1 -> 1
    if (duty_cycle>1)duty_cycle = 1;
    if (duty_cycle<-1)duty_cycle = -1;

    if (duty_cycle>0){
        pwm_set_chan_level(slice_num, PWM_CHAN_A, 7499);
        pwm_set_chan_level(slice_num, PWM_CHAN_B, (int)((1.0-duty_cycle)*7499));
    }
    else{
        pwm_set_chan_level(slice_num, PWM_CHAN_B, 7499);
        pwm_set_chan_level(slice_num, PWM_CHAN_A, (int)((1.0+duty_cycle)*7499));
    }
}

// makes sure the arm does not rip itself apart
void safety_check(){
    raw_adc = adc_read();
    if(raw_adc>3000||raw_adc<550){
        pwm_set_chan_level(slice_num, PWM_CHAN_A, 7499);
        pwm_set_chan_level(slice_num, PWM_CHAN_B, 7499);
    }
}

bool repeating_timer_callback(__unused struct repeating_timer *t){
    safety_check();
    return true;
}

//TODO: putting i2c n inturrupt breaks (use 2 different i2c or update both at same time in loop)
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

    struct HX711 sensor = HX711_create(CLOCK_PIN, DATA_PIN);
    HX711_tare(&sensor);
    HX711_set_scale(&sensor, GRAMS_PER_COUNT);

    init_ina219();
    init_hbridge();

    while (!stdio_usb_connected())
    {
        sleep_ms(100);
    }
    //init_ina219();

    struct AS5600 encoder = AS5600_create(I2C_USED);
    if(AS5600_isConnected(&encoder)){
        printf("connected\n");
    }
    else return 0;

    AS5600_setOffset(&encoder, 275.0);

    add_repeating_timer_ms(-1, repeating_timer_callback, NULL, &upd_timer);

    while (true) {
        // HX711
        //raw_adc = adc_read();
        // sleep_ms(300);

        // int32_t raw = HX711_read_count(&sensor);
        // printf("raw: %ld\n", raw);
        float grams = HX711_read_grams(&sensor);
        // printf("grams: %f\n", grams);

        //sleep_ms(100);
        if(!AS5600_magnetDetected(&encoder)){
            printf("Magnet Not Detected!\n");
        }
        // else if(AS5600_magnetTooStrong(&encoder)){
        //     printf("Magnet too close!\n");
        // }
        // else if(AS5600_magnetTooWeak(&encoder)){
        //     printf("Magnet too far!\n");
        // }
        else{
            // printf("Raw angle: %d, Read Angle: %f\n",AS5600_rawAngle(&encoder),AS5600_readAngle(&encoder));
            // printf("grams: %f\n", grams);
            // printf("Raw ADC: %d\n",raw_adc);
            // printf("Current: %f",read_ina219());


            // user assist arm mode
            // if(fabs(grams)>60){
            //     bool dir = false; // right
            //     if(grams<0) dir = true;

            //     float bgrams = fabs(grams);
            //     bgrams-=60;

            //     bgrams*=0.005;

            //     if(!dir) {
            //         bgrams*=-0.7;
            //     }

            //     set_duty_cycle(bgrams);
            // }
            // else{
            //     set_duty_cycle(0);
            // }

            // toggle switch effect

            float angle = AS5600_readAngle(&encoder);
            if(angle<80&&angle>75){
                if(fabs(grams)>50){
                    set_duty_cycle(-0.4);

                }
                else{
                    set_duty_cycle(0);
                }
            }
            else if(angle<85&&angle>80){
                if(fabs(grams)>50){
                    set_duty_cycle(0.2);
                }
                else{
                    set_duty_cycle(0);
                }
            }
            else if(angle<135&&angle>130){
                if(fabs(grams)>50){
                    set_duty_cycle(-0.4);
                }
                else{
                    set_duty_cycle(0);
                }
            }
            else if(angle<140&&angle>135){
                if(fabs(grams)>50){
                    set_duty_cycle(0.2);
                }
                else{
                    set_duty_cycle(0);
                }
            }
            else{ // user assist
                set_duty_cycle(0);
                // if(fabs(grams)>60){
                //     bool dir = false; // right
                //     if(grams<0) dir = true;

                //     float bgrams = fabs(grams);
                //     bgrams-=60;

                //     bgrams*=0.005;

                //     if(!dir) {
                //         bgrams*=-0.7;
                //     }

                //     set_duty_cycle(bgrams);
                // }
                // else{
                //     set_duty_cycle(0);
                // }
            }

            // set_duty_cycle(0.4);
            // sleep_ms(200);
            // set_duty_cycle(0);
            // sleep_ms(50);
            // set_duty_cycle(-0.4);
            // sleep_ms(200);
            // set_duty_cycle(0);

        }
        
    }
}
