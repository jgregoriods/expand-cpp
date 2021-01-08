from subprocess import Popen, PIPE
import time
import multiprocessing as mp


start_time = time.time()
def show_bar(it, size):
    pct = round(it / size * 30)
    if it != size:
        suff = ">"
    else:
        suff = "="
    fill = ("=" * (pct - 1)) + suff
    empty = "." * (30 - pct)
    now = time.time()
    hours = str(int(now - start_time) // 3600).zfill(2)
    mins = str(int((int(now - start_time) % 3600) // 60)).zfill(2)
    secs = str(int((int(now - start_time) % 3600) % 60)).zfill(2)
    print(f"[{fill}{empty}] {it}/{size} {hours}:{mins}:{secs}", end="\r")
    if it == size:
        print("\n")


SETTINGS = [("tupi", 4400, "encontro", 0.0, -1.0, "tupi"),
            ("tupi", 4400, "encontro", 0.0, 0.5, "tupi")]

PARAMS = []

large = [150, 100]
medium = [100, 50]
small = [50, 10]

fiss_vals = [i for i in range(50, 151, 10)]
k_vals = [i for i in range(10, 101, 10)]
leap_vals = [0, 5, 10, 15]

"""
for i in leap_vals:
    PARAMS.append(large + [i])
    PARAMS.append(medium + [i])
    PARAMS.append(small + [i])
"""

for SETTING in SETTINGS:

    CULTURE = SETTING[0]
    START = SETTING[1]
    SITE = SETTING[2]
    MAXENT = SETTING[3]
    VEG = SETTING[4]
    DATE_FOLDER = SETTING[5]

    
    def run(vals):
        fiss_val = vals[0]
        k_val = vals[1]
        leap_val = vals[2]
        params = f"{fiss_val} {k_val} {leap_val}"
        result = Popen(["./expand", f"--cult={CULTURE}", f"--date={START}",
                        f"--date-folder={DATE_FOLDER}", f"--site={SITE}",
                        f"--fiss={fiss_val}", f"--k={k_val}", f"--perm=10",
                        f"--leap={leap_val}", f"--max={MAXENT}",
                        f"--veg={VEG}"], stdout=PIPE).communicate()[0]
        result = result.strip().split()
        return {params: result}


    combs = []
    for fiss_val in fiss_vals:
        for k_val in k_vals:
            for leap_val in leap_vals:
                if fiss_val > k_val:
                    combs.append([fiss_val, k_val, leap_val])

    pool = mp.Pool(11)
    res = pool.map(run, combs)
    #res = pool.map(run, PARAMS)
    pool.close()

    with open(f"res_3pct_{int(time.time())}.csv", "w") as f:
        f.write("fiss,k,leap,score,dates\n")
        for r in res:
            for k in r:
                params = k.split(' ')
                f.write(f"{params[0]},{params[1]},{params[2]},{float(r[k][0])},{int(r[k][1])}\n")
        f.write(f"\nCult: {CULTURE} | Start: {START} | Site: {SITE} | Veg: {VEG}")
