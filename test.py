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


SETTINGS = [(4400, "encontro")]#, (5800, "urupa")]

veg_vals = [0.0, 0.4]#, 0.5, 0.6]
max_vals = [0.0, 0.25]
fiss_vals = [50, 100, 150]
r_vals = [0.02, 0.03, 0.04]
k_vals = [10, 50, 100]
leap_vals = [0, 5, 15]
perm_vals = [10, 20, 30]


for SETTING in SETTINGS:

    START = SETTING[0]
    SITE = SETTING[1]
    
    def run(vals):
        veg_val = vals[0]
        max_val = vals[1]
        fiss_val = vals[2]
        r_val = vals[3]
        k_val = vals[4]
        leap_val = vals[5]
        perm_val = vals[6]
        params = f"{veg_val} {max_val} {fiss_val} {r_val} {k_val} {leap_val} {perm_val}"
        result = Popen(["./expand", f"--cult=tupi", f"--date={START}",
                        f"--date-folder=tupi", f"--site={SITE}",
                        f"--fiss={fiss_val}", f"--k={k_val}", f"--perm={perm_val}",
                        f"--leap={leap_val}", f"--max={max_val}",
                        f"--veg={veg_val}", f"--r={r_val}"],
                        stdout=PIPE).communicate()[0]
        result = result.strip().split()
        return {params: result}


    combs = []
    for veg_val in veg_vals:
        for max_val in max_vals:
            for fiss_val in fiss_vals:
                for r_val in r_vals:
                    for k_val in k_vals:
                        for leap_val in leap_vals:
                            for perm_val in perm_vals:
                                if fiss_val > k_val:
                                    combs.append([veg_val, max_val, fiss_val, r_val, k_val, leap_val, perm_val])

    pool = mp.Pool(11)
    res = pool.map(run, combs)
    #res = pool.map(run, PARAMS)
    pool.close()

    with open(f"res{int(time.time())}.csv", "w") as f:
        f.write("veg,max,fiss,r,k,leap,perm,score\n")
        for r in res:
            for k in r:
                params = k.split(' ')
                f.write(f"{params[0]},{params[1]},{params[2]},{params[3]},{params[4]},{params[5]},{params[6]},{float(r[k])}\n")
        f.write(f"\nCult: tupi | Start: {START} | Site: {SITE}")
