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
#ifndef ESPX_DANCING_MUSIC_AUDIOANALYZER_H
#define ESPX_DANCING_MUSIC_AUDIOANALYZER_H

// SAMPLING_PERIOD is calculated according to (int)round(1000000 * (1.0 / SAMPLING_FREQUENCY))
#define FFT_SAMPLES_COUNT 256
#ifdef ESP32
    #define SAMPLING_FREQUENCY 40000
    #define SAMPLING_PERIOD 25
#else
    #define SAMPLING_FREQUENCY 38000
    #define SAMPLING_PERIOD 26
#endif

#include <math/NoiseFilter.hpp>
#include <math/MedianFilter.hpp>
#include <math/MedianFilterOf3.hpp>
#include <math/MovingAverage.hpp>
#include "arduinoFFT.h"
#include "config/config.h"

class AudioAnalyzer;

class AudioAnalyzer {
public:
    /**
       Constructor
    */
    explicit AudioAnalyzer();

    /**
       Destructor
    */
    ~AudioAnalyzer();

    struct AudioAnalyzeResult {
        unsigned int soundLevel = 0;
        unsigned int avgSoundLevel = 0;
        double majorPeak = 0.0;
        unsigned int bands[3] = {0, 0, 0};
        unsigned int filteredBands[3] = {0, 0, 0};
    };

    AudioAnalyzer* sampling();
    AudioAnalyzeResult analyzeAudio() {
        return analyzeAudio(true);
    }
    AudioAnalyzeResult analyzeAudio(bool needFft);

    const double *getVReal() const;

private:
    arduinoFFT fft;
    double vReal[FFT_SAMPLES_COUNT]{};
    double vImag[FFT_SAMPLES_COUNT]{};

    MedianFilter<unsigned int> filteredBands[3] = {
            MedianFilter<unsigned int>(5),
            MedianFilter<unsigned int>(5),
            MedianFilter<unsigned int>(5),
    };

    MovingAverage<unsigned int> avgSoundLevel = MovingAverage<unsigned int>(3);
};

#endif //ESPX_DANCING_MUSIC_AUDIOANALYZER_H
