# DSP in Python
---
1. Getting data out of csv:
![alt text](images/image.png)
2. Plotting data in matplotlib:
![alt text](images/image1.png)
3. Finding sample rate:
```python
print("Sample rate: ",t.size/t[-1])
```
```terminal
Sample rate:  100.20040080160321
```
4. FFT data plots:
sigA:
![alt text](images/A.png)
sigB:
![alt text](images/B.png)
sigC:
![alt text](images/C.png)
sigD:
![alt text](images/D.png)
5.
Low pass filtering:

sigA (filter noise):
![alt text](images/A_LP1.png)
sigA (filter higher frequency signal) (very large lag):
![alt text](images/A_LP2.png)
sigB (filter noise): 
![alt text](images/B_LP1.png)
sigB (filter higher frequency signal):
![alt text](images/B_LP2.png)
sigC (no noise):
![alt text](images/C_LP1.png)
sigD:
![alt text](images/D_LP1.png)
