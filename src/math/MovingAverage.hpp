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
#ifndef ESPX_DANCING_LED_MOVINGAVERAGE_HPP
#define ESPX_DANCING_LED_MOVINGAVERAGE_HPP


#include "NoiseFilter.hpp"
#include "CircularBuffer.hpp"

template<class T>
class MovingAverage: public NoiseFilter<T> {
private:
    int size;
    CircularBuffer<T> *buffer;
    T avg = (T)0;
public:
    explicit MovingAverage(int of) {
        size = of;
        buffer = new CircularBuffer<T>(of);
    }

    ~MovingAverage() {
        delete buffer;
    }

    T push(T value) {
        buffer->push(value);
        avg = findAvg();

        return avg;
    }

    T mean() {
        return avg;
    }

private:
    T findAvg() {
        T *values = buffer->all();
        T sum = 0;
        for (int i = 0; i < size; ++i) {
            sum += values[i];
        }

        return sum / size;
    }
};

#endif //ESPX_DANCING_LED_MOVINGAVERAGE_HPP
