/* 
 * File:   paudio.h
 * Author: Jiri Zahradnik
 *
 * Created on March 9, 2017, 1:17 PM
 */

#include "paudio.h"

static int paudio::paCallback(
        const void* input, 
        void* output, 
        unsigned long frameCount, 
        const PaStreamCallbackTimeInfo* timeInfo, 
        PaStreamCallbackFlags statusFlags, 
        void* userData) 
{
    paTestData *data = (paTestData *)userData;
    float *out = (float *)output;
    (void) input;
    
    for(unsigned int i = 0; i < frameCount; i++){
        *out++ = data->left_phase;
        *out++ = data->right_phase;
        data->left_phase += 0.01f;
        if(data->left_phase >= 1.0f){
            data->left_phase -= 2.0f;
        }
        
        data->right_phase += 0.01f;
        if(data->right_phase >= 1.0f){
            data->right_phase -= 2.0f;
        }
    }
    
    return 0;
}

int paudio::initSound(){
    PaStreamParameters outputParameters;
    
    paError err = Pa_Initialize();
    if(err != paNoError){
        Pa_Terminate();
        fprintf( stderr, "An error occured while using the portaudio stream\n" );
        fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
        return err;
    }
    
    
    err = Pa_Terminate();
    return err;
}