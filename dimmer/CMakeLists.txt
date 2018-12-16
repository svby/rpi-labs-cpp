project(rpi-dimmer)

add_executable(dimmer src/main.cpp)

target_link_libraries(dimmer common)
target_link_libraries(dimmer Threads::Threads)
target_link_libraries(dimmer wiringPi)
