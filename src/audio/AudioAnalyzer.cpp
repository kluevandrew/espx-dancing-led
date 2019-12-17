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
#include <Application.h>
#include <settings/Settings.h>
#include "AudioAnalyzer.h"

AudioAnalyzer::AudioAnalyzer() {
    fft = arduinoFFT(vReal, vImag, FFT_SAMPLES_COUNT, SAMPLING_FREQUENCY);
}

AudioAnalyzer::~AudioAnalyzer() = default;


AudioAnalyzer* AudioAnalyzer::sampling() {
    auto settings = Settings::getInstance().audio;
    auto adc = Application::getInstance().getAdc();
    for (int i = 0; i < FFT_SAMPLES_COUNT; i++) {
        auto microseconds = micros();
        vReal[i] = adc->read(settings.micPin);
        vImag[i] = 0;
        while ((micros() - microseconds) < SAMPLING_PERIOD) {}
    };

    return this;
}

AudioAnalyzer::AudioAnalyzeResult AudioAnalyzer::analyzeAudio(bool needFft) {
    auto result = AudioAnalyzer::AudioAnalyzeResult();

    auto settings = Settings::getInstance().audio;
    sampling();

    unsigned int maxValue = 0;
    for (int i = 3; i < (FFT_SAMPLES_COUNT / 2); i++) {
        auto value = (unsigned int)vReal[i];
        maxValue = value > maxValue ? value : maxValue;
    }
    result.soundLevel = maxValue;
    result.avgSoundLevel = avgSoundLevel.push(result.soundLevel);

    if (!needFft) {
        return result;
    }

    fft.Windowing(settings.fftWindowType, FFT_FORWARD);  /* Weigh data */
    fft.Compute(FFT_FORWARD); /* Compute FFT */
    fft.ComplexToMagnitude();
    if (settings.useDCRemoval) {
        fft.DCRemoval();
    }

    result.majorPeak = fft.MajorPeak();

    for (int i = 3; i < (FFT_SAMPLES_COUNT / 2); i++) {
        auto value = (unsigned int)vReal[i];

        if (i <= 20) {
            if (value < settings.lowLevelFilter) {
                continue;
            }
            result.bands[0] = max(result.bands[0], value);
        } else if (i > 20 && i <= 40) {
            if (value < settings.midLevelFilter) {
                continue;
            }
            result.bands[1] = max(result.bands[1], value);
        } else if (i > 40) {
            if (value < settings.highLevelFilter) {
                continue;
            }
            result.bands[2] = max(result.bands[2], value);
        }
    }

    result.filteredBands[0] = filteredBands[0].push(result.bands[0]);
    result.filteredBands[1] = filteredBands[1].push(result.bands[1]);
    result.filteredBands[2] = filteredBands[2].push(result.bands[2]);

    return result;
}

const double *AudioAnalyzer::getVReal() const {
    return vReal;
}
