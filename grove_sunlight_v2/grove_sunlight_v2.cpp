/*
 * grove_sunlight_v2.cpp
 *
 * Copyright (c) Seeed K.K.
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
#include "grove_sunlight_v2.h"
#include "Si115X.h"

GroveSI115X::GroveSI115X(int pinsda, int pinscl)
{
    this->i2c = (I2C_T *)malloc(sizeof(I2C_T));
    suli_i2c_init(this->i2c, pinsda, pinscl);
    si115x.Begin(i2c);

    uint8_t conf[4];
    conf[0] = 0b01100000;   // ADCCONFIGx
    conf[1] = 0b00000000;   // ADCSENSx
    conf[2] = 0b00100000;   // ADCPOSTx
    conf[3] = 0b00000000;   // MEASCONFIGx
    si115x.config_channel(0, conf);
    conf[0] = 0b01101011;   // ADCCONFIGx
    conf[1] = 0b00000000;   // ADCSENSx
    conf[2] = 0b00100000;   // ADCPOSTx
    conf[3] = 0b00000000;   // MEASCONFIGx
    si115x.config_channel(1, conf);
}

bool GroveSI115X::read_visiblelight(float *VL)
{
    *VL = si115x.ReadVisible();
    return true;
}

bool GroveSI115X::read_IR(float *IR)
{
    *IR = si115x.ReadIR();
    return true;
}
