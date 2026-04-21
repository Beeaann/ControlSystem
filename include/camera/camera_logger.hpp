#pragma once

#include <string>

namespace camera {

/**
 * @brief Interfaces with the Raspberry Pi Camera module.
 * Logs images automatically to disk containing timestamped visual logs
 * upon critical system events or fault triggers.
 */
class CameraLogger {
public:
    CameraLogger();

    /**
     * @brief Trigger an immediate image capture.
     * @param eventReason The reason logged on the metadata text of the image.
     */
    void captureEvent(const std::string& eventReason);
    
private:
    bool is_initialized_;
};

} // namespace camera
