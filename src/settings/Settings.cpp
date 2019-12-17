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
#include <led/animations/frequency/FrequencyStrobeAnimation.h>
#include "Settings.h"


#define SETTING_SETTER(KEY) if (key == #KEY) { \
    Serial.println(#KEY); \
    KEY = value; \
    return; \
}
#define COLOR_SETTING_SETTER(KEY) if (key == #KEY) { \
    Serial.println(#KEY); \
    KEY.hue =  map(value.getMember("hue").as<int>(), 0, 360, 0, 255); \
    KEY.sat =  map(value.getMember("sat").as<int>(), 0, 100, 0, 255); \
    KEY.val =  map(value.getMember("val").as<int>(), 0, 100, 0, 255); \
} \
SETTING_SETTER(KEY.hue) \
SETTING_SETTER(KEY.sat) \
SETTING_SETTER(KEY.val)

#define SETTING_LOADER(KEY) if (!document.getMember(#KEY).isNull() && !document.getMember(#KEY).isUndefined()) { \
                                KEY = document.getMember(#KEY); \
                            }

#define COLOR_SETTING_LOADER(KEY) SETTING_LOADER(KEY.hue) \
                                  SETTING_LOADER(KEY.sat) \
                                  SETTING_LOADER(KEY.val)

#define SETTING_SAVER(KEY) document[#KEY] = KEY;
#define COLOR_SETTING_SAVER(KEY) SETTING_SAVER(KEY.hue) \
                                 SETTING_SAVER(KEY.sat) \
                                 SETTING_SAVER(KEY.val)

#define ANIMATIONS_MACRO(SINGLE_MACRO, COLOR_MACRO) SINGLE_MACRO(audio.micPin) \
                           SINGLE_MACRO(audio.fftWindowType) \
                           SINGLE_MACRO(audio.useDCRemoval) \
                           SINGLE_MACRO(audio.lowLevelFilter) \
                           SINGLE_MACRO(audio.midLevelFilter) \
                           SINGLE_MACRO(audio.highLevelFilter) \
                                                                \
                           SINGLE_MACRO(animations.directColor.step) \
                           SINGLE_MACRO(animations.directColor.delay) \
                           COLOR_MACRO(animations.directColor.color) \
                                                                \
                           SINGLE_MACRO(animations.dynamicColor.delay) \
                           COLOR_MACRO(animations.dynamicColor.color) \
                                                                \
                           SINGLE_MACRO(animations.strobe.timeOn) \
                           SINGLE_MACRO(animations.strobe.timeOff) \
                           SINGLE_MACRO(animations.strobe.stepOn) \
                           SINGLE_MACRO(animations.strobe.delayOn) \
                           COLOR_MACRO(animations.strobe.color) \
                                                                \
                           SINGLE_MACRO(animations.rainbow.step) \
                           SINGLE_MACRO(animations.rainbow.period) \
                           SINGLE_MACRO(animations.rainbow.delay) \
                           COLOR_MACRO(animations.rainbow.color) \
                                                                \
                           SINGLE_MACRO(animations.blur.bpm) \
                           COLOR_MACRO(animations.blur.color) \
                                                                \
                           SINGLE_MACRO(animations.dotBeat.bpm) \
                           SINGLE_MACRO(animations.dotBeat.fade) \
                           COLOR_MACRO(animations.dotBeat.middle) \
                           COLOR_MACRO(animations.dotBeat.inner) \
                           COLOR_MACRO(animations.dotBeat.outer) \
                                                                \
                           SINGLE_MACRO(animations.confetti.duration) \
                           SINGLE_MACRO(animations.confetti.delay) \
                           SINGLE_MACRO(animations.confetti.fade) \
                           COLOR_MACRO(animations.confetti.color) \
                                                                            \
                           SINGLE_MACRO(animations.simpleFrequency.coefficient) \
                           SINGLE_MACRO(animations.simpleFrequency.smoothStep) \
                           COLOR_MACRO(animations.simpleFrequency.low) \
                           COLOR_MACRO(animations.simpleFrequency.mid) \
                           COLOR_MACRO(animations.simpleFrequency.high) \
                                                                \
                           SINGLE_MACRO(animations.runningFrequency.coefficient) \
                           SINGLE_MACRO(animations.runningFrequency.latency) \
                           COLOR_MACRO(animations.runningFrequency.low) \
                           COLOR_MACRO(animations.runningFrequency.mid) \
                           COLOR_MACRO(animations.runningFrequency.high) \
                                                                \
                           SINGLE_MACRO(animations.frequencyStrobe.coefficient) \
                           SINGLE_MACRO(animations.frequencyStrobe.mode) \
                           COLOR_MACRO(animations.frequencyStrobe.low) \
                           COLOR_MACRO(animations.frequencyStrobe.mid) \
                           COLOR_MACRO(animations.frequencyStrobe.high) \
                           COLOR_MACRO(animations.frequencyStrobe.empty) \
                                                                          \
                           SINGLE_MACRO(animations.spectrumAnalyzer.startColor) \
                           SINGLE_MACRO(animations.spectrumAnalyzer.step) \
                                                                                  \
                           SINGLE_MACRO(animations.uvMeter.bottomCoefficient) \
                           SINGLE_MACRO(animations.uvMeter.topCoefficient) \
                           SINGLE_MACRO(animations.uvMeter.fadeFactor) \
                           SINGLE_MACRO(animations.uvMeter.mode)

void Settings::set(const String &key, JsonVariant value) {
    ANIMATIONS_MACRO(SETTING_SETTER, COLOR_SETTING_SETTER);
}

void Settings::load() {
    File configFile = SPIFFS.open("/config.json", "r");
    if (!configFile) {
        Serial.println("Failed to open config file");
        save();
        return;
    }
    size_t size = configFile.size();
    if (size > SETTINGS_CAPACITY) {
        Serial.println("Config file size is too large");
        save();
        configFile.close();
        return;
    }

    DynamicJsonDocument document(SETTINGS_CAPACITY);
    DeserializationError error = deserializeJson(document, configFile);
    if (error) {
        Serial.println("Failed to read file, using default configuration");
        save();
        configFile.close();
        return;
    }
    configFile.close();

    if (!document.getMember("animations.currentAnimation").isNull()
        && !document.getMember("animations.currentAnimation").isUndefined()) {
        animations.currentAnimation = document.getMember("animations.currentAnimation").as<String>();
    }
    ANIMATIONS_MACRO(SETTING_LOADER, COLOR_SETTING_LOADER);
}


void Settings::save() {
    SPIFFS.remove("/config.json");

    // Open file for writing
    File configFile = SPIFFS.open("/config.json", "w");
    if (!configFile) {
        Serial.println("Failed to open config file");
        return;
    }

    DynamicJsonDocument document(SETTINGS_CAPACITY);

    document["animations.currentAnimation"] = animations.currentAnimation;
    ANIMATIONS_MACRO(SETTING_SAVER, COLOR_SETTING_SAVER);

    serializeJson(document, configFile);
    configFile.close();
}

void Settings::reset() {
    SPIFFS.remove("/config.json");
    audio = AudioSettings();
    animations = AnimationsSettings();
}