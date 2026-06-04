import matplotlib.pyplot as plt
import numpy as np
import csv

t = [] # column 0
data1 = [] # column 1

x = 10 #number of points to average

data2 = [] # avg data (assumes 0 at start)

with open('sigA.csv') as f:
    # open the csv file
    reader = csv.reader(f)
    i = 0
    for row in reader:
        # read the rows 1 one by one
        t.append(float(row[0])) # leftmost column
        data1.append(float(row[1])) # second column

        average=0
        for j in range(x):
            if(i-x<0):
                average+=0
            else:
                average+=data1[i-x];
        average/=x
        data2.append(average)
        i+=1

plt.plot(t,data1,'k')
plt.plot(t,data2,'r')
plt.savefig("plot.png")

plt.cla()

Fs = len(t)/t[-1] # sample rate
y = data1 # the data to make the fft from
n = len(y) # length of the signal
k = np.arange(n)
T = n/Fs
frq = k/T # two sides frequency range
frq = frq[range(int(n/2))] # one side frequency range
Y = np.fft.fft(y)/n # fft computing and normalization
Y = Y[range(int(n/2))]

plt.xlabel('Freq (Hz)')
plt.ylabel('|Y(freq)|')

plt.loglog(frq,abs(Y),'k') # plotting the fft

Fs = len(t)/t[-1] # sample rate
y = data2 # the data to make the fft from
n = len(y) # length of the signal
k = np.arange(n)
T = n/Fs
frq = k/T # two sides frequency range
frq = frq[range(int(n/2))] # one side frequency range
Y = np.fft.fft(y)/n # fft computing and normalization
Y = Y[range(int(n/2))]

plt.loglog(frq,abs(Y),'r')

#plt.show() #plot
plt.savefig("fft.png") #save plot as picture .pdf for pdf