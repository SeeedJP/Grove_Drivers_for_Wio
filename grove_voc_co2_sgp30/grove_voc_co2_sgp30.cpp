/*
 * grove_voc_co2_sgp30.cpp
 *
 * Copyright (c) 2024 Fin Warman
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

#include "suli2.h"
#include "grove_voc_co2_sgp30.h"

#define SGP30_I2C_ADDRESS (0x58 << 1)
#define SGP30_INIT_AIR_QUALITY 0x2003
#define SGP30_MEASURE_AIR_QUALITY 0x2008

GroveVOCSGP30::GroveVOCSGP30(int pinsda, int pinscl)
{
    error = false;
    this->i2c = (I2C_T *)malloc(sizeof(I2C_T));
    suli_i2c_init(i2c, pinsda, pinscl);
    suli_i2c_clock(i2c, 50000);

    if (!begin())
    {
        error = true;
    }
}

bool GroveVOCSGP30::begin()
{
    return send_command(SGP30_INIT_AIR_QUALITY); // Initialize air quality measurement
}

bool GroveVOCSGP30::read_air_quality(uint16_t *tvoc, uint16_t *eco2)
{
    if (error)
        return false;

    // Send the measure air quality command
    if (!send_command(SGP30_MEASURE_AIR_QUALITY))
    {
        return false;
    }

    // Delay to allow measurement to complete
    suli_delay_ms(12);

    uint8_t readData[6];
    if (suli_i2c_read(i2c, SGP30_I2C_ADDRESS, readData, 6) != 6)
    {
        return false;
    }

    // Verify CRC for eCO2
    if (calculate_crc(readData, 2) != readData[2])
    {
        return false; // CRC mismatch
    }

    // Verify CRC for TVOC
    if (calculate_crc(&readData[3], 2) != readData[5])
    {
        return false; // CRC mismatch
    }

    *eco2 = (readData[0] << 8) | readData[1]; // Assign eCO2 value
    *tvoc = (readData[3] << 8) | readData[4]; // Assign TVOC value

    return true;
}

bool GroveVOCSGP30::send_command(uint16_t cmd)
{
    uint8_t data[2] = {(uint8_t)(cmd >> 8), (uint8_t)(cmd & 0xFF)};
    return suli_i2c_write(i2c, SGP30_I2C_ADDRESS, data, 2) == 2;
}

// CRC-8 calculation based on SGP30 datasheet specification
uint8_t GroveVOCSGP30::calculate_crc(uint8_t data[], uint8_t length)
{
    uint8_t crc = 0xFF; // Init as per the datasheet
    for (uint8_t i = 0; i < length; i++)
    {
        crc ^= data[i];
        for (uint8_t bit = 0; bit < 8; bit++)
        {
            if (crc & 0x80)
            {
                crc = (crc << 1) ^ 0x31; // Polynomial 0x31 for CRC-8
            }
            else
            {
                crc <<= 1;
            }
        }
    }
    return crc;
}
