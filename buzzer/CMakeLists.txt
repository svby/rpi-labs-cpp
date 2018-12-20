project(rpi-buzzer)

add_executable(buzzer src/main.cpp)

target_link_libraries(buzzer common)
target_link_libraries(buzzer Threads::Threads)
target_link_libraries(buzzer wiringPi)
