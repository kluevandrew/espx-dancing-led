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
#include "RainbowAnimation.h"
#include <Application.h>
#include <settings/Settings.h>

void RainbowAnimation::animate() {
    auto settings = Settings::getInstance().animations.rainbow;
    auto step = settings.step;
    auto period = settings.period;
    auto delay = settings.delay;
    byte saturation = settings.color.sat;
    byte bright = settings.color.val;

    auto led = Application::getInstance().getLed();

    if (millis() - timer > delay) {
        timer = millis();
        color += period;
        if (color > 255) {
            color = 0;
        }
        if (color < 0) {
            color = 255;
        }
    }
    steps = color;
    for (int i = 0; i < LED_LENGTH; i++) {
        led->setColorAt(i, CHSV(steps, saturation, bright));
        steps += step;
        if (steps > 255) {
            steps = 0;
        }
        if (steps < 0) {
            steps = 255;
        }
    }

    FastLED.show();
}
