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
#include <thread>

constexpr int B = 0;
constexpr int TRIG = 2, ECHO = 1;

constexpr float LIMIT = 40.0f;
constexpr float BUZZER_LOW = 200.0f;
constexpr float BUZZER_HIGH = 2000.0f;

void startSensorThread();

void setup() {
    pinMode(TRIG, OUTPUT);
    pinMode(ECHO, INPUT);

    digitalWrite(TRIG, LOW);

    // The buzzer
    softToneCreate(B);

    startSensorThread();
}

std::int64_t timer;
std::int64_t cycleTimer;

int buzzerValue;

std::thread sensor;

volatile bool result{false};
volatile float distance;

void startSensorThread() {
    sensor = std::thread([] () {
        while (true) {
            if (result) continue;
            digitalWrite(TRIG, HIGH);
            std::this_thread::sleep_for(std::chrono::microseconds(10));
            digitalWrite(TRIG, LOW);

            while (digitalRead(ECHO) == LOW);

            auto start = std::chrono::high_resolution_clock::now();
            while (digitalRead(ECHO) == HIGH);
            auto end = std::chrono::high_resolution_clock::now();

            const auto duration = std::chrono::duration_cast<microseconds>(end - start).count();

            distance = duration / 1000000.0 * 34300 / 2;
            result = true;
        }
    });
}

void loop(std::int64_t delta) {
    timer += delta;
    cycleTimer += delta;

    if (cycleTimer >= 10000 && result) {
        auto clamped = (LIMIT - std::clamp(static_cast<float>(distance), 0.0f, LIMIT));
        
        if (clamped > 0.1f) {
            auto scaled = clamped / LIMIT * (BUZZER_HIGH - BUZZER_LOW) + BUZZER_LOW;
            buzzerValue = static_cast<int>(scaled);
        } else buzzerValue = 0;

        cycleTimer = 0;
        result = false;
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
