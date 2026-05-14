#ifndef LSM6DS3TR_Soft_I2C_h
#define LSM6DS3TR_Soft_I2C_h
#include "soft_i2c.h"

// registers table
// taken from https://github.com/adafruit/Adafruit_LSM6DS/blob/master/Adafruit_LSM6DS.h:

#define LSM6DS_I2CADDR_DEFAULT      0x6A      ///< LSM6DS default i2c address
#define LSM6DS_FUNC_CFG_ACCESS      0x1       ///< Enable embedded functions register
#define LSM6DS_INT1_CTRL            0x0D      ///< Interrupt control for INT 1
#define LSM6DS_INT2_CTRL            0x0E      ///< Interrupt control for INT 2
#define LSM6DS_WHOAMI               0x0F      ///< Chip ID register
#define LSM6DS_CTRL1_XL             0x10      ///< Main accelerometer config register
#define LSM6DS_CTRL2_G              0x11      ///< Main gyro config register
#define LSM6DS_CTRL3_C              0x12      ///< Main configuration register
#define LSM6DS_CTRL8_XL             0x17      ///< High and low pass for accel
#define LSM6DS_CTRL10_C             0x19      ///< Main configuration register
#define LSM6DS_WAKEUP_SRC           0x1B      ///< Why we woke up
#define LSM6DS_STATUS_REG           0X1E      ///< Status register
#define LSM6DS_OUT_TEMP_L           0x20      ///< First data register (temperature low)
#define LSM6DS_OUTX_L_G             0x22      ///< First gyro data register
#define LSM6DS_OUTX_L_A             0x28      ///< First accel data register
#define LSM6DS_STEPCOUNTER          0x4B      ///< 16-bit step counter

#define LSM6DS_TAP_CFG              0x58      ///< Tap/pedometer configuration
#define LSM6DS_WAKEUP_THS           0x5B      ///< Single and double-tap function threshold register
#define LSM6DS_WAKEUP_DUR           0x5C      ///< Free-fall, wakeup, timestamp and sleep mode duration
#define LSM6DS_MD1_CFG              0x5E      ///< Functions routing on INT1 register

#define LSM6DS_ACCEL_DEFAULT_EN     0b01011000 // 208 Hz, +-4g
#define SENSOR_DPS_TO_RADS         0.01745329252F  // pi/180
#define SENSOR_G_TO_MS2            9.80665F   // g = ~9.8m/s^2


/** The accelerometer data rate */
typedef enum data_rate {
  LSM6DS_RATE_SHUTDOWN,
  LSM6DS_RATE_12_5_HZ,
  LSM6DS_RATE_26_HZ,
  LSM6DS_RATE_52_HZ,
  LSM6DS_RATE_104_HZ,
  LSM6DS_RATE_208_HZ,
  LSM6DS_RATE_416_HZ,
  LSM6DS_RATE_833_HZ,
  LSM6DS_RATE_1_66K_HZ,
  LSM6DS_RATE_3_33K_HZ,
  LSM6DS_RATE_6_66K_HZ,
} lsm6ds_data_rate_t;

/** The accelerometer data range */
typedef enum accel_range {
  LSM6DS_ACCEL_RANGE_2_G,
  LSM6DS_ACCEL_RANGE_16_G,
  LSM6DS_ACCEL_RANGE_4_G,
  LSM6DS_ACCEL_RANGE_8_G
} lsm6ds_accel_range_t;

/** The gyro data range */
typedef enum gyro_range {
  LSM6DS_GYRO_RANGE_125_DPS = 0b0010,
  LSM6DS_GYRO_RANGE_250_DPS = 0b0000,
  LSM6DS_GYRO_RANGE_500_DPS = 0b0100,
  LSM6DS_GYRO_RANGE_1000_DPS = 0b1000,
  LSM6DS_GYRO_RANGE_2000_DPS = 0b1100,
} lsm6ds_gyro_range_t;

/** The high pass filter bandwidth */
typedef enum hpf_range {
  LSM6DS_HPF_ODR_DIV_50 = 0,
  LSM6DS_HPF_ODR_DIV_100 = 1,
  LSM6DS_HPF_ODR_DIV_9 = 2,
  LSM6DS_HPF_ODR_DIV_400 = 3,
} lsm6ds_hp_filter_t;


class LSM6DS3TR_Soft_I2C : SoftI2C {
public:
    struct AccelData {
        float x;
        float y;
        float z;
    };
    
    struct GyroData {
        float x;
        float y;
        float z;
    };

    struct GyroAccelMeasurement {
        float temperature;
        AccelData accel;
        GyroData gyro;
        String toString() const {
            String s;
            s += "temp=" + String(temperature, 2);
            s += ", accel=(" + String(accel.accel_x, 3) + ", ";
            s += String(accel.accel_y, 3) + ", ";
            s += String(accel.accel_z, 3) + ")";
            s += ", gyro=(" + String(gyro.gyro_x, 3) + ", ";
            s += String(gyro.gyro_y, 3) + ", ";
            s += String(gyro.gyro_z, 3) + ")";
            return s;
        }   
    };

    LSM6DS3TR_Soft_I2C(uint8_t addr, uint8_t sda, uint8_t scl) : SoftI2C(addr, sda, scl) {}
    bool begin(
        lsm6ds_data_rate_t accel_data_rate, 
        lsm6ds_accel_range_t accel_range, 
        lsm6ds_gyro_range_t gyro_range);
    bool begin();
    void changeAccelRange(lsm6ds_accel_range_t accel_range) {
        this->accel_range = accel_range;
    }
    void changeGyroRange(lsm6ds_gyro_range_t gyro_range) {
        this->gyro_range = gyro_range;
    }
    float readTemp();
    void readGyroAccel(GyroAccelMeasurement &measurement);

private:
    lsm6ds_data_rate_t data_rate = LSM6DS_RATE_104_HZ;
    lsm6ds_accel_range_t accel_range = LSM6DS_ACCEL_RANGE_4_G;
    lsm6ds_gyro_range_t gyro_range = LSM6DS_GYRO_RANGE_250_DPS;
};
#endif