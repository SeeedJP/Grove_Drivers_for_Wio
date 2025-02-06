/*
 * grove_mp3_v4.h
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


#ifndef __GROVE_MP3V4_H__
#define __GROVE_MP3V4_H__

#include "suli2.h"

//GROVE_NAME        "Grove - MP3 v4.0"
//SKU               107020129
//IF_TYPE           UART
//IMAGE_URL         https://files.seeedstudio.com/wiki/grove-mp3-v4/0.jpg
//DESCRIPTION       "This MP3 module is an integrated circuit widely utilized in the field of audio playback, designed to deliver high-quality audio decoding and playback capabilities. Typically employed in conjunction with embedded systems or electronic devices, this module facilitates the playback of audio files, such as music files in the MP3 format."
//WIKI_URL          https://wiki.seeedstudio.com/grove_mp3_v4/
//ADDED_AT          "2025-02-06"
//AUTHOR            "Seeed K.K."

class GroveMP3V4
{
public:
    GroveMP3V4(int pintx, int pinrx);

    /**
     * Play a music specified by its index.
     *
     * @param index - 1~n
     *
     * @return bool
     */
    bool write_index_to_play(uint8_t index);

    /**
     * Stop playing the currently playing music.
     *
     * @return bool
     */
    bool write_stop(void);

    /**
     * Tune the volume.
     *
     * @param volume - 0~31
     *
     * @return bool
     */
    bool write_volume(uint8_t volume);

private:
    UART_T *uart;

};

#endif
