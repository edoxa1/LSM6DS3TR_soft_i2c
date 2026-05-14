#ifndef SOFT_I2C_H
#define SOFT_I2C_H
#include "Arduino.h"
#include "SoftwareWire.h"

class SoftI2C {
    public:
        SoftI2C(uint8_t addr, uint8_t sda, uint8_t scl) : wire(sda, scl), _i2c_address(addr) {
            wire = SoftwareWire(sda, scl);
        }
    protected: 
        uint8_t _i2c_address;
        void begin();
        bool writeRegister(uint8_t reg, uint8_t value);
        uint8_t readRegister(uint8_t reg, bool send_stop = true);
        uint16_t readRegister16(uint8_t reg, bool send_stop = true, bool msb_first = true);
        uint32_t readRegister24(uint8_t reg, bool send_stop = true, bool msb_first = true);
        uint32_t readRegister32(uint8_t reg, bool send_stop = true, bool msb_first = true);
        SoftwareWire wire;
};
#endif