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
#ifndef ESPX_DANCING_MUSIC_APPLICATION_H
#define ESPX_DANCING_MUSIC_APPLICATION_H

#include "config/config.h"
#include <FastLED.h>
#include <adc/ADC.h>
#include <audio/AudioAnalyzer.h>
#include <web/WebServer.h>
#include <led/Led.h>
#include <wifi/WiFiManager.h>

class Application {
public:
    static Application &getInstance() {
        static Application s;
        return s;
    }

    Application(Application const &) = delete;

    Application &operator=(Application const &) = delete;

    ADC *getAdc();

    AudioAnalyzer *getAudioAnalyzer();

    Led *getLed();

    void setup();

    void loop();

    static void reboot() {
        ESP.restart();
    }

    static void hardwareReset() {
#ifdef ESP32
        WiFi.disconnect(true, true);
        ESP.restart();
#else
        ESP.reset();
#endif
    }

private:
    ADC *adc;
    AudioAnalyzer *audioAnalyzer;
    WebServer *webServer;
    Led *led;

    Application();

    ~Application() = default; // и деструктор

    bool apMode = false;
};


#endif //ESPX_DANCING_MUSIC_APPLICATION_H
