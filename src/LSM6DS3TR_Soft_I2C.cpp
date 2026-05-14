#include "LSM6DS3TR_Soft_I2C.h"

bool LSM6DS3TR_Soft_I2C::begin(
    lsm6ds_data_rate_t data_rate, 
    lsm6ds_accel_range_t accel_range, 
    lsm6ds_gyro_range_t gyro_range) {
    wire.begin();
    uint8_t id = readRegister(LSM6DS_WHOAMI); // who am i?

    if (id != 0x6A) {
        Serial.println("Failed to find LSM6DS3TR-C chip");
        return false;
    }

    writeRegister(LSM6DS_CTRL1_XL, (data_rate << 4 | accel_range << 2)); // enable accelerometer
    writeRegister(LSM6DS_CTRL2_G, (data_rate << 4 | gyro_range << 2)); // set gyro range
    writeRegister(LSM6DS_CTRL3_C, 0x44); // BDU=1, IF_INC = 1 

    this->data_rate = data_rate;
    this->accel_range = accel_range;
    this->gyro_range = gyro_range;

    delay(10); // wait 10ms
    return true;
}

bool LSM6DS3TR_Soft_I2C::begin() {
    return begin(this->data_rate, this->accel_range, this->gyro_range);
}

float LSM6DS3TR_Soft_I2C::readTemp() {
    int16_t temp = (int16_t)readRegister16(LSM6DS_OUT_TEMP_L);
    return temp; 
}

void LSM6DS3TR_Soft_I2C::readGyroAccel(GyroAccelMeasurement &measurement) {
    // Read temp data
    wire.beginTransmission(_i2c_address);
    wire.write(LSM6DS_OUT_TEMP_L); // begin from temp register.
    wire.requestFrom(_i2c_address, (uint8_t)14); // there are 14 registers to read: temp (2), gyro (6), accel (6)
    int16_t raw_temperature = (wire.read() | (wire.read() << 8));
    // read gyro data
    int16_t raw_gx = (wire.read() | (wire.read() << 8));
    int16_t raw_gy = (wire.read() | (wire.read() << 8));
    int16_t raw_gz = (wire.read() | (wire.read() << 8));
    // Read accel data
    int16_t raw_ax = (wire.read() | (wire.read() << 8));
    int16_t raw_ay = (wire.read() | (wire.read() << 8));
    int16_t raw_az = (wire.read() | (wire.read() << 8));
    wire.endTransmission();
    // according to datasheet: "The output of the temperature sensor is 0 LSB (typ.) at 25 °C."
    float temperature = (raw_temperature / 256) + 25;// div by 256 and add 25

    float gyro_scale = 1;
    // switch according to datasheet page 21:
    switch (this->gyro_range) {
    case LSM6DS_GYRO_RANGE_2000_DPS:
        gyro_scale = 70.0;
        break;
    case LSM6DS_GYRO_RANGE_1000_DPS:
        gyro_scale = 35.0;
        break;
    case LSM6DS_GYRO_RANGE_500_DPS:
        gyro_scale = 17.50;
        break;
    case LSM6DS_GYRO_RANGE_250_DPS:
        gyro_scale = 8.75;
        break;
    case LSM6DS_GYRO_RANGE_125_DPS:
        gyro_scale = 4.375;
        break;
    }

    float accel_scale = 1; 
    switch (this->accel_range)
    {
    case LSM6DS_ACCEL_RANGE_2_G:
        accel_scale = 0.061;
        break;
    case LSM6DS_ACCEL_RANGE_4_G:
        accel_scale = 0.122;
        break;
    case LSM6DS_ACCEL_RANGE_8_G:
        accel_scale = 0.244;
        break;
    case LSM6DS_ACCEL_RANGE_16_G:
        accel_scale = 0.488;
    }   
    // (milli-degrees per second * scale) * (radians per degree) / 1000 (to convert from milli-degrees to degrees)
    float gx = raw_gx * gyro_scale * SENSOR_DPS_TO_RADS / 1000.0; 
    float gy = raw_gy * gyro_scale * SENSOR_DPS_TO_RADS / 1000.0;
    float gz = raw_gz * gyro_scale * SENSOR_DPS_TO_RADS / 1000.0;

    // (milli-g * scale) * (meters per second squared per g) / 1000 (to convert from milli-g to g)
    float ax = raw_ax * accel_scale * SENSOR_G_TO_MS2 / 1000.0;
    float ay = raw_ay * accel_scale * SENSOR_G_TO_MS2 / 1000.0;
    float az = raw_az * accel_scale * SENSOR_G_TO_MS2 / 1000.0;

    measurement.temperature = temperature;
    measurement.accel.accel_x = ax;
    measurement.accel.accel_y = ay;
    measurement.accel.accel_z = az;
    measurement.gyro.gyro_x = gx;
    measurement.gyro.gyro_y = gy;
    measurement.gyro.gyro_z = gz;
}
