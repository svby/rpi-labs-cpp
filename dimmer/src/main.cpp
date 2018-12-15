#include <wiringPi.h>
#include <softPwm.h>

#include <constants.h>

#include <chrono>
#include <cmath>
#include <cstdint>
#include <ctime>
#include <iostream>

const int R = 0, G = 1, B = 2;
const int P = 3;

void setup() {
    // The LED
    softPwmCreate(R, 0, 100);
    softPwmCreate(G, 0, 100);
    softPwmCreate(B, 0, 100);
    
    // The photoresistor
    pinMode(P, INPUT);
}

std::int64_t timer;
std::int64_t cycleTimer;

int ledStrength;

void loop(std::int64_t delta) {
    timer += delta;
    cycleTimer += delta;
    
    if (cycleTimer >= 10000) {
        auto seconds = timer / 1000000.0f;
        auto value = std::sin(PI<float> * seconds);
        auto scaled = (value + 1) * 20;
        
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
