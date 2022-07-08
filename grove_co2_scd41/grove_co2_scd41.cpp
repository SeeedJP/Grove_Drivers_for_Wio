/*
 * grove_co2_scd41.cpp
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

#include "suli2.h"
#include "grove_co2_scd41.h"

GroveCo2SCD41::GroveCo2SCD41(int pinsda, int pinscl)
{
    this->i2c = (I2C_T *)malloc(sizeof(I2C_T));
    suli_i2c_init(i2c, pinsda, pinscl);
	suli_i2c_clock(i2c, 100000);

	StartPeriodicMeasurement();
}

bool GroveCo2SCD41::read_measurement(float *concentration, float *temperature, float *humidity)
{
	float co2;
	float t;
	float rh;
	if (!ReadMeasurement(&co2, &t, &rh)) return false;

	*concentration = co2;
	*temperature = t;
	*humidity = rh;

	return true;
}


bool GroveCo2SCD41::read_temperature(float *temperature)
{
	float co2;
	float t;
	float rh;
	if (!ReadMeasurement(&co2, &t, &rh)) return false;

	*temperature = t;

	return true;
}

bool GroveCo2SCD41::read_humidity(float *humidity)
{
	float co2;
	float t;
	float rh;
	if (!ReadMeasurement(&co2, &t, &rh)) return false;

	*humidity = rh;

	return true;
}

bool GroveCo2SCD41::read_concentration(float *concentration)
{
	float co2;
	float t;
	float rh;
	if (!ReadMeasurement(&co2, &t, &rh)) return false;

	*concentration = co2;

	return true;
}

uint8_t GroveCo2SCD41::CalcCRC(const void* data, int dataSize) const
{
	uint8_t crc = 0xff;

	for (int i = 0; i < dataSize; ++i)
	{
		crc ^= static_cast<const uint8_t*>(data)[i];

		for (int bit = 8; bit > 0; --bit)
		{
			if (crc & 0x80)
			{
				crc = (crc << 1) ^ 0x31;  // x^8 + x^5 + x^4 + 1
			}
			else
			{
				crc = (crc << 1);
			}
		}
	}

	return crc;
}

void GroveCo2SCD41::Write(uint16_t cmd, const uint16_t* data, int dataNumber, unsigned long commandExecutionTimeMs)
{
	const int writeDataSize{ 2 + 3 * dataNumber };
	uint8_t writeData[writeDataSize];
	writeData[0] = cmd >> 8;
	writeData[1] = cmd;
	for (int i = 0; i < dataNumber; ++i)
	{
		writeData[2 + i * 3 + 0] = data[i] >> 8;
		writeData[2 + i * 3 + 1] = data[i];
		writeData[2 + i * 3 + 2] = CalcCRC(&writeData[2 + i * 3 + 0], 2);
	}

	suli_i2c_write(i2c, I2C_ADDRESS, writeData, writeDataSize);
	if (commandExecutionTimeMs >= 1) suli_delay_ms(commandExecutionTimeMs);
}

int GroveCo2SCD41::Read(uint16_t address, uint16_t* data, int dataNumber, unsigned long commandExecutionTimeMs)
{
	const int writeDataSize{ 2 };
	uint8_t writeData[writeDataSize];
	writeData[0] = address >> 8;
	writeData[1] = address;

	suli_i2c_write(i2c, I2C_ADDRESS, writeData, writeDataSize);
	if (commandExecutionTimeMs >= 1) suli_delay_ms(commandExecutionTimeMs);

	const int readDataSize{ 3 * dataNumber };
	uint8_t readData[readDataSize];

	if (suli_i2c_read(i2c, I2C_ADDRESS, readData, readDataSize) != readDataSize) return 0;

	for (int i = 0; i < dataNumber; ++i)
	{
		if (CalcCRC(&readData[i * 3 + 0], 2) != readData[i * 3 + 2]) return i;
		data[i] = readData[i * 3 + 0] << 8 | readData[i * 3 + 1];
	}

	return dataNumber;
}

void GroveCo2SCD41::StartPeriodicMeasurement()
{
	Write(0x21b1, nullptr, 0, 0);
}

bool GroveCo2SCD41::ReadMeasurement(float* co2, float* t, float* rh)
{
	uint16_t data[3];

	if (Read(0xec05, data, sizeof(data) / sizeof(data[0]), 1) != sizeof(data) / sizeof(data[0])) return false;
	*co2 = data[0];
	*t = (float)(175 * data[1]) / 0xffff - 45;
	*rh = (float)(100 * data[2]) / 0xffff;

	return true;
}

void GroveCo2SCD41::StopPeriodicMeasurement()
{
	Write(0x3f86, nullptr, 0, 500);
}
