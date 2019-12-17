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
#ifndef ESPX_DANCING_LED_SETTINGS_H
#define ESPX_DANCING_LED_SETTINGS_H

#include <arduinoFFT.h>
#include <ArduinoJson.h>
#include <config/config.h>
#include <FastLED.h>
#include <FS.h>
#ifdef ESP32
#include <SPIFFS.h>
#endif


class Settings {

public:

    static Settings &getInstance() {
        static Settings instance;

        return instance;
    }

    Settings(Settings const &) = delete;

    Settings &operator=(Settings const &) = delete;

    struct AudioSettings {
        // micro
        int micPin = ADC_MIC_CHANNEL;

        // fft
        byte fftWindowType = FFT_WIN_TYP_HAMMING;
        boolean useDCRemoval = false;

        // noise filter
        unsigned int lowLevelFilter = 4500;
        unsigned int midLevelFilter = 2500;
        unsigned int highLevelFilter = 2500;
    };

    struct AnimationsSettings {
        struct ColorSetting {
            byte hue;
            byte sat;
            byte val;

            ColorSetting() = default;

            ColorSetting(byte hue, byte sat, byte val) {
                this->hue = hue;
                this->sat = sat;
                this->val = val;
            }
        };

        struct DirectColorAnimationSettings {
            int step = 5;
            int delay = 5;
            ColorSetting color = ColorSetting(0, 0, 200);
        };

        struct DynamicColorAnimationSettings {
            unsigned long delay = 100;
            ColorSetting color = ColorSetting(HUE_RED, 255, 200);
        };

        struct StrobeAnimationSettings {
            unsigned int timeOn = 200;
            unsigned int timeOff = 100;
            byte stepOn = 50;
            byte delayOn = 10;
            ColorSetting color = ColorSetting(0, 0, 200);
        };

        struct RainbowAnimationSettings {
            unsigned int step = 1;
            unsigned int period = 1;
            unsigned int delay = 30;
            ColorSetting color = ColorSetting(0, 255, 200);
        };

        struct BlurAnimationSettings {
            byte bpm = 3;
            ColorSetting color = ColorSetting(0, 255, 200);
        };

        struct DotBeatAnimationSettings {
            int bpm = 33;
            int fade = 224;
            ColorSetting middle = ColorSetting(HUE_PURPLE, 255, 255);
            ColorSetting inner = ColorSetting(HUE_BLUE, 255, 255);
            ColorSetting outer = ColorSetting(HUE_AQUA, 255, 255);
        };

        struct ConfettiAnimationSettings {
            byte duration = 15;
            byte delay = 5;
            byte fade = 8;
            ColorSetting color = ColorSetting(0, 0, 255);
        };

        struct SimpleFrequencyAnimationSettings {
            double coefficient = 2.0;
            int smoothStep = 20;
            ColorSetting low = ColorSetting(HUE_RED, 255, 200);
            ColorSetting mid = ColorSetting(HUE_YELLOW, 255, 200);
            ColorSetting high = ColorSetting(HUE_GREEN, 255, 200);
        };

        struct RunningFrequencyAnimationSettings {
            double coefficient = 2.0;
            unsigned int latency = 20;
            ColorSetting low = ColorSetting(HUE_RED, 255, 200);
            ColorSetting mid = ColorSetting(HUE_YELLOW, 255, 200);
            ColorSetting high = ColorSetting(HUE_GREEN, 255, 200);
        };

        struct FrequencyStrobeAnimationSettings {
            double coefficient = 2.0;
            byte mode = 0;
            ColorSetting low = ColorSetting(HUE_RED, 255, 200);
            ColorSetting mid = ColorSetting(HUE_YELLOW, 255, 200);
            ColorSetting high = ColorSetting(HUE_GREEN, 255, 200);
            ColorSetting empty = ColorSetting(0, 0, 0);
        };

        struct SpectrumAnalyzerFrequencyAnimationSettings {
            byte startColor = 0;
            byte step = 5;
        };

        struct UvMeterAnimationSettings {
            double bottomCoefficient = 0.074;
            double topCoefficient = 0.6;
            byte fadeFactor = 20;
            byte mode = 1;
        };

        String currentAnimation = "DirectColor";
        DirectColorAnimationSettings directColor = {};
        DynamicColorAnimationSettings dynamicColor = {};
        StrobeAnimationSettings strobe = {};
        RainbowAnimationSettings rainbow = {};
        BlurAnimationSettings blur = {};
        DotBeatAnimationSettings dotBeat = {};
        ConfettiAnimationSettings confetti = {};
        SimpleFrequencyAnimationSettings simpleFrequency = {};
        RunningFrequencyAnimationSettings runningFrequency = {};
        FrequencyStrobeAnimationSettings frequencyStrobe = {};
        SpectrumAnalyzerFrequencyAnimationSettings spectrumAnalyzer = {};
        UvMeterAnimationSettings uvMeter = {};
    };

    AudioSettings audio = {};
    AnimationsSettings animations = {};

    void set(const String &key, JsonVariant value);

    void load();
    void save();
    void reset();

private:
    Settings() = default;
    ~Settings() = default;
};


#endif //ESPX_DANCING_LED_SETTINGS_H
