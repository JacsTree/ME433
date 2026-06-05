#include "AS5600.h"
#include "hardware/i2c.h"
#include <stdint.h>
#include "math.h"

// ------------------- Registers ------------------- //

//  OUTPUT REGISTERS
#define AS5600_RAW_ANGLE 0x0C   //  + 0x0D
#define AS5600_ANGLE     0x0E   //  + 0x0F

//  STATUS REGISTERS
#define AS5600_STATUS    0x0B

// --------------- Conversion Numbers --------------- //
static const float   AS5600_RAW_TO_DEGREES   =   360.0 / 4096;
static const float   AS5600_DEGREES_TO_RAW    =  4096 / 360.0;

// ----------------- Struct Creation ----------------- //
struct AS5600 AS5600_create(uint8_t SDA_PIN, uint8_t SCL_PIN, i2c_inst_t *address){
    struct AS5600 encoder;
    encoder.SDA = SDA_PIN;
    encoder.SCL = SCL_PIN;
    encoder.i2c = address;
    encoder.offset = 0;
    return encoder;
}

// ------------ Helper Function Prototypes ------------ //

/// Reads 8 bits from a register
uint8_t readReg(struct AS5600* encoder, uint8_t reg);
/// Reads 16 bits from a register
uint16_t readReg2(struct AS5600* encoder, uint8_t reg);

// -------------------- Functions -------------------- //

/// Returns True if AS5600 is detected
bool AS5600_isConnected(struct AS5600 *encoder){
    uint8_t reg = 0x0B;

    return i2c_write_blocking(
        encoder->i2c,
        AS5600_DEFAULT_ADDRESS,
        &reg,
        1,
        true) == 1;
}

/// Reports raw angle (0-4095)
uint16_t AS5600_rawAngle(struct AS5600* encoder){
    int16_t value = readReg2(encoder, AS5600_RAW_ANGLE);

    if (encoder->offset > 0) value += encoder->offset;
    value &= 0x0FFF;

    return value;
}

/// Reports angle (0-360 degrees)
float AS5600_readAngle(struct AS5600* encoder){
    uint16_t value = readReg2(encoder, AS5600_ANGLE);

    if (encoder->offset > 0) value += encoder->offset;
    value &= 0x0FFF;

    return value*AS5600_RAW_TO_DEGREES;
}

/// Sets offset angle (degrees)
void AS5600_setOffset(struct AS5600* encoder, float degrees){
    bool neg = (degrees < 0);
    if (neg) degrees = -degrees;

    uint16_t offset = round(degrees * AS5600_DEGREES_TO_RAW);
    offset &= 0x0FFF;
    if (neg) offset = (4096 - offset) & 0x0FFF;
    encoder->offset = offset;
}

/// Returns True if magnet is detected
bool AS5600_magnetDetected(struct AS5600* encoder){
    return (readReg(encoder, AS5600_STATUS) & 0x20) > 1;
}

/// Returns True if magnet is too close
bool AS5600_magnetTooStrong(struct AS5600* encoder){
    return (readReg(encoder, AS5600_STATUS) & 0x8) > 1;
}

/// Returns True if magnet is too far
bool AS5600_magnetTooWeak(struct AS5600* encoder){
    return (readReg(encoder, AS5600_STATUS) & 0x10) > 1;
}

// ------------------- Helper Functions ------------------- //

uint8_t readReg(struct AS5600* encoder, uint8_t reg){
    uint8_t data;

    i2c_write_blocking(
        encoder->i2c,
        AS5600_DEFAULT_ADDRESS,
        &reg,
        1,
        true);

    i2c_read_blocking(
        encoder->i2c,
        AS5600_DEFAULT_ADDRESS,
        &data,
        1,
        false);

    return data;
}
uint16_t readReg2(struct AS5600* encoder, uint8_t reg){
    uint8_t buf[2];

    i2c_write_blocking(
        encoder->i2c,
        AS5600_DEFAULT_ADDRESS,
        &reg,
        1,
        true);

    i2c_read_blocking(
        encoder->i2c,
        AS5600_DEFAULT_ADDRESS,
        buf,
        2,
        false);

    return ((uint16_t)buf[0] << 8) | buf[1];
}