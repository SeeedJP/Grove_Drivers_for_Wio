/*
 * grove_temp_humi_sht41.h
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


#ifndef __GROVE_TEMP_HUMI_SHT41_H__
#define __GROVE_TEMP_HUMI_SHT41_H__

#include "suli2.h"

//GROVE_NAME        "Grove - Temperature & Humidity Sensor(SHT41)"
//SKU               101021033
//IF_TYPE           I2C
//IMAGE_URL         https://files.seeedstudio.com/wiki/SHT4x/SHT4x.jpeg
//DESCRIPTION       "This Grove SHT41 digital sensor is based on Sensirion's industry-proven humidity and temperature sensors. It can provide constant temperature and humidity accuracy over an extensive measurement range."
//WIKI_URL          https://wiki.seeedstudio.com/Grove-SHT4x/
//ADDED_AT          "2023-03-11"
//AUTHOR            "Seeed K.K."

class GroveTempHumiSHT41
{
public:
    GroveTempHumiSHT41(int pinsda, int pinscl);
    bool on_power_on();
    bool on_power_off();

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
     * Activate the heater for a specified time
     *
     * @param watt_mw - 20, 110, 200 [milli-watt]
     * @param time_ms - 100, 1000 [milli-seconds]
     *
     * @return bool
     */
    bool write_activate_heater(int watt_mw, int time_ms);

private:
    I2C_T *i2c;

    void SendCommand(uint8_t cmd);
    bool ReadTempHumi(float *temperature, float *humidity);
    bool ActivateHeater(int watt_mw, int time_ms, float *temperature, float *humidity);

    static uint8_t CalcCRC8(const uint8_t *data, int len);

};

#endif
