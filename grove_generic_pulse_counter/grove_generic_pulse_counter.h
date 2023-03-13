/*
 * grove_generic_pulse_counter.h
 *
 * Copyright (c) 2023 Seeed K.K.
 * Website    : www.seeed.co.jp
 * Author     : MATSUOKA Takashi
 *
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */


#ifndef __GENERIC_PULSE_COUNTER_H__
#define __GENERIC_PULSE_COUNTER_H__

#include "suli2.h"

//GROVE_NAME        "Generic Pulse Counter"
//SKU               48d765f4-f579-4590-a76c-d5d51de298d3
//IF_TYPE           GPIO
//IMAGE_URL         https://raw.githubusercontent.com/Seeed-Studio/Grove_Drivers_for_Wio/static/images/wio_generic_din.png
//DESCRIPTION       "The Generic Pulse Counter accumulates the number of pulses from digital in."
//WIKI_URL          https://github.com/Seeed-Studio/Grove_Drivers_for_Wio/wiki/Grove_Generic_Digital_In
//ADDED_AT          "2023-03-13"
//AUTHOR            "Seeed K.K."

class GenericPulseCounter
{
public:
    GenericPulseCounter(int pin);

    bool read_count(uint32_t* elapsed, uint32_t* count, bool forceReset);

private:
    IO_T* io;

    uint32_t time;
    uint32_t pulseCount;

public:
    void IncrementPulseCount();

};

#endif
