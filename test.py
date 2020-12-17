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


SETTINGS = [("arawak", 4100, "latia", 0.224, 0.0, "arawak_sw"),
            ("arawak", 4100, "latia", 0.224, 0.5, "arawak_sw"),
            ("arawak", 3000, "saladero", 0.224, 0.0, "arawak_short"),
            ("arawak", 3000, "saladero", 0.224, 0.5, "arawak_short"),
            ("tupi", 4400, "encontro", 0.244, 0.0, "tupi_sw"),
            ("tupi", 4400, "encontro", 0.244, 0.5, "tupi_sw"),
            ("tupi", 2500, "bvista", 0.244, 0.0, "tupi_e"),
            ("tupi", 2500, "bvista", 0.244, 0.5, "tupi_e")]


"""
fiss_vals = [i for i in range(50, 151, 25)]
k_vals = [i for i in range(20, 101, 20)]
leap_vals = [i for i in range(0, 31, 10)]
"""
fiss_vals = [i for i in range(50, 151, 50)]
k_vals = [i for i in range(20, 101, 40)]
leap_vals = [i for i in range(0, 31, 15)]

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
        result = Popen(["./expand", f"--cult={CULTURE}", f"--date={START}", f"--date-folder={DATE_FOLDER}",
                        f"--site={SITE}", f"--fiss={fiss_val}", f"--k={k_val}",
                        f"--perm=10", f"--leap={leap_val}", f"--max={MAXENT}", f"--veg={VEG}"], stdout=PIPE).communicate()[0]
        return {params: float(result)}


    combs = []
    for fiss_val in fiss_vals:
        for k_val in k_vals:
            for leap_val in leap_vals:
                combs.append([fiss_val, k_val, leap_val])

    pool = mp.Pool(3)
    res = pool.map(run, combs)
    pool.close()

    with open(f"res{int(time.time())}.csv", "w") as f:
        f.write("fiss,k,leap,score\n")
        for r in res:
            for k in r:
                params = k.split(' ')
                f.write(f"{params[0]},{params[1]},{params[2]},{r[k]}\n")
        f.write(f"\nCult: {CULTURE} | Start: {START} | Site: {SITE} | Veg: {VEG}")
