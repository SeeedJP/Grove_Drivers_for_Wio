/*
 * grove_generic_pulse_counter.cpp
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

#include "suli2.h"
#include "grove_generic_pulse_counter.h"

static void PinHandler(void* context)
{
    GenericPulseCounter* g = (GenericPulseCounter*)context;
    g->IncrementPulseCount();
}

GenericPulseCounter::GenericPulseCounter(int pin)
{
    io = (IO_T*)malloc(sizeof(IO_T));
    suli_pin_init(io, pin, SULI_INPUT);

    time = millis();
    pulseCount = 0;

    suli_pin_attach_interrupt_handler(io, &PinHandler, SULI_RISE, this);
}

bool GenericPulseCounter::read_count(uint32_t* elapsed, uint32_t* count, bool forceReset)
{
    *elapsed = millis() - time;
    *count = pulseCount;

    if (forceReset)
    {
        time = millis();
        pulseCount = 0;
    }

    return true;
}

void GenericPulseCounter::IncrementPulseCount()
{
    ++pulseCount;
}
