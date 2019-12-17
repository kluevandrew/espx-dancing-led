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
#include "SimpleFrequencyAnimation.h"
#include <Application.h>
#include <settings/Settings.h>

void SimpleFrequencyAnimation::animate() {
    auto settings = Settings::getInstance().animations.simpleFrequency;
    auto audio = Application::getInstance().getAudioAnalyzer()->analyzeAudio();

    for (int i = 0; i < 3; i++) {
        auto current = audio.bands[i];
        auto avg = audio.filteredBands[i];

        flashes[i] = current > (avg * settings.coefficient);
    }

    renderLine(0, settings.low);
    renderLine(1, settings.mid);
    renderLine(2, settings.high);

    FastLED.show();
}

void SimpleFrequencyAnimation::renderLine(byte index, Settings::AnimationsSettings::ColorSetting color) {
    int ledsInBand = LED_LENGTH / 3;
    auto settings = Settings::getInstance().animations.simpleFrequency;
    auto led = Application::getInstance().getLed();

    for (int i = ledsInBand * index; i < ledsInBand * (index + 1); i++) {
        if (flashes[index]) {
            brights[index] = color.val;
        } else {
            if (brights[index] > 0) {
                brights[index] -= settings.smoothStep;
            }
            brights[index] = constrain(brights[0], 0, color.val);
        }
        led->setColorAt(i, CHSV(color.hue, color.sat, brights[index]));
    }
}