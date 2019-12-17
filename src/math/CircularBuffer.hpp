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
#ifndef ESPX_DANCING_LED_CIRCULARBUFFER_HPP
#define ESPX_DANCING_LED_CIRCULARBUFFER_HPP

template<class T>
class CircularBuffer {
private:
    unsigned int size;
    T *values;
public:
    CircularBuffer(unsigned int n) {
        size = n;
        values = new T[n];
    }

    ~CircularBuffer() {
        delete[] values;
    }

    void push(T value) {
        for (int i = 1; i < size; ++i) {
            values[i - 1] = values[i];
        }

        values[size - 1] = value;
    }

    T *all() {
        return values;
    }
};


#endif //ESPX_DANCING_LED_CIRCULARBUFFER_HPP
