import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.ticker import ScalarFormatter
import numpy as np
import matplotlib.pyplot as plt
from scipy import signal

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
filters_values = []

worN = int(fs*integration_time/2)

print(f'cantidad de muestras del audio: {worN*2}')
print(f'cantidad de muestras del espectro: {worN}')

#octavas
center_freq = [25, 31.5, 40, 50, 63, 80, 100, 125, 160, 200, 250, 315, 400, 500, 630, 800, 
               1000, 1250, 1600, 2000, 2500, 3150, 4000, 5000, 6300, 8000, 10000, 12500, 16000]
ticks_labels=['25' ,'31.5', '40', '50', '63', '80', '100', '125', '160', '200', '250', '315', '400',
              '500', '630', '800', '1000', '1250', '1600', '2000', '2500', '3150', '4000', '5000', '6300', '8000',
              '10000', '12500', '16000']

n = 3 #octava
for f in center_freq:
  sos = sbutter_bandpass(f*np.power(2,(-1)/(2*n)), f*np.power(2,1/(2*n)), fs, 2)
  w, h = signal.sosfreqz(sos, worN=worN)
  frecs = np.round_((w*fs)/(np.pi*2), 6)
  filters_values.extend(list(10*np.log10(abs(h)+eps)))
  plt.semilogx(frecs, 10*np.log10(abs(h)+eps))
plt.ylim(-40,5)
plt.xlim(10,20000)

plt.xlabel('Frecuencia [Hz]')
plt.ylabel('Ganancia [dB]')

plt.xticks(center_freq, ticks_labels)

plt.rcParams["figure.figsize"] = (11,7)

plt.tight_layout()

plt.show()


with open('filters_values.txt', 'w') as f:
    for i, value in enumerate(filters_values):
        if i == len(filters_values)-1:
            f.write(f"{value}")
        else:
            f.write(f"{value}\n")