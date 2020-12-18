import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

from subprocess import Popen


Popen(["./expand", f"--cult=arawak", f"--date=1100",
        f"--date-folder=arawak_sw", f"--site=latia",
        f"--fiss=150", f"--k=20", f"--perm=10",
        f"--leap=15", f"--max=0.22",
        f"--veg=0.0", "--write", "--show-bar"]).wait()

a = np.vstack(np.loadtxt("./output/arrival.asc", skiprows=6).astype(float))
a[a == -1] = np.nan
dates = pd.read_csv("./output/dates.csv", header=None).loc[1:,:1]
dates[0] = dates[0].astype(pd.np.float64)
dates[1] = dates[1].astype(pd.np.float64)
dates = dates.values
scores = pd.read_csv("./output/dates.csv", header=None).loc[1:,2]
scores = scores.astype(pd.np.float64)
scores = scores.values

plt.imshow(a)
#plt.scatter(*zip(*dates), c=scores, cmap="gray")
plt.show()
