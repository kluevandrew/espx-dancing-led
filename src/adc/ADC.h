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
#ifndef ESPX_DANCING_MUSIC_ADC_H
#define ESPX_DANCING_MUSIC_ADC_H

#ifndef ESP32
#include <MCP3208.h>
#endif
#include <config/config.h>

class ADC {
public:
    int read(int channel);

private:
#ifndef ESP32
    MCP3208 mcp3208 = MCP3208(ADC_CLOCK_PIN, ADC_MOSI_PIN, ADC_MISO_PIN, ADC_CS_PIN);
#endif
};


#endif //ESPX_DANCING_MUSIC_ADC_H
