cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)

project(rpi-labs-parent)

find_package(Threads REQUIRED)

add_subdirectory(common)
add_subdirectory(led-pulse)
add_subdirectory(dimmer)
add_subdirectory(buzzer)
add_subdirectory(distance-buzzer)
add_subdirectory(humidity)
add_subdirectory(wiper)
