/*
 * grove_co2_scd41.h
 *
 * Copyright (c) 2022 Seeed K.K.
 * Website    : www.seeed.co.jp
 * Author     : Takashi Matsuoka
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


#ifndef __GROVE_CO2_SCD41_H__
#define __GROVE_CO2_SCD41_H__

#include "suli2.h"

//GROVE_NAME        "Grove - CO2 (SCD41)"
//SKU               101020952
//IF_TYPE           I2C
//IMAGE_URL         https://media-cdn.seeedstudio.com/media/catalog/product/cache/9d0ce51a71ce6a79dfa2a98d65a0f0bd/1/0/101020952_preview-07.png
//DESCRIPTION       "The Grove - CO2 & Temperature & Humidity Sensor - SCD41 is a small but powerful module made by Sensirion."
//WIKI_URL          https://wiki.seeedstudio.com/Grove-CO2_&_Temperature_&_Humidity_Sensor-SCD41/
//ADDED_AT          "2022-07-08"
//AUTHOR            "Seeed K.K."

class GroveCo2SCD41
{
public:
    GroveCo2SCD41(int pinsda, int pinscl);

    /**
     * Read values of the envirenment
     *
     * @param concentration - unit: PPM
     * @param temperature - unit: Celsius degree
     * @param humidity - unit: %
     *
     * @return bool
     */
    bool read_measurement(float *concentration, float *temperature, float *humidity);

    /**
     * Read the temperature value of the envirenment
     *
     * @param temperature - unit: Celsius degree
     *
     * @return bool
     */
    bool read_temperature(float *temperature);

    /**
     * Read the humidity value of the envirenment
     *
     * @param humidity - unit: %
     *
     * @return bool
     */
    bool read_humidity(float *humidity);

    /**
     * Read the concentration of CO2 value of the envirenment
     *
     * @param concentration - unit: PPM
     *
     * @return bool
     */
    bool read_concentration(float *concentration);

private:
    static constexpr uint8_t I2C_ADDRESS = 0x62 << 1;

    I2C_T *i2c;

	uint8_t CalcCRC(const void* data, int dataSize) const;

	void Write(uint16_t cmd, const uint16_t* data, int dataNumber, unsigned long commandExecutionTimeMs);
	int Read(uint16_t address, uint16_t* data, int dataNumber, unsigned long commandExecutionTimeMs);

    void StartPeriodicMeasurement();
    bool ReadMeasurement(float* co2, float* t, float* rh);
    void StopPeriodicMeasurement();

};

#endif
