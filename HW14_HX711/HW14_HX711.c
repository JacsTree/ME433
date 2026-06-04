#include <hardware/gpio.h>
#include <stdbool.h>
#include <stdio.h>
#include "pico/stdlib.h"

#include "HX711.h"

#define DATA_PIN 14
#define CLOCK_PIN 15

#define GRAMS_PER_COUNT -0.01 // TODO: update


int main()
{
    stdio_init_all();

    struct HX711 sensor = HX711_create(CLOCK_PIN, DATA_PIN);

    HX711_tare(&sensor);
    HX711_set_scale(&sensor, GRAMS_PER_COUNT);
    
    while (true) {
        // int32_t raw = HX711_read_count(&sensor);
        // printf("raw: %ld\n", raw);
        // sleep_ms(500);
        float grams = HX711_read_grams_avg(&sensor,20);
        printf("grams: %f\n", grams);
        sleep_ms(500);
    }
}
