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
#ifndef ESPX_DANCING_LED_MEDIANFILTER_HPP
#define ESPX_DANCING_LED_MEDIANFILTER_HPP

#include "NoiseFilter.hpp"
#include "CircularBuffer.hpp"
#include "QuickSortLib.h"

template<class T>
class MedianFilter: public NoiseFilter<T>  {
private:
    unsigned int size;
    unsigned int counter = 0;
    CircularBuffer<T> *buffer;
    T median = (T)0;
public:
    explicit MedianFilter(int of) {
        size = of;
        buffer = new CircularBuffer<T>(of);
    }

    ~MedianFilter() {
        delete buffer;
    }

    T push(T value) override {
        buffer->push(value);
        if (counter < size) {
            counter++;
            median = findMedian(slice(), counter);
        } else {
            median = findMedian(buffer->all(), size);
        }

        return median;
    }

    T mean() override {
        return median;
    }

private:
    T* slice() {
        T *filled = new T[counter];
        T* all = buffer->all();
        for (int i = 0; i < counter; ++i) {
            filled[i] = all[i];
        }

        return filled;
    }
    T findMedian(T values[], unsigned int n)
    {
        QuickSort<T>::SortAscending(values, 0, n - 1);

        // check for even case
        if (n % 2 != 0)
            return values[n/2];

        return (T)(values[(n-1)/2] + values[n/2]) / 2.0;
    }
};


#endif //ESPX_DANCING_LED_MEDIANFILTER_HPP
