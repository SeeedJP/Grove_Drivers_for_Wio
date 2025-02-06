/*
 * grove_mp3_v4.cpp
 *
 * Copyright (c) Seeed K.K.
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
#include "grove_mp3_v4.h"

GroveMP3V4::GroveMP3V4(int pintx, int pinrx)
{
    this->uart = (UART_T *)malloc(sizeof(UART_T));
    
    suli_uart_init(uart, pintx, pinrx, 115200);
}

bool GroveMP3V4::write_index_to_play(uint8_t index)
{
    char command[20];
    snprintf(command, sizeof(command), "AT+PLAY=sd0,%d\r", (int)index);
    suli_uart_write_bytes(uart, (uint8_t*)command, strlen(command));

    return true;
}

bool GroveMP3V4::write_stop(void)
{
    char command[] = "AT+STOP\r";
    suli_uart_write_bytes(uart, (uint8_t*)command, strlen(command));

    return true;
}

bool GroveMP3V4::write_volume(uint8_t volume)
{
    if (volume > 31) volume = 31;

    char command[20];
    snprintf(command, sizeof(command), "AT+VOL=%d\r", (int)volume);
    suli_uart_write_bytes(uart, (uint8_t*)command, strlen(command));

    return true;
}
