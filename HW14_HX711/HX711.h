#ifndef HX711_h
#define HX711_h

#include <stdint.h>
#include <stdbool.h>

struct HX711{
    uint8_t CLOCK_PIN; //Clock pin
    uint8_t DATA_PIN; //Data Pin
    uint8_t GAIN; // HX711 Gain
    int32_t OFFSET; // Base reading at zero
    float SCALE; // Scale in g/count
};

/// Creates HX711 struct
struct HX711 HX711_create(uint8_t clk, uint8_t data);

/// Set gain value
void HX711_set_gain(struct HX711* sensor, uint8_t gain);

/// Set offset value
void HX711_set_offset(struct HX711* sensor, int32_t offset);

/// Set scale value
void HX711_set_scale(struct HX711* sensor, float scale);

/// Reads HX711 raw count value
int32_t HX711_read_count(struct HX711* sensor);

int32_t HX711_read_count_avg(struct HX711* sensor, int N);

/// Tares HX711
int32_t HX711_tare(struct HX711* sensor);

/// Reads HX711 in grams
float HX711_read_grams(struct HX711* sensor);

float HX711_read_grams_avg(struct HX711* sensor, int N);

/// Returns true if data is ready to be read
bool HX711_ready(struct HX711* sensor);

/// Reads current offset value
int32_t HX711_read_offset(struct HX711* sensor);

#endif