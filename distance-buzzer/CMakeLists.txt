project(rpi-buzzer)

add_executable(distance-buzzer src/main.cpp)

target_link_libraries(distance-buzzer common)
target_link_libraries(distance-buzzer Threads::Threads)
target_link_libraries(distance-buzzer wiringPi)
