//#include <pico/platform/common.h>
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
#include "hardware/sync.h"

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

volatile float angle;
struct AS5600 encoder;

#define LENGTH 400

volatile int state = 0;

volatile float kp = 0.00082;
volatile float ki = 0.006;

volatile float desired_current = 150.0f;
volatile float actual_current = 0.0f;

volatile float current_log[LENGTH];
volatile float desired_log[LENGTH];
volatile int index_log[LENGTH];

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

// !! This kills all power to the motor if it goes outside of the safe range !!
bool safety_check(void){
    raw_adc = adc_read();

    if(raw_adc < 550 || raw_adc > (4095-1090)){
        pwm_set_chan_level(slice_num, PWM_CHAN_A, 7499);
        pwm_set_chan_level(slice_num, PWM_CHAN_B, 7499);
        return false;
    }

    return true;
}



bool repeating_timer_callback(__unused struct repeating_timer *t)
{
    static volatile int counter = 0;
    static volatile float eint = 0.0f;

    if(!safety_check()){//kill everything
        state = 0;
        counter = 0;
        eint = 0;
        return true;
    }

    if(state == 1){

        actual_current = -1*read_ina219();

        float error = desired_current - actual_current;

        eint += error * 0.001f;

        float u = kp*error + ki*eint;

        set_duty_cycle(u);

        index_log[counter] = counter;
        desired_log[counter] = desired_current;
        current_log[counter] = actual_current;

        counter++;

        if(counter == 100){
            desired_current = -desired_current;
        }

        if(counter == 200){
            desired_current = -desired_current;
        }

        if(counter == 300){
            desired_current = -desired_current;
        }

        if(counter >= 400){
            pwm_set_chan_level(slice_num, PWM_CHAN_A, 7499);
            pwm_set_chan_level(slice_num, PWM_CHAN_B, 7499);

            state = 0;
            counter = 0;
            eint = 0;
            
            desired_current = fabsf(desired_current);
        }
    }

    return true;
}


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

    // struct HX711 sensor = HX711_create(CLOCK_PIN, DATA_PIN);
    // HX711_tare(&sensor);
    // HX711_set_scale(&sensor, GRAMS_PER_COUNT);

    init_ina219();
    init_hbridge();

    while (!stdio_usb_connected())
    {
        sleep_ms(100);
    }
    //init_ina219();

    // encoder = AS5600_create(I2C_USED);
    // if(AS5600_isConnected(&encoder)){
    //     printf("connected\n");
    // }
    // else return 0;

    // AS5600_setOffset(&encoder, 275.0);

    add_repeating_timer_ms(-1, repeating_timer_callback, NULL, &upd_timer);

    while (true) {
        int c = getchar_timeout_us(0);

        if(c == 'a')
        {
            state = 1;

            while(state == 1)
            {
                tight_loop_contents();
            }

            for(int i=0;i<LENGTH;i++)
            {
                printf("%d,%f,%f\n",
                    index_log[i],
                    desired_log[i],
                    current_log[i]);
            }

            printf("DONE\n");
        }
        //tight_loop_contents();
        // HX711
        //raw_adc = adc_read();
        // sleep_ms(300);

        // int32_t raw = HX711_read_count(&sensor);
        // printf("raw: %ld\n", raw);

        // uint32_t irq_state = save_and_disable_interrupts();
        //float grams = HX711_read_grams(&sensor);
        // restore_interrupts(irq_state);
        // printf("grams: %f\n", grams);

        //sleep_ms(100);
        
        
        // else if(AS5600_magnetTooStrong(&encoder)){
        //     printf("Magnet too close!\n");
        // }
        // else if(AS5600_magnetTooWeak(&encoder)){
        //     printf("Magnet too far!\n");
        // }
    
            // printf("Raw angle: %d, Read Angle: %f\n",AS5600_rawAngle(&encoder),AS5600_readAngle(&encoder));
            // printf("grams: %f\n", grams);
            // printf("Raw ADC: %d\n",raw_adc);
            // printf("Current: %f",read_ina219());


        // user assist
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

        // set_duty_cycle(0.4);
        // sleep_ms(200);
        // set_duty_cycle(0);
        // sleep_ms(50);
        // set_duty_cycle(-0.4);
        // sleep_ms(200);
        // set_duty_cycle(0);

        
        
    }
}
