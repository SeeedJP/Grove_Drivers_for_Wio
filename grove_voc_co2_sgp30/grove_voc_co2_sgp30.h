/*
 * grove_voc_co2_sgp30.h
 *
 * Copyright (c) 2023 Fin Warman
 * Website    : github.com/finwarman
 * Author     : Fin Warman
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

// Datasheet: https://files.seeedstudio.com/wiki/Grove-VOC_and_eCO2_Gas_Sensor-SGP30/res/Sensirion_Gas_Sensors_SGP30_Datasheet_EN.pdf

#ifndef __grove_voc_co2_sgp30_H__
#define __grove_voc_co2_sgp30_H__

#include "suli2.h"

//GROVE_NAME        "Grove - VOC and eCO2 Gas Sensor (SGP30)"
//SKU               101020512
//IF_TYPE           I2C
//IMAGE_URL         https://files.seeedstudio.com/wiki/Grove-VOC_and_eCO2_Gas_Sensor-SGP30/img/IMG_0012a.jpg
//DESCRIPTION       "The Grove - VOC and eCO2 Gas Sensor (SGP30) is a multi-pixel gas sensor for air quality measurements using I2C communication."
//WIKI_URL          http://wiki.seeedstudio.com/Grove-VOC_and_eCO2_Gas_Sensor-SGP30/
//ADDED_AT          "2024-11-03"
//AUTHOR            "Fin Warman"

class GroveVOCSGP30
{
public:
    GroveVOCSGP30(int pinsda, int pinscl);

    /**
     * Initialize the SGP30 sensor
     *
     * @return bool
     */
    bool begin();

    /**
     * Read the TVOC and eCO2 values
     *
     * @param tvoc - Total Volatile Organic Compounds in ppb
     * @param eco2 - Equivalent CO2 in ppm
     *
     * @return bool
     */
    bool read_air_quality(uint16_t *tvoc, uint16_t *eco2);

private:
    I2C_T *i2c;
    bool error;

    bool send_command(uint16_t cmd);
    uint8_t calculate_crc(uint8_t data[], uint8_t length);
};

#endif
