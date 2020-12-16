from subprocess import Popen, PIPE
from tqdm import tqdm


CULTURE = "tupi"
START = 2500
SITE = "bvista"
MAXENT = 0.244
VEG = 0.5


fiss_vals = [50, 150, 250]
k_vals = [20, 50, 80]
leap_vals = [0, 10, 20]


for fiss_val in fiss_vals:
    for k_val in k_vals:
        for leap_val in leap_vals:
            result = Popen(["./expand", f"--cult={CULTURE}", f"--date={START}", f"--site={SITE}",
                            f"--fiss={fiss_val}", f"--k={k_val}", f"--perm=10", f"--leap={leap_val}",
                            f"--max={MAXENT}", f"--veg={VEG}"], stdout=PIPE).communicate()[0]
            params = f"{fiss_val} {k_val} {leap_val}"
            print(params, float(result))
