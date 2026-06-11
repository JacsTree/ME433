import serial
import matplotlib.pyplot as plt
import numpy as np

PORT = "/dev/ttyACM0"
BAUD = 115200

ser = serial.Serial(PORT, BAUD, timeout=1)

input("Press ENTER to start experiment")

ser.write(b'a')

idx = []
desired = []
actual = []

while True:
    line = ser.readline().decode().strip()

    if not line:
        continue

    if line == "DONE":
        break

    i, d, a = line.split(',')

    idx.append(int(i))
    desired.append(float(d))
    actual.append(float(a))

ser.close()

idx = np.array(idx)
desired = np.array(desired)
actual = np.array(actual)

plt.figure()

plt.plot(idx, desired, label='Desired Current')
plt.plot(idx, actual, label='Measured Current')

plt.xlabel('Sample')
plt.ylabel('Current (A)')
plt.title('Current Controller Response')
plt.grid(True)
plt.legend()

plt.savefig("plot.png")
#plt.show()