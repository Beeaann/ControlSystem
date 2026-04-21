#include "sensors/mpu6050.hpp"
#include <iostream>

namespace sensors {

MPU6050::MPU6050(drivers::I2CBus& bus, int address) : i2c_bus_(bus), pitch_(0), roll_(0) {}

bool MPU6050::initialize() {
    // 0x68 is the default MPU6050 address.
    // Register 0x6B (107) is Power Management. Write 0x00 to wake the sensor up!
    if (!i2c_bus_.writeBytes(0x68, 0x6B, {0x00})) {
        std::cerr << "[MPU6050] Hardware ERROR: Failed to communicate at 0x68\n";
        return false;
    }
    std::cout << "[MPU6050] Successfully woken up from I2C sleep.\n";
    return true;
}

void MPU6050::readRawData() {
    // Register 0x3B starts the 14 continuous bytes of physics data (Accel X,Y,Z, Temp, Gyro X,Y,Z)
    // We'll read the first 6 bytes which correlate to the 16-bit Accelerometer values.
    auto data = i2c_bus_.readBytes(0x68, 0x3B, 6);
    
    if (data.size() == 6) {
        // MPU6050 data is split into HIGH byte and LOW byte over I2C. Bitshift them into 16-bit signed ints.
        int16_t accelX = (data[0] << 8) | data[1];
        int16_t accelY = (data[2] << 8) | data[3];
        int16_t accelZ = (data[4] << 8) | data[5];
        
        // Simple Gravity Mapping: 1g is roughly 16384 raw. Map X-axis acceleration directly to an angle (-90 to +90)
        pitch_ = (accelX / 16384.0f) * 90.0f;
        
        // Clamp it safely for the servo limits
        if (pitch_ > 90.0f) pitch_ = 90.0f;
        if (pitch_ < -90.0f) pitch_ = -90.0f;
    }
}

void MPU6050::updateComplementaryFilter(float dt) {
    // Stubbed Alpha/Beta fusion filter
    pitch_ += 0.02f * dt; 
    roll_ += 0.02f * dt;
}

float MPU6050::getPitch() const { return pitch_; }
float MPU6050::getRoll() const { return roll_; }

} // namespace sensors
