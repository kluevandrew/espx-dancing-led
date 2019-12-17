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
#ifndef ESPX_DANCING_LED_MEDIANFILTEROF3_HPP
#define ESPX_DANCING_LED_MEDIANFILTEROF3_HPP

#include "NoiseFilter.hpp"

template<class T>
class MedianFilterOf3: public NoiseFilter<T>  {
public:
    T filter(T newValue) {
        values[0] = values[1];
        values[1] = values[2];
        values[2] = newValue;

        counter++;
        if (counter < 3) {
            median = newValue;
        } else {
            median = calculateMedian();
        }

        return median;
    }

    T getMedian() {
        return median;
    }

private:
    int counter = 0;
    T median = 0;
    T values[3] = {0, 0, 0};

    int calculateMedian() {
        T a = values[0];
        T b = values[1];
        T c = values[2];

        if ((a <= b) && (a <= c)) {
            return (b <= c) ? b : c;
        }

        if ((b <= a) && (b <= c)) {
            return (a <= c) ? a : c;
        }

        return (a <= b) ? a : b;
    }
};


#endif //ESPX_DANCING_LED_MEDIANFILTEROF3_HPP
