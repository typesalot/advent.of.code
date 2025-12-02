import os
import time

sample = """
L68
L30
R48
L5
R60
L55
L1
L99
R14
L82
"""

def parseInput(useSample=False):
  input = []
  lines = []
  if not useSample:
    assets_path = os.path.expanduser("~/assets/advent.of.code/2025")
    f = open(os.path.join(assets_path,'day1.txt'),'r')
    lines = f.readlines()
    f.close()
  else:
    lines = [ l for l in sample.splitlines() if l != "" ]
  for l in lines:
    direction = l[0]
    amount = int(l[1:].strip())
    input.append([direction,amount])
  return input

def part1(input):
  cur = 50
  cnt = 0
  for i in input:
    if i[0] == 'L':
      cur = ( cur - i[1] + 100 ) % 100
    else:
      cur = ( cur + i[1] - 100 ) % 100
    if cur == 0:
      cnt = cnt + 1
  return cnt

def part2(input):
  cur = 50
  cnt = 0
  for i in input:
    direction = i[0]
    ticks = i[1]
    if direction == 'L':
      if cur <= ticks:
        cnt = cnt + int(( ticks - cur ) / 100)
        if cur != 0:
          cnt = cnt + 1
      cur = ( cur - ticks + 100 ) % 100
    else:
      t = cur + ticks
      if t >= 100:
        cnt = cnt + int( t  / 100 )
      cur = ( t - 100 ) % 100
  return cnt

print("2025, Day1")

input = parseInput()
start = time.perf_counter()
result = part1(input)
end = time.perf_counter()
elapsed_ms = (end - start) * 1000
print(f"Part1: {result}, Elapsed: {elapsed_ms:.3f} ms")

input = parseInput()
start = time.perf_counter()
result = part2(input)
end = time.perf_counter()
elapsed_ms = (end - start) * 1000
print(f"Part2: {result}, Elapsed: {elapsed_ms:.3f} ms")