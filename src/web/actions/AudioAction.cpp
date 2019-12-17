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
#include "AudioAction.h"

void AudioAction::operator()(AsyncWebServerRequest *request) {
    auto analyzer = Application::getInstance().getAudioAnalyzer();
    auto audio = analyzer->analyzeAudio();

    DynamicJsonDocument response(10240);

    response["lastMajorPeak"] = audio.majorPeak;
    response["samplingFrequency"] = SAMPLING_FREQUENCY;
    response["samplingPeriodUS"] = SAMPLING_PERIOD;

    auto bandsJson = response.createNestedArray("bands");
    for (auto band : audio.bands) {
        bandsJson.add(band);
    }

    auto filteredBandsJson = response.createNestedArray("filteredBands");
    for (auto filteredBand : audio.filteredBands) {
        filteredBandsJson.add(filteredBand);
    }

    if (request->hasParam("vReal")) {
        auto vReal = analyzer->getVReal();
        auto vRealJson = response.createNestedObject("vReal");
        for (int i = 0; i < FFT_SAMPLES_COUNT / 2; i++) {
            auto abscissa = ((i * 1.0 * SAMPLING_FREQUENCY) / FFT_SAMPLES_COUNT);
            String key = "f_" + String(i) + "_" + String(abscissa) + "Hz";
            vRealJson[key] = vReal[i];
        }
    }

    json(request, response);
}

