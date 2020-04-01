import math
import random

def newton_sqrt(x):
    next_x = lambda t: (t*t + x) / (2 * t)
    t0 = random.random()
    while True:
        t1 = next_x(t0)
        if abs(t1 - t0) < 1e-4:
            return t1
        t0 = t1
        print(t1)

print(newton_sqrt(1024))
