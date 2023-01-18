import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.ticker import ScalarFormatter
import numpy as np
import matplotlib.pyplot as plt
from scipy import signal
import spectrum
from scipy.fft import rfft, rfftfreq

def pure_tone(f, sr, duration):
    return (np.sin(2 * np.pi * np.arange(sr * duration) * f / sr)).astype(np.float32)

DATA_SIZE = 6000
SPECTRUM_SIZE = 3000
SAMPLE_RATE = 48000

CALIBRATION_CONSTANT = 0.23456789

duration = DATA_SIZE / SAMPLE_RATE  # in seconds, may be float
f1 = 1000 # sine frequency, Hz, may be float
f2 = 2000
f3 = 4000

#Buffer para guardar los datos de la fft
fft_buffer = np.zeros(SPECTRUM_SIZE, dtype=np.float32)

#Genero un tono puro:
audio = pure_tone(f1, SAMPLE_RATE, duration) + pure_tone(f2, SAMPLE_RATE, duration) + pure_tone(f3, SAMPLE_RATE, duration)

spectrum.spectrum_alloc(SAMPLE_RATE, CALIBRATION_CONSTANT, fft_buffer) #https://stackoverflow.com/questions/52199057/c-array-to-numpy-with-swig-problem-with-typeerror

spectrum.spectrum_process(audio)

spectrumValuesC = spectrum.spectrum_getSpectrum(int(len(fft_buffer)))

#Calibro el audio en python:
audio = audio/CALIBRATION_CONSTANT

#octavas
center_freq = [25, 31.5, 40, 50, 63, 80, 100, 125, 160, 200, 250, 315, 400, 500, 630, 800, 
               1000, 1250, 1600, 2000, 2500, 3150, 4000, 5000, 6300, 8000, 10000, 12500, 16000]
ticks_labels=['25' ,'31.5', '40', '50', '63', '80', '100', '125', '160', '200', '250', '315', '400',
              '500', '630', '800', '1000', '1250', '1600', '2000', '2500', '3150', '4000', '5000', '6300', '8000',
              '10000', '12500', '16000']

#Filtros de C:

#Buffer para guardar los datos de la fft
filter_buffer = np.zeros(int(SPECTRUM_SIZE*len(center_freq)), dtype=np.float32)

filters_c = spectrum.spectrum_getFilters(int(len(filter_buffer)))

print(len(filters_c))

#Filtros en python:
#Manganeta para no hacer log(0):
eps = np.finfo(float).eps

def sbutter_bandpass(lowcut, highcut, fs, order):
    """ Diseña un filtro pasabanda a partir de especificar las frecuencias 
    de corte superior e inferior, la frecuencia de sampleo y el orden del filtro """
    nyq = 0.5 * fs
    low = lowcut / nyq
    high = highcut / nyq
    sos = signal.butter(order, [low, high], btype='band', output='sos')
    return sos

def sbutter_bandpass_filter(data, lowcut, highcut, fs, order):
    """ filtra una señal (data) a partir de diseñar un filtro pasabanda
    tipo butter a partir de las frecuencias de corte superior e inferior
    la frecuencia de sampleo y el orden del filtro. Devuelve la señal filtrada
    en tiempo """
    sos = sbutter_bandpass(lowcut, highcut, fs, order)
    y = signal.sosfilt(sos, data)
    return y

def Leq(y):
    'Calcula el nivel equivalente sin ponderacion de la señal y'
    N = len(y) #Numero de intervalos
    aux = np.sum(np.power(10,y/10))
    leq = 10*np.log10((1/N)*aux)
    return leq

def fft(signal, fs):
    fft_raw = np.fft.fft(signal)
    fft = fft_raw[:len(fft_raw)//2]
    fft_mag = abs(fft) / len(fft)
    freqs = np.linspace(0, fs/2, fft.size)
    return fft_mag, freqs

def plot_fft(mag, freqs, title, ax):
    mag_db = 20*np.log10(mag)
    ax.semilogx(freqs, mag_db)
    ax.set_xlim(20,freqs[-1])
    xfreqs = [62.5*(2**i) for i in range(8)]
    ax.set_xticks(xfreqs)
    ax.set_xticklabels([f'{f:.0f}' for f in xfreqs])
    ax.set_title(title)
    ax.set_xlabel('Frequency [Hz]')
    ax.set_ylabel('Amplitude [dB]')
    ax.set_ylim(-80,2)

fs = 48000
integration_time = 0.125 #125 ms

worN = int((fs*integration_time)/2)

################## OBTENGO EL BANCO DE FILTROS ########################

print(f'cantidad de muestras del espectro: {worN}')

n = 3 #octava
for f in center_freq:
    sos = sbutter_bandpass(f*np.power(2,(-1)/(2*n)), f*np.power(2,1/(2*n)), fs, 2)
    w, h = signal.sosfreqz(sos, worN=worN)
    frecs = np.round_((w*fs)/(np.pi*2), 6)
    plt.semilogx(frecs, 10*np.log10(abs(h)+eps))
plt.ylim(-40,5)
plt.xlim(10,20000)

plt.xlabel('Frecuencia [Hz]')
plt.ylabel('Ganancia [dB]')

plt.xticks(center_freq, ticks_labels)

plt.rcParams["figure.figsize"] = (11,7)

plt.tight_layout()

plt.show()


i = 0 
for f in center_freq:
  
    h = filters_c[i:(i+SPECTRUM_SIZE)]
    plt.semilogx(frecs, h)

    i+=SPECTRUM_SIZE

print(min(filters_c))
print(max(filters_c))

plt.ylim(-40,5)
plt.xlim(10,20000)

plt.xlabel('Frecuencia [Hz]')
plt.ylabel('Ganancia [dB]')

plt.xticks(center_freq, ticks_labels)

plt.rcParams["figure.figsize"] = (11,7)

plt.tight_layout()

plt.show()

################## SEÑAL FILTRADA POR AMBOS ALGORITMOS ########################

thirdOctaveBandC = spectrum.spectrum_getThirdOctaveBandSpectrum(int(len(center_freq)))

#Scipy:
N = int(SAMPLE_RATE * duration)
yf = rfft(audio)
xf = rfftfreq(N, 1 / SAMPLE_RATE)

spectrumValuesPython = 20*np.log10(np.abs(yf))[0:-1]

thirdOctaveBandPython = []
thirdOctaveBandC2 = []

n = 3 #tercio de octava
idx = 0
for f in center_freq:
    sos = sbutter_bandpass(f*np.power(2,(-1)/(2*n)), f*np.power(2,1/(2*n)), fs, 2)
    w, h = signal.sosfreqz(sos, worN=worN)

    frecs = np.round_((w*fs)/(np.pi*2), 6)

    assert len(spectrumValuesPython) == len(h)

    thirdOctaveBandPython.append(Leq(spectrumValuesPython + 10*np.log10(abs(h)+eps)))
    thirdOctaveBandC2.append(Leq(np.array(spectrumValuesC) + np.array(filters_c[idx:idx+SPECTRUM_SIZE])))
    idx += SPECTRUM_SIZE

plt.bar(np.arange(len(center_freq)), thirdOctaveBandC, alpha = 1, label='C')
#plt.bar(np.arange(len(center_freq)), thirdOctaveBandC2, alpha = 0.7, label='C2')
plt.bar(np.arange(len(center_freq)), thirdOctaveBandPython, alpha = 0.4, label='Python')

plt.xticks(np.arange(len(center_freq)), ticks_labels, fontsize=15, rotation=45)
plt.yticks(fontsize=15)

plt.ylabel('Nivel [dBSPL]', fontsize=15)
plt.xlabel('Frecuencia [Hz]', fontsize=15)

plt.legend()

plt.rcParams["figure.figsize"] = (16,7)


plt.tight_layout()

plt.show()

print(f'Máxima diferencia entre las bandas {max(thirdOctaveBandPython-thirdOctaveBandC)}')
print(f'Mínima diferencia entre las bandas {min(thirdOctaveBandPython-thirdOctaveBandC)}')