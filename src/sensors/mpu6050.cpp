#include "sensors/mpu6050.hpp"
#include <iostream>

namespace sensors {

MPU6050::MPU6050(drivers::I2CBus& bus, int address) : i2c_bus_(bus), pitch_(0), roll_(0) {}

bool MPU6050::initialize() {
    // Wake the sensor by writing 0 to the Power Management register
    if (!i2c_bus_.writeBytes(0x68, 0x6B, {0x00})) {
        std::cerr << "[MPU6050] Hardware ERROR: Failed to communicate at 0x68\n";
        return false;
    }
    std::cout << "[MPU6050] Successfully woken up from I2C sleep.\n";
    return true;
}

void MPU6050::readRawData() {
    // Read the 6 consecutive bytes containing X, Y, and Z accelerometer data
    auto data = i2c_bus_.readBytes(0x68, 0x3B, 6);
    
    if (data.size() == 6) {
        // Build 16-bit signed ints from the data chunks
        int16_t accelX = (data[0] << 8) | data[1];
        int16_t accelY = (data[2] << 8) | data[3];
        int16_t accelZ = (data[4] << 8) | data[5];
        
        // Map 1g (16384) directly to 90 degrees
        pitch_ = (accelX / 16384.0f) * 90.0f;
        
        // Clamp output safely
        if (pitch_ > 90.0f) pitch_ = 90.0f;
        if (pitch_ < -90.0f) pitch_ = -90.0f;
    }
}

void MPU6050::updateComplementaryFilter(float dt) {
    // Basic fusion filter limits
    pitch_ += 0.02f * dt; 
    roll_ += 0.02f * dt;
}

float MPU6050::getPitch() const { return pitch_; }
float MPU6050::getRoll() const { return roll_; }

} // namespace sensors
