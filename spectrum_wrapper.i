%module spectrum

%{
#define SWIG_FILE_WITH_INIT
#include "spectrum.h"
#include "kiss_fft.h"
#include "kiss_fftr.h"
#include "usoundlib_params.h"
#include "engine_params.h"
%}

%include "numpy.i"

%init %{
    import_array();
%}

%apply (float* ARGOUT_ARRAY1, int DIM1) {(float *spectrum, int spectrum_size)};
%apply (float* ARGOUT_ARRAY1, int DIM1) {(float *filterValues, int filters_size)};
%apply (float* ARGOUT_ARRAY1, int DIM1) {(float *thirdOctaveBandSpectrum, int thirdOctaveBandSpectrum_size)};
%apply (float* IN_ARRAY1, int DIM1) { (float * FFTBuffer, int FFTBuffer_size)};
%apply (float* IN_ARRAY1, int DIM1) { (float *audio, int audio_size)};

%constant int SP_LEFT_CHANNEL       = 0x10;
%constant int SP_RIGHT_CHANNEL      = 0x20;
%constant int SP_BYPASS             = 0x30;
%constant int SP_TYPE_CHANGE        = 0x40;
%constant int SP_WINDOW             = 0x50;
%constant int SP_RMS_PEAK           = 0x90;
//%constant int SP_SPECTRUM_MAX       = 0x2;
//%constant int SP_AVERAGES_MAX       = 0x3;
//%constant int SP_SPECTRUM_MAX_INDEX = 0x4;
//%constant int SP_AVERAGES_MAX_INDEX = 0x5;
%constant int SP_SPECTRUM_MAX_FREQ  = 0x60;
%constant int SP_AVERAGES_MAX_FREQ  = 0x70;
%constant int SP_FREQ_STEP_SIZE     = 0x80;


//%apply float *OUTPUT { float *spectrum };

%include "code/spectrum.h"
%include "code/kiss_fft.h"
%include "code/kiss_fftr.h"