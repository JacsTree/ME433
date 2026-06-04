import pgzero.game

pgzero.game.PGZeroGame.show_default_icon = classmethod(lambda cls: None)

import random

import pgzrun
import serial
from pgzero.builtins import *

ser = serial.Serial("/dev/ttyACM0", 9600, timeout=0)

# Window settings
WIDTH = 600
HEIGHT = 600
GRID_SIZE = 20
SPEED = 0.4

# Snake setup
snake = [(10, 10), (9, 10), (8, 10)]
direction = (1, 0)
game_over = False

# Food setup
food = (
    random.randint(0, WIDTH // GRID_SIZE - 1),
    random.randint(0, HEIGHT // GRID_SIZE - 1),
)


def draw():
    screen.clear()

    if game_over:
        screen.draw.text(
            "GAME OVER", center=(WIDTH // 2, HEIGHT // 2), fontsize=60, color="red"
        )
        return

    # Draw snake
    for segment in snake:
        x = segment[0] * GRID_SIZE
        y = segment[1] * GRID_SIZE
        screen.draw.filled_rect(Rect((x, y), (GRID_SIZE, GRID_SIZE)), "green")

    # Draw food
    fx = food[0] * GRID_SIZE
    fy = food[1] * GRID_SIZE
    screen.draw.filled_rect(Rect((fx, fy), (GRID_SIZE, GRID_SIZE)), "red")


def move_snake():
    n_bytes = ser.readline()  # read a line from the serial port, as bytes
    s = str(n_bytes)  # turn the bytes into a string
    # print(s)

    global food, game_over

    if game_over:
        return

    head_x, head_y = snake[0]
    dx, dy = direction
    new_head = (head_x + dx, head_y + dy)

    # Wall collision
    if (
        new_head[0] < 0
        or new_head[0] >= WIDTH // GRID_SIZE
        or new_head[1] < 0
        or new_head[1] >= HEIGHT // GRID_SIZE
    ):
        game_over = True
        return

    # Self collision
    if new_head in snake:
        game_over = True
        return

    snake.insert(0, new_head)

    # Eat food
    if new_head == food:
        food = (
            random.randint(0, WIDTH // GRID_SIZE - 1),
            random.randint(0, HEIGHT // GRID_SIZE - 1),
        )
    else:
        snake.pop()


# Empty update so PGZero is happy
def update():
    global direction

    if ser.in_waiting:
        msg = ser.readline().decode().strip()  # can lag if pico sends data too fast

        # print(msg)  # debug

        if msg == "U" and direction != (0, 1):
            direction = (0, -1)
        elif msg == "D" and direction != (0, -1):
            direction = (0, 1)
        elif msg == "R" and direction != (1, 0):
            direction = (-1, 0)
        elif msg == "L" and direction != (-1, 0):
            direction = (1, 0)


# Snake speed
clock.schedule_interval(move_snake, SPEED)


# Input function
def on_key_down(key):
    global direction

    if key == keys.UP and direction != (0, 1):
        direction = (0, -1)
    elif key == keys.DOWN and direction != (0, -1):
        direction = (0, 1)
    elif key == keys.LEFT and direction != (1, 0):
        direction = (-1, 0)
    elif key == keys.RIGHT and direction != (-1, 0):
        direction = (1, 0)


pgzrun.go()
