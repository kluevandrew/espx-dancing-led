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
#ifndef ESPX_DANCING_MUSIC_CONFETTIANIMATION_H
#define ESPX_DANCING_MUSIC_CONFETTIANIMATION_H


#include <Application.h>
#include <led/animations/ConfigurableLedAnimation.hpp>
#include <FastLED.h>

/*
 * @see https://github.com/atuline/FastLED-Demos/blob/master/confetti/confetti.ino
 */
class ConfettiAnimation : public ConfigurableLedAnimation {
public:
    ConfettiAnimation() = default;

    ~ConfettiAnimation() override = default;

    void animate() override {};

    void animate(CRGB *strip) override;

    String getName() override {
        return "Confetti";
    };

private:
    int hue = 50;
    byte sat = 100;
    int diff = 256;
    unsigned long timer = 0;
    byte inc = 1;
    byte lastSecond = 0;
};


#endif //ESPX_DANCING_MUSIC_CONFETTIANIMATION_H
