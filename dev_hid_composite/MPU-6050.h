#ifndef MPU6050_H__
#define MPU6050_H__

#include "pico/stdlib.h"
#include "hardware/i2c.h"

typedef struct {
    double accel_x; // unit (g)
    double accel_y; // unit (g)
    double accel_z; // unit (g)
    double temp; // unit (deg c)
    double gyro_x; // unit (deg/s)
    double gyro_y; // unit (deg/s)
    double gyro_z; // unit (deg/s)
} imu_data;

extern imu_data imu;

uint8_t MPUcheck();
void MPUsetup();
void MPUread();

#endif