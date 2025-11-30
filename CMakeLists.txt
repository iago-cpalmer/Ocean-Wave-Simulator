cmake_minimum_required(VERSION 3.16)
project(OceanWavesSimulator LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Vendor libraries
add_subdirectory(vendor/glm)
add_subdirectory(vendor/glfw)
add_subdirectory(vendor/assimp)
add_subdirectory(vendor/glad)

# Project source
add_subdirectory(src)