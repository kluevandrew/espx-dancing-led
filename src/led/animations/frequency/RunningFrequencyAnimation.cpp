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
#include "RunningFrequencyAnimation.h"
#include <Application.h>
#include <settings/Settings.h>

void RunningFrequencyAnimation::animate(CRGB *strip) {
    auto settings = Settings::getInstance().animations.runningFrequency;
    auto audio = Application::getInstance().getAudioAnalyzer()->analyzeAudio();

    byte flashLine = 0;
    unsigned int maxFlashValue = 0;
    for (int i = 0; i < 3; i++) {
        auto current = audio.bands[i];
        auto avg = audio.filteredBands[i];

        if (current > (avg * settings.coefficient) && audio.bands[i] > maxFlashValue) {
            maxFlashValue = audio.bands[i];
            flashLine = i + 1;
        }
    }

    int middle = LED_LENGTH / 2;

    if (flashLine > 0) {
        Settings::AnimationsSettings::ColorSetting color{};
        switch (flashLine) {
            case 1:
                color = settings.low;
                break;
            case 2:
                color = settings.mid;
                break;
            case 3:
            default:
                color = settings.high;
                break;
        }
        strip[middle] = CHSV(color.hue, color.sat, color.val);
        strip[middle - 1] = strip[middle];
    }

    if (millis() - stepTimer > settings.latency) {
        stepTimer = (long)millis();
        for (int i = 0; i < middle - 1; i++) {
            strip[i] = strip[i + 1];
            strip[LED_LENGTH - i - 1] = strip[i];
        }

        strip[middle] = CRGB::Black;
        strip[middle - 1] = CRGB::Black;
    }

    FastLED.show();
}
