#include "HX711.h"
#include <hardware/gpio.h>
#include <stdint.h>
#include "pico/stdlib.h"

// Private:
static inline void hx711_clock_pulse(uint8_t clk_pin) {
    gpio_put(clk_pin, 1);
    sleep_us(1);
    gpio_put(clk_pin, 0);
    sleep_us(1);
}

struct HX711 HX711_create(uint8_t clk, uint8_t data){
    struct HX711 sensor;
    sensor.CLOCK_PIN = clk;
    sensor.DATA_PIN = data;
    sensor.GAIN = 128;
    sensor.OFFSET = 0;
    sensor.SCALE = 0.0;

    gpio_init(sensor.DATA_PIN);
    gpio_init(sensor.CLOCK_PIN);
    gpio_set_dir(sensor.DATA_PIN, false);
    gpio_set_dir(sensor.CLOCK_PIN, true);
    return sensor;
}

/// Set gain value
void HX711_set_gain(struct HX711* sensor, uint8_t gain){
    sensor->GAIN = gain;
    //TODO: set gain for 1st read if not 128
}

/// Set offset value
void HX711_set_offset(struct HX711* sensor, int32_t offset){
    sensor->OFFSET = offset;
}

/// Set scale value
void HX711_set_scale(struct HX711* sensor, float scale){
    sensor->SCALE = scale;
}

/// Reads HX711 raw count value (blocking)
int32_t HX711_read_count(struct HX711 *sensor){
    uint32_t data = 0;

    while (gpio_get(sensor->DATA_PIN)) {
        tight_loop_contents();
    }
    for (int i = 0; i < 24; i++) {
        hx711_clock_pulse(sensor->CLOCK_PIN);

        data <<= 1; //shift left

        if (gpio_get(sensor->DATA_PIN)) {
            data++;
        }
    } 
    int g;
    switch (sensor->GAIN){
        case 128:
            g=1;
        case 64:
            g=3;
        case 32:
            g=2;
        default:
            g=1; //128
    }
    for (int i = 0; i < g; i++) {
        hx711_clock_pulse(sensor->CLOCK_PIN);
    }

    if (data & 0x800000) {
        data |= 0xFF000000;
    }//convert to signed 32 bit
    return (int32_t)data;
}

/// Reads HX711 raw count value many times and average (blocking)
int32_t HX711_read_count_avg(struct HX711* sensor, int N){
    int32_t sum = 0;

    for (int i = 0; i < N; i++) {
        sum += HX711_read_count(sensor);
    }//this overflows of you try more than a couple hundred, dont
    return (int32_t)(sum / N);
}

/// Tares HX711
int32_t HX711_tare(struct HX711* sensor){
    sensor->OFFSET=HX711_read_count_avg(sensor, 10); //10 is decent and fairy quick
    return sensor->OFFSET;
}

/// Reads HX711 in grams
float HX711_read_grams(struct HX711* sensor){
    int32_t counts = HX711_read_count(sensor);
    return (counts-sensor->OFFSET)*sensor->SCALE;
}

float HX711_read_grams_avg(struct HX711* sensor, int N){
    int32_t counts = HX711_read_count_avg(sensor, N);
    return (counts-sensor->OFFSET)*sensor->SCALE;
}

/// Returns true if data is ready to be read
bool HX711_ready(struct HX711* sensor){
    return !gpio_get(sensor->DATA_PIN);
}

/// Reads current offset value
int32_t HX711_read_offset(struct HX711* sensor){
    return sensor->OFFSET;
}