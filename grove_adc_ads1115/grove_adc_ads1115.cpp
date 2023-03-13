/*
 * grove_adc_ads1115.cpp
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
#include "grove_adc_ads1115.h"

static constexpr int CONFIG_CONVERSION  = 0;
static constexpr int CONFIG_ADDRESS     = 1;

static constexpr uint16_t CONFIG_OS_START           = 1 << 15;
static constexpr uint16_t CONFIG_MUX_AIN0_AIN1      = 0b000 << 12;
static constexpr uint16_t CONFIG_MUX_AIN0_AIN3      = 0b001 << 12;
static constexpr uint16_t CONFIG_MUX_AIN1_AIN3      = 0b010 << 12;
static constexpr uint16_t CONFIG_MUX_AIN2_AIN3      = 0b011 << 12;
static constexpr uint16_t CONFIG_MUX_AIN0_GND       = 0b100 << 12;
static constexpr uint16_t CONFIG_MUX_AIN1_GND       = 0b101 << 12;
static constexpr uint16_t CONFIG_MUX_AIN2_GND       = 0b110 << 12;
static constexpr uint16_t CONFIG_MUX_AIN3_GND       = 0b111 << 12;
static constexpr uint16_t CONFIG_PGA_6144           = 0b000 << 9;
static constexpr uint16_t CONFIG_PGA_4096           = 0b001 << 9;
static constexpr uint16_t CONFIG_PGA_2048           = 0b010 << 9;
static constexpr uint16_t CONFIG_PGA_1024           = 0b011 << 9;
static constexpr uint16_t CONFIG_PGA_0512           = 0b100 << 9;
static constexpr uint16_t CONFIG_PGA_0256           = 0b101 << 9;
static constexpr uint16_t CONFIG_MODE_CONTINUOUS    = 0 << 8;
static constexpr uint16_t CONFIG_MODE_SINGLESHOT    = 1 << 8;
static constexpr uint16_t CONFIG_DR_8SPS            = 0b000 << 5;
static constexpr uint16_t CONFIG_DR_16SPS           = 0b001 << 5;
static constexpr uint16_t CONFIG_DR_32SPS           = 0b010 << 5;
static constexpr uint16_t CONFIG_DR_64SPS           = 0b011 << 5;
static constexpr uint16_t CONFIG_DR_128SPS          = 0b100 << 5;
static constexpr uint16_t CONFIG_DR_250SPS          = 0b101 << 5;
static constexpr uint16_t CONFIG_DR_475SPS          = 0b110 << 5;
static constexpr uint16_t CONFIG_DR_860SPS          = 0b111 << 5;
static constexpr uint16_t CONFIG_COMP_DISABLE       = 0b00011 << 0;

GroveAdcADS1115::GroveAdcADS1115(int pinsda, int pinscl)
{
    this->i2c = (I2C_T*)malloc(sizeof(I2C_T));
    suli_i2c_init(this->i2c, pinsda, pinscl);
}

bool GroveAdcADS1115::read_a0_raw(int16_t* value)
{
    return ReadRaw(0, value);
}

bool GroveAdcADS1115::read_a0_voltage(float* volt)
{
    return ReadVoltage(0, volt);
}

bool GroveAdcADS1115::read_a1_raw(int16_t* value)
{
    return ReadRaw(1, value);
}

bool GroveAdcADS1115::read_a1_voltage(float* volt)
{
    return ReadVoltage(1, volt);
}

bool GroveAdcADS1115::read_a2_raw(int16_t* value)
{
    return ReadRaw(2, value);
}

bool GroveAdcADS1115::read_a2_voltage(float* volt)
{
    return ReadVoltage(2, volt);
}

bool GroveAdcADS1115::read_a3_raw(int16_t* value)
{
    return ReadRaw(3, value);
}

bool GroveAdcADS1115::read_a3_voltage(float* volt)
{
    return ReadVoltage(3, volt);
}

void GroveAdcADS1115::WriteToRegister(int address, uint16_t data)
{
    uint8_t writeData[3];
    writeData[0] = address;
    writeData[1] = data >> 8;
    writeData[2] = data;

    suli_i2c_write(i2c, I2C_ADDRESS, writeData, sizeof(writeData));
}

int GroveAdcADS1115::ReadFromRegister(int address)
{
    uint8_t writeData[1];
    writeData[0] = address;

    suli_i2c_write(i2c, I2C_ADDRESS, writeData, sizeof(writeData));

    uint8_t readData[2];

    if (suli_i2c_read(i2c, I2C_ADDRESS, readData, sizeof(readData)) != sizeof(readData)) return -1;

    return readData[0] << 8 | readData[1];
}

bool GroveAdcADS1115::ReadRaw(int channel, int16_t* value)
{
    uint16_t configMux;
    switch (channel)
    {
    case 0:
        configMux = CONFIG_MUX_AIN0_GND;
        break;
    case 1:
        configMux = CONFIG_MUX_AIN1_GND;
        break;
    case 2:
        configMux = CONFIG_MUX_AIN2_GND;
        break;
    case 3:
        configMux = CONFIG_MUX_AIN3_GND;
        break;
    default:
        return false;
    }

    WriteToRegister(CONFIG_ADDRESS, CONFIG_OS_START | configMux | CONFIG_PGA_4096 | CONFIG_MODE_SINGLESHOT | CONFIG_DR_8SPS);

    unsigned long startTime = suli_millis();
    for (;;)
    {
        int config = ReadFromRegister(CONFIG_ADDRESS);
        if (config < 0) return false;
        if ((config & (1 << 15)) != 0) break;   // Performed conversion

        if (suli_millis() - startTime >= 200) return false; // Timeout
    }

    int result = ReadFromRegister(CONFIG_CONVERSION);
    if (result < 0) return false;

    *value = result < 0x8000 ? result : -(0x10000 - result);

    return true;
}

bool GroveAdcADS1115::ReadVoltage(int channel, float* volt)
{
    int16_t value;
    if (!ReadRaw(channel, &value)) return false;

    *volt = (float)value * 4.096 / 32768;

    return true;
}
