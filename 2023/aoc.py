import os
import time

def getInputFile( day ):
  p = f"~/assets/advent.of.code/2023/{day:02}.txt"
  return os.path.expanduser(p)

def readInput( day ):
  filename = getInputFile(day)
  f = open(filename)
  lines = [ l.strip() for l in f.readlines() ]
  f.close()
  return lines

def bench(func):
    start = time.perf_counter()
    func()
    end = time.perf_counter()
    elapsed_ms = (end - start) * 1000
    print(f"\t{elapsed_ms:.2f} ms")