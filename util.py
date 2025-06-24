import random
import subprocess
def gen_pts(n):
    return [(random.randint(0, 1000), random.randint(0, 1000)) for _ in range(n)]

def save(fn, pts):
    with open(fn, 'w') as f:
        f.write(f"{len(pts)}\n")
        for x, y in pts:
            f.write(f"{x} {y}\n")

def run_prog(fn):
    try:
        subprocess.run(["./main", fn], check=True)
        print(f"Executed './main {fn}' successfully.")
    except subprocess.CalledProcessError as e:
        print(f"Error executing './main {fn}': {e}")

def gen_samples(s=10, e=1000):
    for n in range(s, e + 1):
        fn = f"input{n}"
        pts = gen_pts(n)
        save(fn, pts)
        print(f"Created '{fn}' with {n} points.")
        run_prog(fn)

if __name__ == "__main__":
    gen_samples()
