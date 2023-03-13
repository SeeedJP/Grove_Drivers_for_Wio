/*
 * grove_adc_ads1115.h
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


#ifndef __GROVE_ADC_ADS1115_H__
#define __GROVE_ADC_ADS1115_H__

#include "suli2.h"

//GROVE_NAME        "Grove - 4-Channel 16-bit ADC(ADS1115)"
//SKU               109020041
//IF_TYPE           I2C
//IMAGE_URL         https://files.seeedstudio.com/wiki/Grove-16bit-ADC-ADS1115/ADS1115-45.png
//DESCRIPTION       "Grove - 4-Channel 16-bit ADC(ADS1115) based on Texas Instrument ADS1115, which is a high-precision, low-power, 16-bit ADC chip. It supports 4 single ended inputs or 2 differential inputs."
//WIKI_URL          https://wiki.seeedstudio.com/Grove-16-bit-ADC-ADS1115/
//ADDED_AT          "2023-03-07"
//AUTHOR            "Seeed K.K."

class GroveAdcADS1115
{
public:
    GroveAdcADS1115(int pinsda, int pinscl);

    /**
     * Read the 16bit raw value from A0.
     *
     * @param value - Raw value, range is -32768..32767
     *
     * @return bool
     */
    bool read_a0_raw(int16_t* value);

    /**
     * Read the voltage from A0.
     *
     * @param volt - voltage
     *
     * @return bool
     */
    bool read_a0_voltage(float* volt);

    /**
     * Read the 16bit raw value from A1.
     *
     * @param value - Raw value, range is -32768..32767
     *
     * @return bool
     */
    bool read_a1_raw(int16_t* value);

    /**
     * Read the voltage from A1.
     *
     * @param volt - voltage
     *
     * @return bool
     */
    bool read_a1_voltage(float* volt);

    /**
     * Read the 16bit raw value from A2.
     *
     * @param value - Raw value, range is -32768..32767
     *
     * @return bool
     */
    bool read_a2_raw(int16_t* value);

    /**
     * Read the voltage from A2.
     *
     * @param volt - voltage
     *
     * @return bool
     */
    bool read_a2_voltage(float* volt);

    /**
     * Read the 16bit raw value from A3.
     *
     * @param value - Raw value, range is -32768..32767
     *
     * @return bool
     */
    bool read_a3_raw(int16_t* value);

    /**
     * Read the voltage from A3.
     *
     * @param volt - voltage
     *
     * @return bool
     */
    bool read_a3_voltage(float* volt);

private:
    static constexpr uint8_t I2C_ADDRESS = 0x48 << 1;

    I2C_T *i2c;

    void WriteToRegister(int address, uint16_t data);
    int ReadFromRegister(int address);

    bool ReadRaw(int channel, int16_t* value);
    bool ReadVoltage(int channel, float* volt);

};

#endif
