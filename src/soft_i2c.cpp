#include "soft_i2c.h"

void SoftI2C::begin() {
    wire.begin();
}

// read register of 24 bit length
uint32_t SoftI2C::readRegister24(uint8_t address, bool send_stop = true, bool msb_first = true) {
    wire.beginTransmission(_i2c_address);
    wire.write(address);
    if (wire.endTransmission(send_stop) != 0) return 0x800000;
    wire.requestFrom(_i2c_address, 3);
    if (msb_first) {
        return (((uint32_t)wire.read() << 16) | ((uint32_t)wire.read() << 8) | (uint32_t)wire.read());
    } else {
        return (((uint32_t)wire.read()) | ((uint32_t)wire.read() << 8) | ((uint32_t)wire.read() << 16));
    }
}

// read register of 32 bit length
uint32_t SoftI2C::readRegister32(uint8_t reg, bool send_stop = true, bool msb_first = true) {
    wire.beginTransmission(_i2c_address);
    wire.write(reg);
    if (wire.endTransmission(send_stop) != 0) return 0x80000000;
    wire.requestFrom(_i2c_address, 4);
    if (msb_first) {
        return (((uint32_t)wire.read() << 24) | ((uint32_t)wire.read() << 16) | ((uint32_t)wire.read() << 8) | (uint32_t)wire.read());
    } else {
        return (((uint32_t)wire.read()) | ((uint32_t)wire.read() << 8) | ((uint32_t)wire.read() << 16) | ((uint32_t)wire.read() << 24));
    }
}

// read register of 16 bit length
uint16_t SoftI2C::readRegister16(uint8_t address, bool send_stop = true, bool msb_first = true) {
    wire.beginTransmission(_i2c_address);
    wire.write(address);
    if (wire.endTransmission(send_stop) != 0) return 0x8000;
    wire.requestFrom(_i2c_address, 2);
    if (msb_first) {
        return (((uint16_t)wire.read() << 8) | (uint16_t)wire.read());
    } else {
        return (((uint16_t)wire.read()) | ((uint16_t)wire.read() << 8));
    }
}

// write to register
bool SoftI2C::writeRegister(uint8_t address, uint8_t data) {
    wire.beginTransmission(_i2c_address);
    wire.write(address);
    wire.write(data);
    if (wire.endTransmission() != 0) return false;
    return true;
}

// read register of 8 bit length
uint8_t SoftI2C::readRegister(uint8_t address, bool send_stop = true) {
    wire.beginTransmission(_i2c_address);
    wire.write(address);
    if (wire.endTransmission(send_stop) != 0) return 0;
    wire.requestFrom(_i2c_address, 1);
    return wire.read();
}
