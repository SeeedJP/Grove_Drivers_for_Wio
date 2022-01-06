/*
 * grove_co2_scd30.cpp
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
#include "grove_co2_scd30.h"

GroveCo2SCD30::GroveCo2SCD30(int pinsda, int pinscl)
{
    this->i2c = (I2C_T *)malloc(sizeof(I2C_T));
    suli_i2c_init(i2c, pinsda, pinscl);
	suli_i2c_clock(i2c, 50000);

	SetMeasurementInterval(2);
	TriggerContinuousMeasurement();
}

bool GroveCo2SCD30::read_temperature(float *temperature)
{
	if (!ReadyToRead()) return false;

	float co2;
	float t;
	float rh;
	if (!ReadMeasurement(&co2, &t, &rh)) return false;

	*temperature = t;

	return true;
}

bool GroveCo2SCD30::read_humidity(float *humidity)
{
	if (!ReadyToRead()) return false;

	float co2;
	float t;
	float rh;
	if (!ReadMeasurement(&co2, &t, &rh)) return false;

	*humidity = rh;

	return true;
}

bool GroveCo2SCD30::read_concentration(float *concentration)
{
	if (!ReadyToRead()) return false;

	float co2;
	float t;
	float rh;
	if (!ReadMeasurement(&co2, &t, &rh)) return false;

	*concentration = co2;

	return true;
}

uint8_t GroveCo2SCD30::CalcCRC(const void* data, int dataSize) const
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

void GroveCo2SCD30::Write(uint16_t cmd, const uint16_t* data, int dataNumber)
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
}

int GroveCo2SCD30::Read(uint16_t address, uint16_t* data, int dataNumber)
{
	const int writeDataSize{ 2 };
	uint8_t writeData[writeDataSize];
	writeData[0] = address >> 8;
	writeData[1] = address;

	suli_i2c_write(i2c, I2C_ADDRESS, writeData, writeDataSize);

	const int readDataSize{ 3 * dataNumber };
	uint8_t readData[readDataSize];

	suli_delay_ms(3);

	if (suli_i2c_read(i2c, I2C_ADDRESS, readData, readDataSize) != readDataSize) return 0;

	for (int i = 0; i < dataNumber; ++i)
	{
		if (CalcCRC(&readData[i * 3 + 0], 2) != readData[i * 3 + 2]) return i;
		data[i] = readData[i * 3 + 0] << 8 | readData[i * 3 + 1];
	}

	return dataNumber;
}

void GroveCo2SCD30::TriggerContinuousMeasurement(uint16_t pressure)
{
	Write(0x0010, &pressure, 1);
}

void GroveCo2SCD30::StopContinuousMeasurement()
{
	Write(0x0104, nullptr, 0);
}

void GroveCo2SCD30::SetMeasurementInterval(uint16_t interval)
{
	Write(0x4600, &interval, 1);
}

int GroveCo2SCD30::GetMeasurementInterval()
{
	uint16_t interval;

	if (Read(0x4600, &interval, 1) != 1) return -1;

	return interval;
}

int GroveCo2SCD30::GetDataReadyStatus()
{
	uint16_t dataReady;

	if (Read(0x0202, &dataReady, 1) != 1) return -1;

	return dataReady;
}

bool GroveCo2SCD30::ReadyToRead()
{
	return GetDataReadyStatus() == 1 ? true : false;
}

bool GroveCo2SCD30::ReadMeasurement(float* co2, float* t, float* rh)
{
	uint16_t data[6];

	if (Read(0x0300, data, sizeof(data) / sizeof(data[0])) != sizeof(data) / sizeof(data[0])) return false;
	uint32_t tmp;
	tmp = data[0] << 16 | data[1];
	*co2 = *(float*)&tmp;
	tmp = data[2] << 16 | data[3];
	*t = *(float*)&tmp;
	tmp = data[4] << 16 | data[5];
	*rh = *(float*)&tmp;

	return true;
}
