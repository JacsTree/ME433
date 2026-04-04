import time
import pwmio
import board

led = pwmio.PWMOut(board.GP15, frequency=50, duty_cycle=8000) #1600-8000


while True:
    for i in range(100):
        led.duty_cycle = int(i * 6500 / 100)+1600  # Fade up
        time.sleep(0.02)
    led.duty_cycle = 1600
    time.sleep(0.4)

