#!/bin/python3
import matplotlib.pyplot as plt
import numpy as np
from numpy.polynomial import Polynomial
import sys
from pathlib import Path
from os.path import join


def diff_image(deltae: np.ndarray, diff_path: str):
    fig, ax = plt.subplots()
    pos = ax.imshow(deltae, cmap='inferno')
    plt.colorbar(pos, ax=ax)
    ax.axis('off')
    plt.savefig(diff_path, bbox_inches='tight')

def mean_deltaE(img1, img2):
    import cv2
    img1_lab = cv2.cvtColor(img1, cv2.COLOR_BGR2LAB)
    img2_lab = cv2.cvtColor(img2, cv2.COLOR_BGR2LAB)
    deltae = np.sqrt(np.sum((img1_lab - img2_lab) ** 2, axis=-1))
    return (np.mean(deltae), np.median(deltae), np.min(deltae), np.max(deltae), deltae)



def plot(wl, values, show: bool, name=None, fmt="{}.png"):
    fig, ax = plt.subplots()
    ax.plot(wl, values)

    ax.set(xlabel='длина волны', ylabel='отн. мощность',
           title=f'Спектр')
    ax.grid()

    if name != None:
        fig.savefig(join("output", "plots", fmt.format(name)))
    if show: 
        plt.show()
    plt.close()

def plot_spd(path: str, show: bool, fmt: str = "{}.png"):
    lst = []
    with open(path, "r") as f:
        for s in f:
            spl = s.split(' ')
            lst.append([float(spl[0]), float(spl[1])])

    spd = np.array(lst)

    name = Path(path).stem

    plot(spd[:, 0], spd[:, 1], show, name, fmt)


def plot_csv(path: str, show: bool, fmt: str = "{}.png"):
    wl=[]
    is_wl = True
    with open(path, "r") as f:
        for s in f:
            spl = s.split(',')
            if is_wl:
                wl = [float(x) for x in spl]
                is_wl = False
            else:
                spd = [float(x) for x in spl]
                plot(wl, spd, True)


def plot_spspec(path: str, show: bool, fmt: str = "{}.png"):
    def sigmoid(x: np.ndarray):
        return 0.5 + 0.5 * x / (1 + x ** 2) ** 0.5

    with open(path, "r") as f:
        s = f.readline().split(" ")
        coef = Polynomial([float(x) for x in s[::-1]])
        wl = np.linspace(360, 830, 100)
    plot(wl, sigmoid(coef(wl)), show)



def run_metrics(image1, image2, out_path: str):
    import cv2
    from skimage.metrics import structural_similarity as ssim
    from skimage.metrics import peak_signal_noise_ratio as psnr

    deltae = mean_deltaE(image1, image2)
    diff_image(deltae[4], out_path) 

    image1_gray = cv2.cvtColor(image1, cv2.COLOR_BGR2GRAY)
    image2_gray = cv2.cvtColor(image2, cv2.COLOR_BGR2GRAY)

    print(f"SSIM: {ssim(image1_gray, image2_gray)}")
    print(f"PSNR: {psnr(image1, image2)}")
    print(f"DeltaE: {deltae[:4]}")


if __name__ == "__main__":
    if(sys.argv[1] == "csv"):
        plot_csv(sys.argv[2], True)
    elif(sys.argv[1] == "spd"):
        plot_spd(sys.argv[2], True)
    elif(sys.argv[1] == "spspec"):
        plot_spspec(sys.argv[2], True)
    elif(sys.argv[1] == "dediff"):
        run_metrics(cv2.imread(sys.argv[2]), cv2.imread(sys.argv[3]), sys.argv[4])



