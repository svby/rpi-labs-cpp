project(rpi-humidity)

add_executable(humidity src/main.cpp)

target_link_libraries(humidity common)
target_link_libraries(humidity Threads::Threads)
target_link_libraries(humidity wiringPi)
