project(rpi-labs-common)

add_library(common STATIC
    src/main.cpp
)

target_include_directories(common PUBLIC include/)
