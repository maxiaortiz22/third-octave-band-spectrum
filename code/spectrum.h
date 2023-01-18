#ifndef __uSound__spectrum__
#define __uSound__spectrum__

#include "engine_params.h"

#ifdef __cplusplus
extern "C" {
#endif
    
    void spectrum_alloc(float samplerate, float calibrationConstant, float * FFTBuffer, int FFTBuffer_size);
    void spectrum_free();
    int spectrum_process(float *audio, int audio_size);
    void spectrum_getSpectrum(float *spectrum, int spectrum_size);
    void spectrum_setValue(int param, float value);
    float spectrum_getValue(int param);
    void spectrum_getFilters(float *filterValues, int filters_size);
    void spectrum_getThirdOctaveBandSpectrum(float *thirdOctaveBandSpectrum, int thirdOctaveBandSpectrum_size);
    
#ifdef __cplusplus
};
#endif

#endif /* defined(__uSound__spectrum__) */
