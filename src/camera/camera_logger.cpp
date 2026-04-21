#include "camera/camera_logger.hpp"
#include <iostream>
#include <cstdlib>

namespace camera {

CameraLogger::CameraLogger() : is_initialized_(true) {}

void CameraLogger::captureEvent(const std::string& eventReason) {
    std::cout << "[CAMERA] Capturing triggered image log: " << eventReason << "\n";
    // Command line utility triggers image natively
    // system(("libcamera-still -o event_log_" + eventReason + ".jpg --immediate &").c_str());
}

} // namespace camera
