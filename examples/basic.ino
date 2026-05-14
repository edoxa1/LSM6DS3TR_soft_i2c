#include "Arduino.h"
#include "LSM6DS3TR_Soft_I2C.h"

LSM6DS3TR_Soft_I2C sensor(0x6A, 4, 5); // I2C address, SDA pin, SCL pin

void setup() {
    Serial.begin(115200);
    while (!Serial) {
        delay(10);
    }
    lsm6ds_data_rate_t data_rate = LSM6DS_RATE_104_HZ;
    lsm6ds_accel_range_t accel_range = LSM6DS_ACCEL_RANGE_2_G;
    lsm6ds_gyro_range_t gyro_range = LSM6DS_GYRO_RANGE_250_DPS;

    if (!sensor.begin(data_rate, accel_range, gyro_range)) {
        Serial.println("Failed to initialize the sensor!");
        while (1) {}
    }
}

void loop() {
    LSM6DS3TR_Soft_I2C::GyroAccelMeasurement measurement;
    sensor.readGyroAccel(measurement);
    Serial.print("Temperature: "); Serial.print(measurement.temperature, 2); Serial.println(" °C");
    Serial.print("Gyro X: "); Serial.print(measurement.gyro.x, 2); Serial.println(" rad/s");
    Serial.print("Gyro Y: "); Serial.print(measurement.gyro.y, 2); Serial.println(" rad/s");
    Serial.print("Gyro Z: "); Serial.print(measurement.gyro.z, 2); Serial.println(" rad/s");
    Serial.print("Accel X: "); Serial.print(measurement.accel.x, 2); Serial.println(" m/s^2");
    Serial.print("Accel Y: "); Serial.print(measurement.accel.y, 2); Serial.println(" m/s^2");
    Serial.print("Accel Z: "); Serial.print(measurement.accel.z, 2); Serial.println(" m/s^2");

    // Alternatively, you can use toString():
    // Serial.println(measurement.toString());
    delay(1000);
}
