project(rpi-led-pulse)

add_executable(led-pulse src/main.cpp)

target_link_libraries(led-pulse common)
target_link_libraries(led-pulse Threads::Threads)
target_link_libraries(led-pulse wiringPi)
