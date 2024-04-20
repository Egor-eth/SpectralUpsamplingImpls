#!/bin/python3
import matplotlib.pyplot as plt
import numpy as np
import sys
from pathlib import Path
from os.path import join


lst = []
with open(sys.argv[1], "r") as f:
    for s in f:
        spl = s.split(' ')
        lst.append([float(spl[0]), float(spl[1])])

spd = np.array(lst)

fig, ax = plt.subplots()
ax.plot(spd[:, 0], spd[:, 1])

ax.set(xlabel='wavelenght', ylabel='rel. power',
       title='Spectral Power Distribution')
ax.grid()

fig.savefig(join("output", "plots", Path(sys.argv[1]).stem + ".png"))
plt.show()
