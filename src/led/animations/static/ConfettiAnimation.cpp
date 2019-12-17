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
#include "ConfettiAnimation.h"

void ConfettiAnimation::animate(CRGB *strip) {
    auto settings = Settings::getInstance().animations.confetti;

    auto duration = settings.duration;
    auto delay = settings.delay;
    auto fade = settings.fade;
    auto bright = settings.color.val;

    byte secondHand = (millis() / 1000) % duration;
    if (lastSecond != secondHand) {                             // Debounce to make sure we're not repeating an assignment.
        lastSecond = secondHand;
        switch (secondHand) {
            case 0:
                inc = 1;
                hue = 192;
                sat = 255;
                fade = 2;
                diff = 256;
                break;  // You can change values here, one at a time , or altogether.
            case 5:
                inc = 2;
                hue = 128;
                fade = 8;
                diff = 64;
                break;
            case 10:
                inc = 1;
                hue = random16(255);
                fade = 1;
                diff = 16;
                break;      // Only gets called once, and not continuously for the next several seconds. Therefore, no rainbows.
            case 15:
            default:
                break;                                                                // Here's the matching 15 for the other one.
        }
    }

    if (millis() - timer > delay) {
        timer = millis();
        fadeToBlackBy(strip, LED_LENGTH, fade);                             // Low values = slower fade.
        int pos = random16(LED_LENGTH);                                     // Pick an LED at random.
        strip[pos] += CHSV((hue + random16(diff)) / 4, sat, bright);    // I use 12 bits for hue so that the hue increment isn't too quick.
        hue = hue + inc;                                                    // It increments here.
    }

    FastLED.show();
}
