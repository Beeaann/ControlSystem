#include "camera/camera_logger.hpp"
#include <iostream>
#include <cstdlib>
#include <string>

namespace camera {

CameraLogger::CameraLogger() : is_initialized_(true) {}

void CameraLogger::captureEvent(const std::string& eventReason) {
    std::cout << "[CAMERA] Capturing triggered image log: " << eventReason << "\n";
    
    // Fire the native Pi 5 camera command to save a JPEG instantly upon fault
    std::string command = "libcamera-still -o fault_log_" + eventReason + ".jpg --immediate &";
    system(command.c_str());
}

} // namespace camera
