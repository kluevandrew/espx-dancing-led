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
#include "AnimationFactory.h"
#include "ArduinoJson.h"
#include <led/animations/NoopAnimation.h>
#include <led/animations/static/StrobeAnimation.h>
#include <led/animations/frequency/SimpleFrequencyAnimation.h>
#include <led/animations/frequency/FrequencyStrobeAnimation.h>
#include <led/animations/frequency/RunningFrequencyAnimation.h>
#include <led/animations/frequency/SpectrumAnalyzerFrequencyAnimation.h>
#include <led/animations/static/RainbowAnimation.h>
#include <led/animations/static/DirectColorAnimation.h>
#include <led/animations/static/DynamicColorAnimation.h>
#include <led/animations/static/DotBeatAnimation.h>
#include <led/animations/static/ConfettiAnimation.h>
#include <led/animations/uv/UvMeterAnimation.h>

#define ANIMATION(ANIMATION_NAME, CONSTRUCTOR) if (name.equals(ANIMATION_NAME)) { return new CONSTRUCTOR(); }

LedAnimation *AnimationFactory::create(const String& name) {
    ANIMATION("DirectColor", DirectColorAnimation);
    ANIMATION("DynamicColor", DynamicColorAnimation);
    ANIMATION("Strobe", StrobeAnimation)
    ANIMATION("Rainbow", RainbowAnimation);
    ANIMATION("DotBeat", DotBeatAnimation);
    ANIMATION("Confetti", ConfettiAnimation);
    ANIMATION("SimpleFrequency", SimpleFrequencyAnimation);
    ANIMATION("RunningFrequency", RunningFrequencyAnimation);
    ANIMATION("SpectrumAnalyzerFrequency", SpectrumAnalyzerFrequencyAnimation);
    ANIMATION("UvMeter", UvMeterAnimation);
    ANIMATION("FrequencyStrobe", FrequencyStrobeAnimation);

    return new NoopAnimation();
}
