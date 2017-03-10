/* 
 * File:   paudio.h
 * Author: Jiri Zahradnik
 *
 * Created on March 9, 2017, 1:17 PM
 */

#include "paudio.h"

static paudio::paData data;
static PaStream *stream;

int paudio::initSound(){
    PaStreamParameters outputParam;

    int err = Pa_Initialize();
    if(err != paNoError){
        Pa_Terminate();
        fprintf( stderr, "An error occured while using the portaudio stream\n" );
        fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
        return err;
    }

    outputParam.device = Pa_GetDefaultOutputDevice(); /* default output device */
    outputParam.channelCount = 1;       /* stereo output */
    outputParam.sampleFormat = paUInt8; /* 32 bit floating point output */
    outputParam.suggestedLatency = Pa_GetDeviceInfo( outputParam.device )->defaultLowOutputLatency;
    outputParam.hostApiSpecificStreamInfo = NULL;

    err = Pa_OpenStream(&stream, NULL, &outputParam, SAMPLE_RATE, FRAMES_PER_BUFFER, paClipOff, paCallback, &data);

    if(err != paNoError){
        Pa_Terminate();
        fprintf( stderr, "An error occured while using the portaudio stream\n" );
        fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
        return err;
    }

    err = Pa_StartStream(stream);

    if(err != paNoError){
        Pa_Terminate();
        fprintf( stderr, "An error occured while using the portaudio stream\n" );
        fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
        return err;
    }

    return err;
}

int paudio::stopSound(){
    int err = 0;
    err = Pa_StopStream(stream);
    if(err != paNoError){
        Pa_Terminate();
        fprintf( stderr, "An error occured while using the portaudio stream\n" );
        fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
        return err;
    }

    err = Pa_CloseStream(stream);

    if(err != paNoError){
        Pa_Terminate();
        fprintf( stderr, "An error occured while using the portaudio stream\n" );
        fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
        return err;
    }

    Pa_Terminate();
    return err;
}

void paudio::setFreq(uint32_t f){
    data.up_count = 0;
    data.freq = f;
}

void paudio::beep(uint32_t f, int t){
    data.total_count = 0;
    data.up_count = (uint32_t)(SAMPLE_RATE * t / 1000);
    data.freq = f;
}