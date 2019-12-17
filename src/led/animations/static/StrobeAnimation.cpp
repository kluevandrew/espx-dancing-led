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
#include <settings/Settings.h>
#include "StrobeAnimation.h"

void StrobeAnimation::animate() {
    auto settings = Settings::getInstance().animations.strobe;

    if (nextTickAt <= millis()) {
        if (way) {
            byte hueOn = settings.color.hue;
            byte satOn = settings.color.sat;
            byte brightOn = settings.color.val;

            unsigned int timeOn = settings.timeOn;
            byte stepOn = settings.stepOn;
            byte delayOn = settings.delayOn;

            currentBright = currentBright + stepOn;

            if (currentBright >= brightOn) {
                currentBright = brightOn;
                way = false;
                nextTickAt = millis() + timeOn;
            } else {
                nextTickAt = millis() + delayOn;
            }

            FastLED.showColor(CHSV(hueOn, satOn, currentBright));
        } else {
            unsigned int timeOff = settings.timeOff;
            FastLED.showColor(CRGB::Black);
            nextTickAt = millis() + timeOff;
            currentBright = 0;
            way = true;
        }
    }
}


