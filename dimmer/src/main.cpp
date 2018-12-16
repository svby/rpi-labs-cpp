#include <wiringPi.h>
#include <softPwm.h>
#include <ads1115.h>

#include <constants.h>

#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <ctime>
#include <iostream>

constexpr int R = 0, G = 1, B = 2;
constexpr int P = 0;

constexpr int ADS1115_OFFSET = 128;
constexpr int ADS1115_ADDRESS = 0x48;

void setup() {
    ads1115Setup(ADS1115_OFFSET, ADS1115_ADDRESS);
    
    // The LED
    softPwmCreate(R, 0, 100);
    softPwmCreate(G, 0, 100);
    softPwmCreate(B, 0, 100);
    
    // The photoresistor
    pinMode(ADS1115_OFFSET + P, INPUT);
}

std::int64_t timer;
std::int64_t cycleTimer;

int ledStrength;

constexpr float CLAMP_MIN = 6000.0f;
constexpr float CLAMP_MAX = 26000.0f;

void loop(std::int64_t delta) {
    timer += delta;
    cycleTimer += delta;
    
    if (cycleTimer >= 10000) {
        auto value = static_cast<float>(analogRead(ADS1115_OFFSET + P));
        auto strength = std::clamp(value, CLAMP_MIN, CLAMP_MAX) - CLAMP_MIN;

        auto scaled = strength / (CLAMP_MAX - CLAMP_MIN) * 100.0f;
        
        ledStrength = static_cast<int>(scaled);
    
        cycleTimer = 0;
    }
    
    softPwmWrite(R, ledStrength);
    softPwmWrite(G, ledStrength);
    softPwmWrite(B, ledStrength);
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
