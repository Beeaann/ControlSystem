#pragma once

#include <cstdint>
#include <vector>

namespace drivers {

/**
 * @brief Low-level I2C Bus Driver handling standard OS-level calls
 * (e.g. ioctl via /dev/i2c-x) for Embedded Linux environments.
 */
class I2CBus {
public:
    I2CBus(int busNumber);
    ~I2CBus();

    bool initialize();

    bool writeBytes(uint8_t deviceAddress, uint8_t registerAddress, const std::vector<uint8_t>& data);
    std::vector<uint8_t> readBytes(uint8_t deviceAddress, uint8_t registerAddress, size_t length);

private:
    int bus_number_;
    int file_descriptor_;
};

} // namespace drivers
