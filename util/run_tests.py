#!/bin/python3

import matplotlib
import matplotlib.pyplot as plt
import numpy as np
import sys
import subprocess
import cv2
from pathlib import Path
from os.path import join 
from skimage import io
from skimage.metrics import structural_similarity as ssim
from skimage.metrics import peak_signal_noise_ratio as psnr
from plot import diff_image, mean_deltaE



METHODS = ["glassner", "smits", "sigpoly"]
IMAGES = [join("input", "lenna.png"), join("input", "grid.png")]
TARGET_DIR = join("output", "tests")


def run_test(path: str):
    image_gt = cv2.imread(path)
    image_gt_gray = cv2.cvtColor(image_gt, cv2.COLOR_BGR2GRAY)
    for method in METHODS:
        print(path + " : " + method)
        f = open(join(TARGET_DIR, Path(path).stem + "_" + method + ".txt"), "w+")
        subprocess.run(["./build/comparsion", method, "im", path], stdout=f)

        image = cv2.imread(join(TARGET_DIR, f"{Path(path).stem}_{method}.png"))

        image_gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)

        print(f"SSIM: {ssim(image_gt_gray, image_gray)}", file=f)
        print(f"PSNR: {psnr(image_gt, image)}", file=f)
        deltae = mean_deltaE(image_gt, image)
        print(f"MSE-deltaE: {deltae[:4]}", file=f)

        diff_path = join(TARGET_DIR, f"{Path(path).stem}_{method}_diff.png")
        #cv2.imwrite(diff_path, diff_image(deltae[4]))
        diff_image(deltae[4], diff_path)
        #plt.imsave(diff_path, deltae[4], cmap='inferno')
        

def main():
    if len(sys.argv) == 1:
        for img in IMAGES:
            run_test(img)
    else:
        run_test(sys.argv[1])


if __name__ == "__main__":
    main()
