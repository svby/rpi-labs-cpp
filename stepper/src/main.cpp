
#include <wiringPi.h>
#include <softPwm.h>
#include <softTone.h>

#include <constants.h>

#include <algorithm>
#include <bitset>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <ctime>
#include <exception>
#include <iostream>
#include <thread>

constexpr int IN1 = 1, IN2 = 2, IN3 = 3, IN4 = 4;

constexpr std::int64_t MOTOR_INTERVAL = 50000;

constexpr int steps[6][4] = {
    {1, 0, 1, 0},
    {0, 1, 1, 0},
    {0, 1, 0, 1},
    {1, 0, 0, 1},
    {0, 1, 0, 1},
    {0, 1, 1, 0}
};

void setup() {
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
}

std::int64_t timer;
std::int64_t cycleTimer;

int currentStep{0};

void loop(std::int64_t delta) {
    timer += delta;
    cycleTimer += delta;

    if (cycleTimer >= MOTOR_INTERVAL) {
        currentStep = (currentStep + 1) % 6;
        int const *stepData = steps[currentStep];

        digitalWrite(IN1, stepData[0]);
        digitalWrite(IN2, stepData[1]);
        digitalWrite(IN3, stepData[2]);
        digitalWrite(IN4, stepData[3]);

        cycleTimer = 0;
    }
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
