import csv
import os

import matplotlib.pyplot as plt
import numpy as np

t = []  # column 0
data1 = []  # column 1
data2 = []  # low pass averaged data

avg = 1000  # 800
a = 0.8
b = round(1 - a, 3)

with open("sigD.csv") as f:
    # open the csv file
    reader = csv.reader(f)
    for row in reader:
        # read the rows 1 one by one
        t.append(float(row[0]))  # leftmost column
        data1.append(float(row[1]))  # second column

b = [
    -0.011021740228542347,
    -0.007395873825178662,
    -0.002711724949312769,
    0.002937702028422173,
    0.009412930322954659,
    0.016532610342813527,
    0.024080214022055922,
    0.031812539162291147,
    0.039469644246706001,
    0.046785757634171470,
    0.053500650914860663,
    0.059370937512630631,
    0.064180756035643438,
    0.067751323668625713,
    0.069948896891495818,
    0.070690752440725285,
    0.069948896891495818,
    0.067751323668625713,
    0.064180756035643438,
    0.059370937512630631,
    0.053500650914860663,
    0.046785757634171470,
    0.039469644246706001,
    0.031812539162291147,
    0.024080214022055922,
    0.016532610342813527,
    0.009412930322954659,
    0.002937702028422173,
    -0.002711724949312769,
    -0.007395873825178662,
    -0.011021740228542347,
]

data2 = [0.0] * len(data1)

for n in range(len(data1)):
    sum = 0.0
    for k in range(len(b)):
        i = n - k
        if i < 0:
            break  # oob
        sum += b[k] * data1[i]
    data2[n] = sum

# dt = 1.0/10000.0 # 10kHz
# t = np.arange(0.0, 1.0, dt) # 10s
# # a constant plus 100Hz and 1000Hz
# s = 4.0 * np.sin(2 * np.pi * 100 * t) + 0.25 * np.sin(2 * np.pi * 1000 * t) + 25

fig, (ax1, ax2) = plt.subplots(2, 1)
ax1.set_xlabel("Time")
ax1.set_ylabel("Amplitude")
ax2.set_xlabel("Freq (Hz)")
ax2.set_ylabel("|Y(freq)|")
ax1.plot(t, data1, "k")
ax1.plot(t, data2, "r")
plt.suptitle(
    f"FIR filtered ({len(b)} taps) — windowed-sinc lowpass, fc=400 Hz, transition=300 Hz"
)


Fs = len(t) / t[-1]  # sample rate
y = data1  # the data to make the fft from
n = len(y)  # length of the signal
k = np.arange(n)
T = n / Fs
frq = k / T  # two sides frequency range
frq = frq[range(int(n / 2))]  # one side frequency range
Y = np.fft.fft(y) / n  # fft computing and normalization
Y = Y[range(int(n / 2))]
ax2.loglog(frq, abs(Y), "k")


Fs = len(t) / t[-1]  # sample rate
y = data2  # the data to make the fft from
n = len(y)  # length of the signal
k = np.arange(n)
T = n / Fs
frq = k / T  # two sides frequency range
frq = frq[range(int(n / 2))]  # one side frequency range
Y = np.fft.fft(y) / n  # fft computing and normalization
Y = Y[range(int(n / 2))]
ax2.loglog(frq, abs(Y), "r")


try:
    os.remove("fft.png")
except FileNotFoundError:
    pass
plt.savefig("fft.png")
plt.show()
