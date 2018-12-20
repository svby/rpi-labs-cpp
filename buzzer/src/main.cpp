#include <wiringPi.h>
#include <softPwm.h>
#include <softTone.h>

#include <constants.h>

#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <ctime>
#include <iostream>

constexpr int B = 0;

void setup() {
    // The buzzer
    softToneCreate(B);
}

std::int64_t timer;
std::int64_t cycleTimer;

int buzzerValue;

void loop(std::int64_t delta) {
    timer += delta;
    cycleTimer += delta;
    
    if (cycleTimer >= 10000) {
        auto seconds = timer / 1000000.0f;
        auto value = std::sin(PI<float> * seconds / 5.0f);
        auto scaled = (value + 1) * 1250 + 200;
        
        buzzerValue = static_cast<int>(scaled);
    
        cycleTimer = 0;
    }
    
    softToneWrite(B, buzzerValue);
}

int main() {
    wiringPiSetup();
    
    setup();
    
    auto oldTime = std::chrono::high_resolution_clock::now();
    
    while (true) {
        auto newTime = std::chrono::high_resolution_clock::now();
        auto delta = std::chrono::duration_cast<microseconds>(newTime - oldTime).count();
        if (!delta) continue;
        loop(delta);
        oldTime = newTime;
    }
}
