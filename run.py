import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

from subprocess import Popen
from math import ceil


MIN_X = -2985163.8955
MAX_Y = 5227968.786
CELL_SIZE = 10000


def grid_x(x):
    grid_x = round((x - MIN_X) / CELL_SIZE)
    return grid_x


def grid_y(y):
    grid_y = abs(round((y - MAX_Y) / CELL_SIZE))
    return grid_y


CULT = "tupi"
START = 4400
SITE = "encontro"
VEG = 0.0
FOLDER = "tupi"

FISS = 50
K = 10
LEAP = 15
R = 0.02
PERM = 20


Popen(["./expand", f"--cult=tupi", f"--date={START}",
        f"--date-folder={FOLDER}", f"--site={SITE}",
        f"--fiss={FISS}", f"--k={K}", f"--perm={PERM}",
        f"--leap={LEAP}", "--max=0.1", f"--r={R}",
        f"--veg={VEG}", "--write", "--show-bar"]).wait()

a = np.vstack(np.loadtxt("./output/arrival.asc", skiprows=6).astype(float))
a[a == -1] = np.nan
dates = pd.read_csv("./output/dates.csv", header=None).loc[1:,1:2]
dates[1] = dates[1].astype(pd.np.float64)
dates[1] = [grid_x(x) for x in dates[1]]
dates[2] = dates[2].astype(pd.np.float64)
dates[2] = [grid_y(y) for y in dates[2]]
dates = dates.values
scores = pd.read_csv("./output/dates.csv", header=None).loc[1:,3]
scores = scores.astype(pd.np.float64)
scores = scores.values
scores = [ceil(i) for i in scores]


plt.imshow(a)
plt.scatter(*zip(*dates), c=scores, cmap="autumn")
plt.show()
