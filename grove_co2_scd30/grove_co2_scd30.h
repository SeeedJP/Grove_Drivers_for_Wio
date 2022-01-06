/*
 * grove_co2_scd30.h
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


#ifndef __GROVE_CO2_SCD30_H__
#define __GROVE_CO2_SCD30_H__

#include "suli2.h"

//GROVE_NAME        "Grove - CO2 (SCD30)"
//SKU               101020634
//IF_TYPE           I2C
//IMAGE_URL         https://media-cdn.seeedstudio.com/media/catalog/product/cache/9d0ce51a71ce6a79dfa2a98d65a0f0bd/p/r/preview_1_1.png
//DESCRIPTION       "The Grove - SCD30 is a 3-in-1 Arduino sensor that can measure CO2, temperature, and humidity. Based on Sensirion SCD30."
//WIKI_URL          https://wiki.seeedstudio.com/Grove-CO2_Temperature_Humidity_Sensor-SCD30/
//ADDED_AT          "2022-01-06"
//AUTHOR            "Seeed K.K."

class GroveCo2SCD30
{
public:
    GroveCo2SCD30(int pinsda, int pinscl);

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
    static constexpr uint8_t I2C_ADDRESS = 0x61 << 1;

    I2C_T *i2c;

	uint8_t CalcCRC(const void* data, int dataSize) const;

	void Write(uint16_t cmd, const uint16_t* data, int dataNumber);
	int Read(uint16_t address, uint16_t* data, int dataNumber);

	void TriggerContinuousMeasurement(uint16_t pressure = 0);
	void StopContinuousMeasurement();
	void SetMeasurementInterval(uint16_t interval);
	int GetMeasurementInterval();
	int GetDataReadyStatus();
	bool ReadyToRead();
	bool ReadMeasurement(float* co2, float* t, float* rh);

};

#endif
