import os
import time

def parseInput(useSample=False):
  input = []
  lines = []
  assets_path = os.path.expanduser("~/assets/advent.of.code/2025")
  fileName = os.path.join(assets_path,'day2.txt')
  if useSample:
    fileName = fileName.replace('.txt', 'sample.txt')
  f = open(fileName,'r')
  lines = f.readlines()
  f.close()

  for l in lines:
    ranges = l.strip().split(',')
    for range in ranges:
      if range != '':
        lower,upper = range.split('-')
        input.append([int(lower),int(upper)])
  return input

def part1(input):
  def is_valid(num):
    snum = str(num)
    if len(snum) % 2 != 0:
      return False
    l = 0
    m = len(snum) // 2
    while m < len(snum):
      if snum[l] != snum[m]:
        return False
      l = l + 1
      m = m + 1
    return True
  
  total = 0
  for r in input:
    for num in range(r[0],r[1]+1):
      if is_valid(num):
        total = total + num
  return total

def part2(input):
  pass

print("2025, Day2")

input = parseInput(True)
start = time.perf_counter()
result = part1(input)
end = time.perf_counter()
elapsed_ms = (end - start) * 1000
print(f"Part1 Sample: {result}, Elapsed: {elapsed_ms:.3f} ms")

input = parseInput()
start = time.perf_counter()
result = part1(input)
end = time.perf_counter()
elapsed_ms = (end - start) * 1000
print(f"Part1: {result}, Elapsed: {elapsed_ms:.3f} ms")

input = parseInput(True)
start = time.perf_counter()
result = part2(input)
end = time.perf_counter()
elapsed_ms = (end - start) * 1000
print(f"Part2 Sample: {result}, Elapsed: {elapsed_ms:.3f} ms")

input = parseInput()
start = time.perf_counter()
result = part2(input)
end = time.perf_counter()
elapsed_ms = (end - start) * 1000
print(f"Part2: {result}, Elapsed: {elapsed_ms:.3f} ms")