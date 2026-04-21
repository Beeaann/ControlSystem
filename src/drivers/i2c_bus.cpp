#include "drivers/i2c_bus.hpp"
#include <iostream>
#include <string>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

namespace drivers {

I2CBus::I2CBus(int busNumber) : bus_number_(busNumber), file_descriptor_(-1) {}

I2CBus::~I2CBus() {
    if (file_descriptor_ >= 0) {
        close(file_descriptor_);
    }
}

bool I2CBus::initialize() {
    std::string deviceFile = "/dev/i2c-" + std::to_string(bus_number_);
    file_descriptor_ = open(deviceFile.c_str(), O_RDWR);
    if (file_descriptor_ < 0) {
        std::cerr << "[I2C] Failed to open bus " << bus_number_ << ". Run 'sudo raspi-config' to ensure I2C is enabled.\n";
        return false;
    }
    std::cout << "[I2C] Successfully opened " << deviceFile << "\n";
    return true; 
}

bool I2CBus::writeBytes(uint8_t deviceAddress, uint8_t registerAddress, const std::vector<uint8_t>& data) {
    if (file_descriptor_ < 0) return false;
    if (ioctl(file_descriptor_, I2C_SLAVE, deviceAddress) < 0) return false;

    // The first byte written is the Target Register Address
    std::vector<uint8_t> buffer;
    buffer.push_back(registerAddress);
    buffer.insert(buffer.end(), data.begin(), data.end());

    if (write(file_descriptor_, buffer.data(), buffer.size()) != (ssize_t)buffer.size()) {
        return false;
    }
    return true; 
}

std::vector<uint8_t> I2CBus::readBytes(uint8_t deviceAddress, uint8_t registerAddress, size_t length) {
    std::vector<uint8_t> buffer(length, 0);
    if (file_descriptor_ < 0) return buffer;
    
    // Assert target chip
    if (ioctl(file_descriptor_, I2C_SLAVE, deviceAddress) < 0) return buffer;
    
    // Command the register we want to read from
    if (write(file_descriptor_, &registerAddress, 1) != 1) return buffer;
    
    // Pull the requested number of bytes
    if (read(file_descriptor_, buffer.data(), length) != (ssize_t)length) {
        std::cerr << "[I2C] Read failed!\n";
    }
    return buffer;
}

} // namespace drivers
