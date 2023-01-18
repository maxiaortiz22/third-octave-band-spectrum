import spectrum
import numpy as np
import matplotlib.pyplot as plt
from scipy.fft import rfft, rfftfreq

def pure_tone(f, sr, duration):
    return (np.sin(2 * np.pi * np.arange(sr * duration) * f / sr)).astype(np.float32)

DATA_SIZE = 6000
SPECTRUM_SIZE = 3000
SAMPLE_RATE = 48000

CALIBRATION_CONSTANT = 0.00023456789

duration = DATA_SIZE / SAMPLE_RATE  # in seconds, may be float
f1 = 1000 # sine frequency, Hz, may be float
f2 = 2000
f3 = 4000

#Buffer para guardar los datos de la fft
fft_buffer = np.zeros(SPECTRUM_SIZE, dtype=np.float32)

#Genero un tono puro:
audio = pure_tone(f1, SAMPLE_RATE, duration) + pure_tone(f2, SAMPLE_RATE, duration) + pure_tone(f3, SAMPLE_RATE, duration) 

spectrum.spectrum_alloc(SAMPLE_RATE, CALIBRATION_CONSTANT,fft_buffer) #https://stackoverflow.com/questions/52199057/c-array-to-numpy-with-swig-problem-with-typeerror

spectrum.spectrum_process(audio)

result = spectrum.spectrum_getSpectrum(int(len(fft_buffer)))

print(len(result))

#Get spectrum values:
#print(spectrum.spectrum_getValue(spectrum.SP_BYPASS)) #Bypass
#print(spectrum.spectrum_getValue(spectrum.SP_TYPE_CHANGE)) #TYPE_CHANGE
#print(spectrum.spectrum_getValue(spectrum.SP_WINDOW)) #WINDOW
#print(spectrum.spectrum_getValue(spectrum.SP_SPECTRUM_MAX_FREQ)) #SpectrumMaxFreq
#print(spectrum.spectrum_getValue(spectrum.SP_FREQ_STEP_SIZE)) #fFreqStepSize

#print(spectrum.SP_BYPASS)
#print(spectrum.SP_TYPE_CHANGE)
#print(spectrum.SP_WINDOW)
#print(spectrum.SP_SPECTRUM_MAX_FREQ)
#print(spectrum.SP_FREQ_STEP_SIZE)


#Plot spectrum:
freq = np.linspace(0, SAMPLE_RATE/2, num=SPECTRUM_SIZE)
print(freq[1])

#Calibro el audio en python:
audio = audio/CALIBRATION_CONSTANT


#Scipy:
N = int(SAMPLE_RATE * duration)
yf = rfft(audio)
xf = rfftfreq(N, 1 / SAMPLE_RATE)

#plt.plot(freq, result, label='c')
#plt.show()

plt.plot(xf, 20*np.log10(np.abs(yf)), label='python')

plt.plot(freq, result, label='c')

plt.legend()
plt.show()

#print(len((20*np.log10(np.abs(yf)))[0:-1]), len(result))
#print(min((20*np.log10(np.abs(yf)))[0:-1] - result), max((20*np.log10(np.abs(yf)))[0:-1] - result))