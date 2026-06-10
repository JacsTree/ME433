import serial
import os

import matplotlib.pyplot as plt
import numpy as np

PORT = "/dev/ttyACM0"  # Adjust if needed
BAUDRATE = 115200
NUM_SAMPLES = 100

times = []
data1 = []
data2 = []

a = 0.3
b = 0.7

with serial.Serial(PORT, BAUDRATE, timeout=3) as ser:
    ser.reset_input_buffer()

    ser.write(b"r\n")
    ser.flush()

    for i in range(NUM_SAMPLES):
        line = ser.readline().decode("utf-8").strip()

        if not line:
            raise RuntimeError(f"Timeout waiting for sample {i}")

        try:
            t, v1, v2 = map(int, line.split())

            times.append(t/1000)
            data1.append(v1)
            data2.append(v2)

        except ValueError:
            raise RuntimeError(f"Malformed line {i}: {line}")

print(f"Received {len(times)} samples")

fig, (ax1, ax2) = plt.subplots(2, 1)
fig.subplots_adjust(hspace=0.6);
ax1.set_xlabel("Time (s)")
ax1.set_ylabel("Amplitude")
ax2.set_xlabel("Freq (Hz)")
ax2.set_ylabel("|Y(freq)|")
ax1.plot(times, data1, "k", label = "raw data")
ax1.plot(times, data2, "r", label = "iir data")
fig.suptitle(f"IIR Filtered Data (A={a}, B={b})")

ax1.legend()

Fs = len(times) / times[-1]  # sample rate
y = data1  # the data to make the fft from
n = len(y)  # length of the signal
k = np.arange(n)
T = n / Fs
frq = k / T  # two sides frequency range
frq = frq[range(int(n / 2))]  # one side frequency range
Y = np.fft.fft(y) / n  # fft computing and normalization
Y = Y[range(int(n / 2))]
ax2.loglog(frq, abs(Y), "k")


Fs = len(times) / times[-1]  # sample rate
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
#plt.show()
