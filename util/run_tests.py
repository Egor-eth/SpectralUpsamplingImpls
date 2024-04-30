#!/bin/python3

import numpy as np
import sys
import subprocess
import cv2
from pathlib import Path
from os.path import join 
from skimage import io
from skimage.metrics import structural_similarity as ssim
from skimage.metrics import peak_signal_noise_ratio as psnr



METHODS = ["glassner", "smits", "sigpoly"]
IMAGES = [join("input", "floppa.jpg"), join("input", "grid.png")]
TARGET_DIR = join("output", "tests")

def mse_deltaE(img1, img2):
    img1_lab = cv2.cvtColor(img1, cv2.COLOR_BGR2LAB)
    img2_lab = cv2.cvtColor(img2, cv2.COLOR_BGR2LAB)
    deltae = np.sqrt(np.sum((img1_lab - img2_lab) ** 2, axis=-1))
    return (np.mean(deltae), np.median(deltae), np.min(deltae), np.max(deltae))

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
        print(f"MSE-deltaE: {mse_deltaE(image_gt, image)}", file=f)
        

def main():
    if len(sys.argv) == 1:
        for img in IMAGES:
            run_test(img)
    else:
        run_test(sys.argv[1])


if __name__ == "__main__":
    main()
