/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   paudio.h
 * Author: Jiri Zahradnik
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

namespace paudio{

    typedef struct{
        float left_phase;
        float right_phase;
    }paTestData;

    /* callback function */
    static int paCallback(
            const void *input,
            void *output,
            unsigned long frameCount,
            const PaStreamCallbackTimeInfo *timeInfo,
            PaStreamCallbackFlags statusFlags,
            void *userData
        );

    int initSound();

}

#endif /* PAUDIO_H */

