#include "spectrum.h"
#include <stdlib.h>

#include <math.h>
#include <string.h>
#include "kiss_fft.h"
#include "kiss_fftr.h"

 #include <stdint.h>

#ifdef __ANDROID__

#include <android/log.h>
#define LOG_TAG "spectrum"
#define printf(fmt,args...)  __android_log_print(ANDROID_LOG_VERBOSE,LOG_TAG, fmt, ##args)

#endif

#define DATA_SIZE 6000
#define SPECTRUM_SIZE 3000 //Contemplando sample_rate = 48k e integration_time = 0.125 (125 ms - Fast)
#define AVERAGES_SIZE 7
#define ISO_SIZE 31
#define TOT_BANDS 29 //Cantidad de centros de bandas de tercios a analizar (de 25 a 16k Hz)

// The sizes for each "memory aallocation increase" of both the arrays of
// chars storing each line (MORE_CHARS), and the array of pointers to chars 
// pointing to the strings/lines (MORE_LINES).
#define MORE_LINES 1024
#define MORE_CHARS 1024
#define true 1

void getSpectrum();
float Leq(float signal[SPECTRUM_SIZE]);

struct spectrum {
    int iBypass;
    uint8_t iSpectrumChange;
    float iSampleRate;
    float fFreqStepSize;
    uint8_t iWindow;
    uint8_t iRMS_peak;
    float calibrationConstant;
    
    uint32_t iDataIndex;
    float fDataBuffer[DATA_SIZE];

    float * fSpectrumBuffer;
    float fAveragesBuffer[AVERAGES_SIZE];
    float fISOBuffer[ISO_SIZE];
    
    int SpectrumMaxIndex;
    float SpectrumMaxFreq;
    uint32_t AveragesMaxIndex;
    float SpectrumMax;

    float AveragesMax;
    float AveragesMaxFreq;

    kiss_fft_cpx out_cpx[DATA_SIZE];

    float filterValues[SPECTRUM_SIZE*TOT_BANDS];
    float thirdOctaveBandSpectrum[TOT_BANDS];

};

static struct spectrum *pSpectrum;

void spectrum_alloc(float samplerate, float calibrationConstant, float * FFTBuffer, int FFTBuffer_size) {
    if (pSpectrum == NULL) {
        pSpectrum = (struct spectrum*)malloc(sizeof(struct spectrum));
    }
    pSpectrum->iBypass = 0;
    pSpectrum->iSpectrumChange = 0;
    pSpectrum->iSampleRate = samplerate;
    pSpectrum->fFreqStepSize = (((pSpectrum->iSampleRate) / 2.0) / (float) SPECTRUM_SIZE);
    pSpectrum->iDataIndex = 0;
    pSpectrum->iWindow = 0;
    pSpectrum->iRMS_peak = 0;
    pSpectrum->calibrationConstant = calibrationConstant;
    
    pSpectrum->SpectrumMaxIndex = 0;
    pSpectrum->SpectrumMaxFreq = pSpectrum->iSampleRate / 2.0;
    pSpectrum->AveragesMaxIndex = 0;

    pSpectrum->SpectrumMax = 0;
    pSpectrum->AveragesMax = 0;
    pSpectrum->AveragesMaxFreq = 0;
    pSpectrum->fSpectrumBuffer = FFTBuffer;


    // open filter_values.txt for reading
    FILE *file = fopen("filters_values.txt", "r");
    
    // if the file failed to open, exit with an error message and status
    if (file == NULL)
    {
        printf("Error opening file.\n");
        return 1;
    }
    
    // lines will point to the array of pointers to chars
    char **lines;
    
    // allocate space for MORE_LINES amount of pointers to char elements initially
    lines = malloc(sizeof(char *) * MORE_LINES);

    // used to keep track of the current number of lines read
    size_t total_lines = 0;
    
    // used to keep track of the current number of chars read on the current line
    size_t total_chars = 0;

    // used to store each char read from the file
    char c;
    
    // loop will read each char from the file until the end of the file is 
    // reached or an error occurs reading from the file
    do 
    {
        // read the next char from the file, store it into c
        c = fgetc(file);
        
        // if error occurs reading from file, exit with error message and status
        if (ferror(file))
        {
        printf("Error reading from file.\n");
        return 1;
        }
        
        // if we reach the end of the file, stop reading from the file
        if (feof(file))
        {
        // if a line is currently being read, total_chars will not equal 0, this 
        // will occur if the last line of the file doesn't end with a newline
        if (total_chars != 0)
        {        
            // In this case, we'll use realloc to re-allocate the block of memory 
            // for this string to be exaclty large enough to store it (total_chars 
            // plus one extra for the null terminator that ends a string.
            lines[total_lines] = realloc(lines[total_lines], total_chars + 1 );
            
            // end the string by placing null terminator at the end
            lines[total_lines][total_chars] = '\0';
            
            // increment total_lines to acknowledge we have read another line from
            // the file
            total_lines++;
        }
        
        // will stop the loop, and stop the reading of the file
        break; 
        }
        
    
        // If total_chars is set to 0 this is the first char we're reading on this
        // line, and we can allocate MORE_CHARS initial amount of space to store the 
        // string using malloc... i.e. a char array of size MORE_CHARS.
        if (total_chars == 0) lines[total_lines] = malloc(MORE_CHARS); 
        
        // store the char into "the current index of the current line" we've read in
        lines[total_lines][total_chars] = c; 
        
        // increment total_chars as we have just read in another char
        total_chars++;
        
        // if the char is a newline, we have reached the end of the current line
        if (c == '\n')
        {
        // We now know exactly how many chars are required to store the string for
        // this line, and we can reallocate the block of memory to be exacly large
        // enough... total_chars + 1 extra for the null terminator char that ends
        // a string.
        lines[total_lines] = realloc(lines[total_lines], total_chars + 1 );
        
        // end the string by placing the null terminator at the end
        lines[total_lines][total_chars] = '\0';
        
        // increment total_lines as we have completed reading in another line
        total_lines++;
        
        // set total_chars back to zero as we are about to start reading in 
        // another line (or at least, trying to)
        total_chars = 0;
        
        // It's possible that we have read enough lines that we have reached the 
        // maximum size of the array of pointers to chars.  We increase the size 
        // of the array by MORE_LINES amount each time, so if total_lines is a
        // multiple of MORE_LINES then we have reached the size of the current 
        // array of pointers.  
        if (total_lines % MORE_LINES == 0)
        {
            // In this case, we increase the size of the array by MORE_LINES and 
            // use realloc to reallocate space for this larger block of memory.  
            size_t new_size = total_lines + MORE_LINES;
            lines = realloc(lines, sizeof(char *) * new_size);
        }
        }
        // If the character is not a newline character, we haven't reached the end 
        // of the line, and we check if we have reached the size of the current 
        // array of chars.  The same as the above, because we increase the size of
        // the array of chars by MORE_CHARS each time, whenever total_chars is set
        // to a multiple of MORE_CHARS we know we have reached the size of the 
        // current char array.
        else if (total_chars % MORE_CHARS == 0)
        {
        // We increase the size of the array by MORE_CHARS and use realloc to
        // reallocate space for this larger block of memory.
        size_t new_size = total_chars + MORE_CHARS;
        lines[total_lines] = 
            realloc(lines[total_lines], new_size); 
        }

    } while (true);

    // At this point we know exactly how many lines we have read from the file and
    // can reallocate the block of memory for the array of pointers to chars to be
    // exactly the size required.
    lines = realloc(lines, sizeof(char *) * total_lines);

    /*
    // Output each string/line in the array of strings
    for (size_t i = 0; i < total_lines; i++)
        printf("%s", lines[i]);
    */
    
    size_t lengthFilters = sizeof(pSpectrum->filterValues) / sizeof(pSpectrum->filterValues[0]);
    // Guardo los valores del archivo leído en la lista de filterValues y los paso a float
    if ((int) total_lines != (int) lengthFilters)
    {
        printf("total_lines y filterValues tienen largos distintos.\n");
        printf("%d", (int) total_lines);
        printf("\n");
        printf("%d", (int) lengthFilters);
        return 1;
    }


    for (int i = 0; i < (int) total_lines; i++){

        //printf("%f", (float) atof(lines[i]));
        pSpectrum->filterValues[i] = atof(lines[i]);
        //printf("%f", (float) pSpectrum->filterValues[i]); 
    }
    
    
    // Free the block of memory allocated for each string
    for (size_t i = 0; i < total_lines; i++)
        free(lines[i]);
    
    // Free the block of memory allocated for the array of pointers to chars
    free(lines);
    
    // close the file as we are done working with it
    fclose(file);


}

void spectrum_free() {
    kiss_fft_cleanup();
    free(pSpectrum);
    pSpectrum = NULL;
}

int spectrum_process(float *audio, int audio_size) {
    int result = 0;
    int sizeBuffer = DATA_SIZE;
    int sizecpy = audio_size;
    if (pSpectrum->iBypass != 1) {

        if (sizecpy > (sizeBuffer - pSpectrum->iDataIndex)){ 
            sizecpy = sizeBuffer - pSpectrum->iDataIndex;
        }
        memcpy(pSpectrum->fDataBuffer + pSpectrum->iDataIndex, audio, sizecpy*sizeof(float));
        //	printf("sizeBuffer %d, sizeCpy %d, dataIndex %d", sizeBuffer, sizecpy, pSpectrum->iDataIndex);
        pSpectrum->iDataIndex += sizecpy;
        if (pSpectrum->iDataIndex == sizeBuffer) {
            getSpectrum();
            
            //Agrego un for para guardar todo en data!!!

            result = 1;
            pSpectrum->iDataIndex = 0;
        }
    }
    return result;
}

void getSpectrum() {
    int i;
    int idx = 0;
    float t_SpectrumMax = 0;
    int t_SpectrumMaxIndex = 0;
    float filtered_signal[SPECTRUM_SIZE];

    // Divido al audio por la constante de calibración para tener los 94 dB de referencia.
    size_t lengthFDataBuffer = sizeof(pSpectrum->fDataBuffer) / sizeof(pSpectrum->fDataBuffer[0]);
    for (int i = 0; i < (int) lengthFDataBuffer; i++){
        pSpectrum->fDataBuffer[i] = pSpectrum->fDataBuffer[i]/pSpectrum->calibrationConstant;
    }

    kiss_fft_scalar * Data = pSpectrum->fDataBuffer;

    kiss_fft_cpx * out_cpx = pSpectrum->out_cpx;

    
    kiss_fftr_cfg FFT = kiss_fftr_alloc(DATA_SIZE,0,NULL,NULL);

    kiss_fftr(FFT, Data, out_cpx);
    free(FFT);


    // Calculate spectrum:
    for(i=0;i<SPECTRUM_SIZE;i++) {
        pSpectrum->fSpectrumBuffer[i] = 10.0f*log10f((out_cpx[i].r * out_cpx[i].r) + (out_cpx[i].i * out_cpx[i].i));
        if (pSpectrum->fSpectrumBuffer[i] > t_SpectrumMax) {
            t_SpectrumMax = pSpectrum->fSpectrumBuffer[i];
            t_SpectrumMaxIndex = i;
        }
        
        //printf("SPECTRUM LEFT CHANNEL %06.2f dB | Freq %08.2f Hz | Index %04d ", pSpectrum->fLeftSpectrumBuffer[i], (i * pSpectrum->fFreqStepSize), i);
        //printf("SPECTRUM RIGHT CHANNEL %06.2f dB | Freq %08.2f Hz | Index %04d", pSpectrum->fRightSpectrumBuffer[i], (i * pSpectrum->fFreqStepSize), i);
    }

    pSpectrum->SpectrumMax = t_SpectrumMax;
    pSpectrum->SpectrumMaxIndex = t_SpectrumMaxIndex;
    
    // Calculate Third octave band spectrum:

    for(i=0; i<TOT_BANDS; i++){

        //Obtengo los valores para la banda en cuestión:
        for(int t=0; t<SPECTRUM_SIZE; t++){
            filtered_signal[t] = pSpectrum->fSpectrumBuffer[t] + pSpectrum->filterValues[idx];
            idx++;
        }

        //Obtengo el valor de la banda:
        pSpectrum->thirdOctaveBandSpectrum[i] = Leq(filtered_signal);
    }

    
    //pSpectrum->SpectrumMaxFreq = pSpectrum->SpectrumMaxIndex * pSpectrum->fFreqStepSize;
    //printf("SPECTRUM LEFT CHANNEL %06.2f dB Index %04d Freq %08.2f Hz", pSpectrum->leftSpectrumMax,  pSpectrum->leftSpectrumMaxIndex, pSpectrum->leftSpectrumMaxFreq );
    //printf("SPECTRUM RIGHT CHANNEL %06.2f dB Index %04d Freq %08.2f Hz", pSpectrum->rightSpectrumMax,  pSpectrum->rightSpectrumMaxIndex, pSpectrum->rightSpectrumMaxFreq );
    kiss_fft_cleanup();
}

float Leq(float signal[SPECTRUM_SIZE]) {
    
    int N = SPECTRUM_SIZE; //Número de intervalos
    float aux = 0.0f;

    //aux = np.sum(np.power(10,y/10))

    for(int i=0; i < SPECTRUM_SIZE; i++){
        aux = aux + (float) pow(10.0, signal[i]/10.0);
    }

    float leq = 10.0f*log10f( aux / N);

    return leq;
}

void spectrum_getSpectrum(float *spectrum, int spectrum_size) { //Dejo solo el izquierdo por ahora!
    //memcpy(spectrum, pSpectrum->fSpectrumBuffer, sizeof(float) * spectrum_size);

    for(int i=0;i<SPECTRUM_SIZE;i++) {
        spectrum[i] = pSpectrum->fSpectrumBuffer[i];     
    }
}

void spectrum_getFilters(float *filterValues, int filters_size) {

    size_t lengthFilters = sizeof(pSpectrum->filterValues) / sizeof(pSpectrum->filterValues[0]);

    for(int i=0; i < (int) lengthFilters; i++) {
        filterValues[i] = pSpectrum->filterValues[i];     
    }
}

void spectrum_getThirdOctaveBandSpectrum(float *thirdOctaveBandSpectrum, int thirdOctaveBandSpectrum_size) {

    for(int i=0; i < TOT_BANDS; i++) {
        thirdOctaveBandSpectrum[i] = pSpectrum->thirdOctaveBandSpectrum[i];     
    }
}


void spectrum_setValue(int param, float val) {
    switch (param & MASK_PARAM) {
        case SP_BYPASS: printf("[SET]spectrum->bypass : %s\n" ,(val == 1) ? "true" : "false");
            pSpectrum->iBypass = (val == 1) ? 1 : 0;
            break;
        case SP_TYPE_CHANGE: printf("[SET]spectrum->type change : %d\n" ,(int) val);
            pSpectrum->iSpectrumChange = (uint8_t)val;
            break;
        case SP_WINDOW: printf("[SET]spectrum->window : %d\n" , (int)val);
            pSpectrum->iWindow = (uint8_t)val;
            break;
        case SP_RMS_PEAK: printf("[SET]spectrum->mode : %s\n" , (val == 1) ? "peak" : "RMS");
            pSpectrum->iRMS_peak = (uint8_t)val;
            break;
        default: printf("error!!\n");
            break;
    }
}

float spectrum_getValue(int param) {
    printf("[GET]spectrum : %d", param);
    switch (param & MASK_BAND) {
        case SP_BYPASS:
            printf("[GET]spectrum->iBypass : %s\n" ,(pSpectrum->iBypass == 1) ? "true" : "false");
            return pSpectrum->iBypass;
        case SP_TYPE_CHANGE:
            printf("[GET]spectrum->iSpectrumChange : %d\n", pSpectrum->iSpectrumChange);
            return pSpectrum->iSpectrumChange;
        case SP_WINDOW: printf("[GET]spectrum->iWindow : %d\n" ,pSpectrum->iWindow);
            return pSpectrum->iWindow;
        case SP_SPECTRUM_MAX_FREQ: printf("[GET]spectrum->SpectrumMaxFreq : %d\n" ,pSpectrum->SpectrumMaxFreq);
            return pSpectrum->SpectrumMaxFreq;
        case SP_FREQ_STEP_SIZE: printf("[GET]spectrum->fFreqStepSize : %d\n" ,pSpectrum->fFreqStepSize);
            return pSpectrum->fFreqStepSize;
        default:
            printf("error!!\n");
            break;
    }

    return 0;
}
