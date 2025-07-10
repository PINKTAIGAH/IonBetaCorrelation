cmake_minimum_required(VERSION 3.18)
project(ionbeta)

# Detect and include the appropriate platform-specific file
if(APPLE)
    message(STATUS "Detected macOS — including macOS CMake config.")
    include(${CMAKE_SOURCE_DIR}/CMakeLists.macos.txt)
elseif(UNIX)
    message(STATUS "Detected Unix/Linux — including default CMake config.")
    include(${CMAKE_SOURCE_DIR}/CMakeLists.linux.txt)
else()
    message(FATAL_ERROR "Unsupported platform")
endif()