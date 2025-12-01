import os
import time

def parseInput():
  input = []
  assets_path = os.path.expanduser("~/assets/advent.of.code/2025")
  f = open(os.path.join(assets_path,'day1.txt'),'r')
  lines = f.readlines()
  f.close()
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
    if i[0] == 'L':
      cur = ( cur - i[1] + 100 ) % 100
    else:
      cur = ( cur + i[1] - 100 ) % 100
    if cur == 0:
      cnt = cnt + 1
  return cnt

print("2025, Day1")

input = parseInput()
start = time.perf_counter()
result = part1(input)
end = time.perf_counter()
elapsed_ms = (end - start) * 1000
print(f"Part1: {result}, Elapsed: {elapsed_ms:.3f} ms")

start = time.perf_counter()
result = part2(input)
end = time.perf_counter()
elapsed_ms = (end - start) * 1000
print(f"Part2: {result}, Elapsed: {elapsed_ms:.3f} ms")