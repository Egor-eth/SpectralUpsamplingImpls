#!/bin/python3

import numpy as np
import sys
import subprocess
from pathlib import Path
from os.path import join 

from plot import plot_spd

LUTS = [1, 3, 6]

TARGETS = ["ff00ff", "ff77ff", "ffff00", "ffff77", "00ffff", "77ffff",
           "ff0000", "00ff00", "0000ff", "ababab", "000000", "ffffff",
           "a05ff1", "151515"]

def main():
    for lut in LUTS:
        lut_path = join("resources", f"f_emission_lut_{lut}.eflf")
        print(lut_path)
        for target in TARGETS:
            subprocess.run(["./build/exporter", target, lut_path])
            plot_spd(join("output", "spd", target + ".spd"), False, "{}_" + str(lut) + ".png")


if __name__ == "__main__":
    main()
