/*
 * grove_sunlight_v2.h
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

#ifndef __GROVE_SUNLIGHT_V2_H__
#define __GROVE_SUNLIGHT_V2_H__

#include "suli2.h"
#include "Si115X.h"

//GROVE_NAME        "Grove - Sunlight Sensor v2.0"
//SKU               101020089-v2
//IF_TYPE           I2C
//IMAGE_URL         https://raw.githubusercontent.com/Seeed-Studio/Grove_Drivers_for_Wio/static/images/grove-sunlight.jpg
//DESCRIPTION       "Grove - Sunlight Sensor is a multi-channel digital light sensor, which has the ability to detect visible light and infrared light"
//WIKI_URL          http://wiki.seeedstudio.com/Grove-Sunlight_Sensor/
//ADDED_AT          "2024-04-30"
//AUTHOR            "Seeed K.K."

class GroveSI115X
{
public:
    GroveSI115X(int pinsda, int pinscl);

    /**
     * Read visible light
     *
     * @param VL - unit: Lumen
     *
     * @return bool
     */
    bool read_visiblelight(float *VL);

    /**
     * Read IR Value
     *
     * @param IR - unit: Lumen
     *
     * @return bool
     */
    bool read_IR(float *IR);

 private:
    I2C_T *i2c;
    Si115X si115x;

};

#endif
