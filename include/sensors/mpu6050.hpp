#pragma once

#include "drivers/i2c_bus.hpp"

namespace sensors {

/**
 * @brief Interfaces with the MPU6050 Accelerometer/Gyroscope over I2C.
 * Implements a Complementary Filter for drift-resistant pose estimation.
 */
class MPU6050 {
public:
    MPU6050(drivers::I2CBus& bus, int address = 0x68);

    /**
     * @brief Initialize registers and calibrate biases.
     */
    bool initialize();

    /**
     * @brief Fetch latest raw telemetry from the sensor.
     */
    void readRawData();

    /**
     * @brief Executes the sensor fusion complimentary filter (Alpha/Beta blend).
     */
    void updateComplementaryFilter(float dt);

    float getPitch() const;
    float getRoll() const;

private:
    drivers::I2CBus& i2c_bus_;
    float pitch_, roll_;
};

} // namespace sensors
