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
#include "DotBeatAnimation.h"

void DotBeatAnimation::animate(CRGB *strip) {
    auto settings = Settings::getInstance().animations.dotBeat;

    byte inner = beatsin8(settings.bpm, LED_LENGTH / 4, LED_LENGTH / 4 * 3);    // Move 1/4 to 3/4
    byte outer = beatsin8(settings.bpm, 0, LED_LENGTH - 1);                     // Move entire length
    byte middle = beatsin8(settings.bpm, LED_LENGTH / 3, LED_LENGTH / 3 * 2);   // Move 1/3 to 2/3

    strip[middle] = CHSV(settings.middle.hue, settings.middle.sat, settings.middle.val);
    strip[inner] = CHSV(settings.inner.hue, settings.inner.sat, settings.inner.val);
    strip[outer] = CHSV(settings.outer.hue, settings.outer.sat, settings.outer.val);

    nscale8(strip, LED_LENGTH, settings.fade);
    FastLED.show();
}

