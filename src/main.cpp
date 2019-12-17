/*
 * This file is part of the espx-dancing-led distribution (https://github.com/kluevandrew/espx-dancing-led).
 * Copyright (c) 2019, Kluev Andrew <kluev.andrew@gmail.com>.
 *
 * espx-dancing-led is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License.
 *
 * You are free to:
 *   Share — copy and redistribute the material in any medium or format
 *   Adapt — remix, transform, and build upon the material
 *
 * Under the following terms:
 *   Attribution — You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
 *   NonCommercial — You may not use the material for commercial purposes.
 *   ShareAlike — If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
 *   No additional restrictions — You may not apply legal terms or technological measures that legally restrict others from doing anything the license permits.
 *
 * For additional information, see <http://creativecommons.org/licenses/by-nc-sa/4.0/>.
 */
#ifndef ESP32
#define FASTLED_ESP8266_RAW_PIN_ORDER
#define FASTLED_ALLOW_INTERRUPTS 0
#define FASTLED_INTERRUPT_RETRY_COUNT 1
#endif
#define ARDUINOJSON_ENABLE_ARDUINO_STRING 1

#include "Arduino.h"
#include "Application.h"

#ifndef ESP32
ADC_MODE(ADC_VCC);
#endif

void setup() {
    Application::getInstance().setup();
}

void loop() {
    pinMode(22, OUTPUT);
    pinMode(23, OUTPUT);
    digitalWrite(22, LOW);
    digitalWrite(23, LOW);
    Application::getInstance().loop();
}

#pragma clang diagnostic pop