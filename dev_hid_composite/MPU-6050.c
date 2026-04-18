#include "MPU-6050.h"

#define I2C_PORT i2c1
#define ADDR 0x68

//--- here b/c main does not need these --
// config registers
#define CONFIG 0x1A
#define GYRO_CONFIG 0x1B
#define ACCEL_CONFIG 0x1C
#define PWR_MGMT_1 0x6B
#define PWR_MGMT_2 0x6C
// sensor data registers:
#define ACCEL_XOUT_H 0x3B
#define ACCEL_XOUT_L 0x3C
#define ACCEL_YOUT_H 0x3D
#define ACCEL_YOUT_L 0x3E
#define ACCEL_ZOUT_H 0x3F
#define ACCEL_ZOUT_L 0x40
#define TEMP_OUT_H   0x41
#define TEMP_OUT_L   0x42
#define GYRO_XOUT_H  0x43
#define GYRO_XOUT_L  0x44
#define GYRO_YOUT_H  0x45
#define GYRO_YOUT_L  0x46
#define GYRO_ZOUT_H  0x47
#define GYRO_ZOUT_L  0x48
#define WHO_AM_I     0x75

imu_data imu = {0};

void setReg(uint8_t reg ,uint8_t value){
    uint8_t buf[2];
    buf[0] = reg;
    buf[1] = value;
    i2c_write_blocking(I2C_PORT, ADDR, buf, 2, false);
}

uint8_t readReg(uint8_t reg){
    uint8_t buf;
    i2c_write_blocking(I2C_PORT, ADDR, &reg, 1, true);  // true to keep host control of bus
    i2c_read_blocking(I2C_PORT, ADDR, &buf, 1, false);  // false - finished with bus
    return buf;
}

uint8_t MPUcheck(){
    return readReg(WHO_AM_I);
}

void MPUsetup(){
    setReg(PWR_MGMT_1,0x00);
    setReg(PWR_MGMT_2,0x00);
    setReg(ACCEL_CONFIG,0x00);
    setReg(GYRO_CONFIG,0b00011000);
}

void MPUread(){
    uint8_t buf[14];
    uint8_t reg = ACCEL_XOUT_H;
    i2c_write_blocking(I2C_PORT, ADDR, &reg, 1, true);
    i2c_read_blocking(I2C_PORT, ADDR, buf, 14, false);
    int16_t sens[7];
    for(int i = 0; i<7;i++){
        sens[i] = (buf[i*2]<<8)|buf[i*2+1];
    }
    imu.accel_x=(double)sens[0]*0.000061;
    imu.accel_y=(double)sens[1]*0.000061;
    imu.accel_z=(double)sens[2]*0.000061;
    imu.temp=((double)sens[3]/340)+36.53;
    imu.gyro_x=(double)sens[4]*0.0609756;
    imu.gyro_y=(double)sens[5]*0.0609756;
    imu.gyro_z=(double)sens[6]*0.0609756;
}
