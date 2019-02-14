
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

constexpr int SENSOR = 0;

void startSensorThread();

void setup() {
    startSensorThread();
}

std::int64_t timer;
std::int64_t cycleTimer;

std::thread sensor;

volatile bool result{false};
volatile float humidity{0};
volatile float temperature{0};

inline std::uint8_t digits_of(std::uint8_t byte) {
    if (byte >= 10) {
        if (byte >= 100) return 3;
        return 2;
    }
    return 1;
}

inline std::uint8_t pow10(std::uint8_t exponent) {
    switch (exponent) {
    case 0: return 1;
    case 1: return 10;
    case 2: return 100;
    default: throw std::runtime_error("Invalid exponent (pow10 may only be called with an argument from 0 to 2)");
    }
}

void startSensorThread() {
    sensor = std::thread([] () {
        while (true) {
            if (result) continue;

            std::uint32_t data;
            std::uint8_t checksum;

            pinMode(SENSOR, OUTPUT);
            digitalWrite(SENSOR, LOW);
            delay(25);
            
            digitalWrite(SENSOR, HIGH);
            pinMode(SENSOR, INPUT);
            pullUpDnControl(SENSOR, PUD_UP);
            delayMicroseconds(27);
            
            if (digitalRead(SENSOR) == LOW) {
                while (digitalRead(SENSOR) == LOW);

                for (std::uint8_t i = 0; i < 32; ++i) {
                    while (digitalRead(SENSOR) == HIGH);
                    while (digitalRead(SENSOR) == LOW);
                    delayMicroseconds(32);
                    data <<= 1;
                    if (digitalRead(SENSOR) == HIGH) data |= 1;
                }

                for (std::uint8_t i = 0; i < 8; ++i) {
                    while (digitalRead(SENSOR) == HIGH); //data clock start
                    while (digitalRead(SENSOR) == LOW); //data start
                    delayMicroseconds(32);
                    checksum <<= 1;
                    if (digitalRead(SENSOR) == HIGH) checksum |= 1;
                }
                
                std::uint8_t humidityWhole = (data >> 24) & 0xFF;
                std::uint8_t humidityFractional = (data >> 16) & 0xFF;
                std::uint8_t temperatureWhole = (data >> 8) & 0xFF;
                std::uint8_t temperatureFractional = data & 0xFF;
                
                std::uint8_t calculatedChecksum = humidityWhole + humidityFractional + temperatureWhole + temperatureFractional;
                
                if (calculatedChecksum != checksum) {
                    std::cerr << "Checksum comparison failed, expected " << checksum << ", got " << calculatedChecksum << '\n';
                } else {
                    humidity = humidityWhole + ((float) humidityFractional / pow10(digits_of(humidityFractional)));
                    temperature = temperatureWhole + ((float) temperatureFractional / pow10(digits_of(temperatureFractional)));
                    result = true;
                }
            }
            else {
                std::cerr << "DHT11 could not be queried\n";
            }
            
            delay(1000);
        }
    });
}

void loop(std::int64_t delta) {
    timer += delta;
    cycleTimer += delta;

    if (result) {
        std::cout << "Result: " << temperature << " C, " << humidity << "% relative humidity\n";
        result = false;
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
