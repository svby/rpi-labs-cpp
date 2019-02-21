project(rpi-stepper)

add_executable(stepper src/main.cpp)

target_link_libraries(stepper common)
target_link_libraries(stepper Threads::Threads)
target_link_libraries(stepper wiringPi)
