/* 
 * File: paudio.h
 * code taken from: http://stackoverflow.com/questions/7678470/generating-sound-of-a-particular-frequency-using-gcc-in-ubuntu
 * and modified by Jiri Zahradnik. 
 * Original code is under WTFPL licence: https://en.wikipedia.org/wiki/WTFPL *
 *
 * Created on March 9, 2017, 1:17 PM
 */

#ifndef __PAUDIO_H__
#define __PAUDIO_H__

#include <iostream>
#include <cmath>
#include <cstdint>
#include <unistd.h>
#include "portaudio.h"

#define SAMPLE_RATE 44100
#define FRAMES_PER_BUFFER 64

namespace paudio{

    typedef struct{
        uint32_t total_count;
        uint32_t up_count;
        uint32_t counter;
        uint32_t prev_freq;
        uint32_t freq;
    }paData;

    /* callback function */
    static inline int paCallback(
            const void *input,
            void *output,
            unsigned long frameCount,
            const PaStreamCallbackTimeInfo *timeInfo,
            PaStreamCallbackFlags statusFlags,
            void *userData
        )
    {
        paData *data = (paData*)userData;
        uint8_t *out = (uint8_t*)output;
        uint32_t freq = data->freq;

        (void) timeInfo; /* Prevent unused variable warnings. */
        (void) statusFlags;
        (void) input;

        for(unsigned int i = 0; i < frameCount; i++){
            if((0 < data->up_count) && (data->total_count == data->up_count)){
                *out++ = 0x00;
                continue;
            }
            data->total_count++;

            if(freq != data->prev_freq){
                data->counter = 0;
            }

            if(freq) {
                uint32_t overflow_max = SAMPLE_RATE / freq;
                uint32_t data_cnt = data->counter % overflow_max;
                if ((overflow_max / 2) < data_cnt){
                    *out++ = 0xff;
                }
                else {
                    *out++ = 0x00;
                }
                data->counter++;
            }
            else {
                data->counter = 0;
                *out++ = 0;
            }
            data->prev_freq = freq;
        }

        return paContinue;

    }

    int initSound();
    int stopSound();
    void setFreq(uint32_t f);
    void beep(uint32_t f, int t);

}

#endif /* PAUDIO_H */

