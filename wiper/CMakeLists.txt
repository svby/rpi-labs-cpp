project(rpi-wiper)

add_executable(wiper src/main.cpp)

target_link_libraries(wiper common)
target_link_libraries(wiper Threads::Threads)
target_link_libraries(wiper wiringPi)
