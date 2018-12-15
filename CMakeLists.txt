cmake_minimum_required(VERSION 3.7.2)
set(CMAKE_CXX_STANDARD 14)

project(rpi-labs-parent)

find_package(Threads REQUIRED)

add_subdirectory(common)
add_subdirectory(led-pulse)
add_subdirectory(dimmer)
