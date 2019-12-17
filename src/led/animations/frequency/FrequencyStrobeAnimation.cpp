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
#include "FrequencyStrobeAnimation.h"

void FrequencyStrobeAnimation::animate() {
    auto settings = Settings::getInstance().animations.frequencyStrobe;
    auto audio = Application::getInstance().getAudioAnalyzer()->analyzeAudio();

    int flashes[3];
    for (int i = 0; i < 3; i++) {
        auto current = audio.bands[i];
        auto avg = audio.filteredBands[i];

        flashes[i] = current > (avg * settings.coefficient);
    }

    switch (settings.mode) {
        case 1:
            if (flashes[2]) drawColor(settings.high);
            else drawColor(settings.empty);
            break;
        case 2:
            if (flashes[1]) drawColor(settings.mid);
            else drawColor(settings.empty);
            break;
        case 3:
            if (flashes[0]) drawColor(settings.low);
            else drawColor(settings.empty);
            break;
        case 0:
        default:
            if (flashes[2]) drawColor(settings.high);
            else if (flashes[1]) drawColor(settings.mid);
            else if (flashes[0]) drawColor(settings.low);
            else drawColor(settings.empty);
            break;
    }

    FastLED.show();
}

void FrequencyStrobeAnimation::drawColor(Settings::AnimationsSettings::ColorSetting color) {
    auto led = Application::getInstance().getLed();
    for (int i = 0; i < LED_LENGTH; i++) {
        led->setColorAt(i, CHSV(color.hue, color.sat, color.val));
    }
}
