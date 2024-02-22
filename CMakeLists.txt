cmake_minimum_required(VERSION 3.16.3)
project(hospital_management)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# Include directories
include_directories(Hospital.hpp)

# Source files
set(SOURCES
    main.cpp
    Hospital.cpp
)

# Executable
add_executable(hospital_management ${SOURCES})