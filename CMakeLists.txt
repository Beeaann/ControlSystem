cmake_minimum_required(VERSION 3.10)
project(AutonomousControlSystem VERSION 1.0)

# We require C++17 for features like standard thread facilities, chrono, etc.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set common compiler flags for performance and warnings
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O3 -pthread")

# Ensure threading package is found
find_package(Threads REQUIRED)

# Includes folder
include_directories(include)

# Gather source files
file(GLOB_RECURSE SRC_FILES "src/*.cpp")

# Define our main executable
add_executable(AutonomousControlSystem ${SRC_FILES})
target_link_libraries(AutonomousControlSystem PRIVATE Threads::Threads lgpio)
# Link real-time libraries (POSIX) if necessary: target_link_libraries(AutonomousControlSystem PRIVATE rt)

# Tests executable
file(GLOB_RECURSE TEST_FILES "tests/*.cpp")
if(TEST_FILES)
    add_executable(Tests_FSM_Faults ${TEST_FILES})
    target_link_libraries(Tests_FSM_Faults PRIVATE Threads::Threads)
endif()
