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
#include "SpectrumAnalyzerFrequencyAnimation.h"
#include <Application.h>
#include <settings/Settings.h>

void SpectrumAnalyzerFrequencyAnimation::animate() {
    auto settings = Settings::getInstance().animations.spectrumAnalyzer;
    byte startColor = settings.startColor;
    byte step = settings.step;

    auto led = Application::getInstance().getLed();
    auto audioAnalyzer = Application::getInstance().getAudioAnalyzer();
    auto vReal = audioAnalyzer->sampling()->getVReal();

    double linesPerLed = 125.0 / (LED_LENGTH / 2.0);

    int minFrequency = 0;
    int maxFrequency = 0;
    double lineNo = 3;
    int values[LED_LENGTH / 2];
    for (int & value : values) {
        if (linesPerLed >= 1.0) {
            int sum = 0;
            for (int j = 0; j < (int)linesPerLed; ++j) {
                sum += (int)vReal[(int)lineNo];
                lineNo++;
            }
            value = (int)floor(sum / linesPerLed);
        } else {
            value = (int)vReal[(int)lineNo];
            lineNo += linesPerLed;
        }

        minFrequency = value < minFrequency ? value : minFrequency;
        maxFrequency = value > maxFrequency ? value : maxFrequency;
    }

    byte hueIndex = startColor;
    for (int i = 0; i < LED_LENGTH / 2; i++) {
        auto value = values[i];

        byte brightness = 0;
        if (value >= 1000) {
          brightness = map(
                  value,
                  0,
                  maxFrequency,
                  0,
                  255
          );
        }

        led->setColorAt(i, CHSV(hueIndex, 255, brightness));
        led->setColorAt(LED_LENGTH - i - 1, led->getColorAt(i));

        hueIndex += step;
        if (hueIndex > 255) {
            hueIndex = 0;
        }
    }

    FastLED.show();
}
