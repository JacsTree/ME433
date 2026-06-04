import struct
import sys
import time

import board
import busio

i2c = busio.I2C(board.GP15, board.GP14)

while not i2c.try_lock():
    pass

ADDR = 0x68

# Wake MPU6050
i2c.writeto(ADDR, bytes([0x6B, 0x00]))
i2c.unlock()

DEADZONE = 3000  # deadzome to ignore noise
last_dir = ""  # no need to send new dir if it hasnt changed

while True:
    while not i2c.try_lock():
        pass

    buf = bytearray(6)
    i2c.writeto_then_readfrom(ADDR, bytes([0x3B]), buf)
    i2c.unlock()

    ax = struct.unpack(">h", buf[0:2])[0]
    ay = struct.unpack(">h", buf[2:4])[0]
    az = struct.unpack(">h", buf[4:6])[0]

    direction = ""

    if abs(ax) > abs(ay):
        if ax > DEADZONE:
            direction = "R"
        elif ax < -DEADZONE:
            direction = "L"
    else:
        if ay > DEADZONE:
            direction = "D"
        elif ay < -DEADZONE:
            direction = "U"

    # Send only on change
    if direction and direction != last_dir:
        print(direction)  # sends directions as "R\n" or similar
        last_dir = direction

    time.sleep(0.05)
