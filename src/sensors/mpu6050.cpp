#include "sensors/mpu6050.hpp"
#include <iostream>

namespace sensors {

MPU6050::MPU6050(drivers::I2CBus& bus, int address) : i2c_bus_(bus), pitch_(0), roll_(0) {}

bool MPU6050::initialize() {
    std::cout << "[MPU6050] Initialized via I2C\n";
    return true;
}

void MPU6050::readRawData() {
    // Read directly from i2c_bus_
}

void MPU6050::updateComplementaryFilter(float dt) {
    // Example complementary alpha/beta blend
    pitch_ += 0.02f * dt; 
    roll_ += 0.02f * dt;
}

float MPU6050::getPitch() const { return pitch_; }
float MPU6050::getRoll() const { return roll_; }

} // namespace sensors
