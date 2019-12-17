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
#include "DirectColorAnimation.h"

#define DO_STEP(CURRENT_NAME, NAME) if (CURRENT_NAME > NAME) { \
                                        if ((CURRENT_NAME - step) < NAME) { \
                                            CURRENT_NAME = NAME; \
                                        } else { \
                                            CURRENT_NAME -= step; \
                                        } \
                                    } else if (CURRENT_NAME < NAME) { \
                                        if ((CURRENT_NAME + step) > NAME) { \
                                            CURRENT_NAME = NAME; \
                                        } else { \
                                            CURRENT_NAME += step; \
                                        } \
                                    }

void DirectColorAnimation::animate() {
    auto settings = Settings::getInstance().animations.directColor;
    unsigned long delay = max(10, settings.delay);
    if (millis() - timer <= delay) {
        return;
    }
    timer = millis();

    byte color = settings.color.hue;
    byte saturation = settings.color.sat;
    byte bright = settings.color.val;
    byte step = max(1, settings.step);

    if (currentColor != color || currentSaturation != saturation || currentBright != bright) {
        DO_STEP(currentColor, color);
        DO_STEP(currentSaturation, saturation);
        DO_STEP(currentBright, bright);

        FastLED.showColor(CHSV(currentColor, currentSaturation, currentBright));
    }
}
