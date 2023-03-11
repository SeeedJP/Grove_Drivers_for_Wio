/*
 * grove_temp_humi_sht41.cpp
 *
 * Copyright (c) 2022 Seeed K.K.
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
#include "grove_temp_humi_sht41.h"

static constexpr uint8_t I2C_ADDRESS = 0x44 << 1;

static constexpr uint8_t CMD_SOFT_RESET         = 0x94;
static constexpr uint8_t CMD_MEAS_HIGH          = 0xfd;
static constexpr uint8_t CMD_HEATER_200MW_1S    = 0x39;
static constexpr uint8_t CMD_HEATER_200MW_100MS = 0x32;
static constexpr uint8_t CMD_HEATER_110MW_1S    = 0x2f;
static constexpr uint8_t CMD_HEATER_110MW_100MS = 0x24;
static constexpr uint8_t CMD_HEATER_20MW_1S     = 0x1e;
static constexpr uint8_t CMD_HEATER_20MW_100MS  = 0x15;

#define POLYNOMIAL              (0x31)

GroveTempHumiSHT41::GroveTempHumiSHT41(int pinsda, int pinscl)
{
    this->i2c = (I2C_T *)malloc(sizeof(I2C_T));
    suli_i2c_init(i2c, pinsda, pinscl);

    on_power_on();
}

bool GroveTempHumiSHT41::on_power_on()
{
    SendCommand(CMD_SOFT_RESET);
    suli_delay_ms(1);

    return true;
}

bool GroveTempHumiSHT41::on_power_off()
{
    return true;
}

bool GroveTempHumiSHT41::read_temperature(float *temperature)
{
    return ReadTempHumi(temperature, NULL);
}

bool GroveTempHumiSHT41::read_humidity(float *humidity)
{
    return ReadTempHumi(NULL, humidity);
}

bool GroveTempHumiSHT41::write_activate_heater(int watt_mw, int time_ms)
{
    return ActivateHeater(watt_mw, time_ms, NULL, NULL);
}

void GroveTempHumiSHT41::SendCommand(uint8_t cmd)
{
    suli_i2c_write(i2c, I2C_ADDRESS, &cmd, sizeof(cmd));
}

bool GroveTempHumiSHT41::ReadTempHumi(float *temperature, float *humidity)
{
    SendCommand(CMD_MEAS_HIGH);
    delay(9);

    uint8_t readData[6];
    if (suli_i2c_read(i2c, I2C_ADDRESS, readData, sizeof(readData)) != 6) return false;

    if (readData[2] != CalcCRC8(&readData[0], 2)) return false;
    if (readData[5] != CalcCRC8(&readData[3], 2)) return false;

    uint16_t ST;
    ST = readData[0];
    ST <<= 8;
    ST |= readData[1];

    uint16_t SRH;
    SRH = readData[3];
    SRH <<= 8;
    SRH |= readData[4];

    if (temperature != NULL) *temperature = (float)ST * 175 / 0xffff - 45;
    if (humidity != NULL) *humidity = (float)SRH * 125 / 0xffff - 6;

    return true;
}

bool GroveTempHumiSHT41::ActivateHeater(int watt_mw, int time_ms, float *temperature, float *humidity)
{
    uint8_t cmd;
    if      (watt_mw == 200 && time_ms == 1000) cmd = CMD_HEATER_200MW_1S   ;
    else if (watt_mw == 200 && time_ms == 100 ) cmd = CMD_HEATER_200MW_100MS;
    else if (watt_mw == 110 && time_ms == 1000) cmd = CMD_HEATER_110MW_1S   ;
    else if (watt_mw == 110 && time_ms == 100 ) cmd = CMD_HEATER_110MW_100MS;
    else if (watt_mw == 20  && time_ms == 1000) cmd = CMD_HEATER_20MW_1S    ;
    else if (watt_mw == 20  && time_ms == 100 ) cmd = CMD_HEATER_20MW_100MS ;
    else return false;

    SendCommand(cmd);
    delay(time_ms * 11 / 10);

    uint8_t readData[6];
    if (suli_i2c_read(i2c, I2C_ADDRESS, readData, sizeof(readData)) != 6) return false;

    if (readData[2] != CalcCRC8(&readData[0], 2)) return false;
    if (readData[5] != CalcCRC8(&readData[3], 2)) return false;

    uint16_t ST;
    ST = readData[0];
    ST <<= 8;
    ST |= readData[1];

    uint16_t SRH;
    SRH = readData[3];
    SRH <<= 8;
    SRH |= readData[4];

    if (temperature != NULL) *temperature = (float)ST * 175 / 0xffff - 45;
    if (humidity != NULL) *humidity = (float)SRH * 125 / 0xffff - 6;

    return true;
}

static uint8_t GroveTempHumiSHT41::CalcCRC8(const uint8_t *data, int len)
{
    uint8_t crc = 0xff;
  
    for (int j = len; j > 0; j--)
    {
        crc ^= *data++;

        for (int i = 8; i > 0; i--)
        {
            crc = crc & 0x80 ? crc << 1 ^ POLYNOMIAL : crc << 1;
        }
    }

    return crc; 
}
