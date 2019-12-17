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
#include <./led/animations/static/DirectColorAnimation.h>
#include <./led/animations/frequency/SimpleFrequencyAnimation.h>
#include "Led.h"
#include "AnimationFactory.h"

Led::Led() : animation(AnimationFactory::create(Settings::getInstance().animations.currentAnimation))  {
    CFastLED::addLeds<LED_TYPE, LED_DATA_PIN, LED_COLOR_ORDER>(strip, LED_LENGTH);
#ifdef FASTLED_POWER_LIMIT
    if (FASTLED_POWER_LIMIT > 0) {
        FastLED.setMaxPowerInVoltsAndMilliamps(5, FASTLED_POWER_LIMIT);
    }
#endif
    FastLED.showColor(CRGB::Black);
}

void Led::animate() {
    if (animation) {
        animation->animate(strip);
    }
}

void Led::setColorAt(int index, const CRGB &color) {
    strip[index] = color;
}

void Led::setColorAt(int index, const CHSV &color) {
    strip[index] = color;
}

CRGB Led::getColorAt(int index) {
    return strip[index];
}

LedAnimation *Led::getAnimation() const {
    return animation;
}

void Led::setAnimation(LedAnimation *newAnimation) {
    Settings::getInstance().animations.currentAnimation = newAnimation->getName();
    Settings::getInstance().save();
    FastLED.clear(true);
    delete this->animation;
    this->animation = newAnimation;
}

void Led::setAnimation(String &name) {
    setAnimation(AnimationFactory::create(name));
}
