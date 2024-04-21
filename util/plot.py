#!/bin/python3
import matplotlib.pyplot as plt
import numpy as np
import sys
from pathlib import Path
from os.path import join


def plot_spd(path: str, show: bool, fmt: str = "{}.png"):
    lst = []
    with open(path, "r") as f:
        for s in f:
            spl = s.split(' ')
            lst.append([float(spl[0]), float(spl[1])])

    spd = np.array(lst)

    name = Path(path).stem

    fig, ax = plt.subplots()
    ax.plot(spd[:, 0], spd[:, 1])

    ax.set(xlabel='wavelenght', ylabel='rel. power',
           title=f'Spectral Power Distribution ({name})')
    ax.grid()

    fig.savefig(join("output", "plots", fmt.format(name)))
    if show: 
        plt.show()
    plt.close()


if __name__ == "__main__":
    plot_spd(sys.argv[1], True)


