#include <hardware/gpio.h>
#include <hardware/timer.h>
#include <pico/time.h>
#include <stdbool.h>
#include <stdio.h>
#include "pico/stdlib.h"

#include "HX711.h"

#define DATA_PIN 16
#define CLOCK_PIN 17

#define count 100

const float A = 0.5; //new data
const float B = 1-A; //old data

int32_t times[count]; // 12.5 sec of data
int32_t counts[count];
int32_t iir[count];

int main()
{
    stdio_init_all();

    while (!stdio_usb_connected())
    {
        sleep_ms(100);
    }
    struct HX711 sensor = HX711_create(CLOCK_PIN, DATA_PIN);

    HX711_tare(&sensor);
    //int32_t count = HX711_read_count(&sensor)-sensor.OFFSET;
    
    

    char buffer[200];

    while (true) {
        // int32_t raw = HX711_read_count(&sensor);
        // printf("raw: %ld\n", raw);
        // sleep_ms(500);
        // count = HX711_read_count(&sensor)-sensor.OFFSET;
        // //printf("count: %d\n", count);
        // sleep_ms(100);

        
        scanf("%s", buffer); // wait for the client to send a command
        switch(buffer[0]){
            case 'r':{
                times[0] = time_us_32()/1000;
                counts[0] = HX711_read_count(&sensor)-sensor.OFFSET;
                iir[0] = counts[0];

                for(int i = 1; i<count; i++){
                    times[i] = time_us_32()/1000 - times[0];
                    counts[i] = HX711_read_count(&sensor)-sensor.OFFSET;
                    iir[i] = A*counts[i]+B*iir[i-1];
                    sleep_us(12500); // 80 hz
                }
                times[0] = 0;

                for(int i = 1; i<count; i++){
                    printf("%d %d %d\n",times[i],counts[i],iir[i]);
                }
            }
            default:
            {
                //fail
            }
        }
    }
}
