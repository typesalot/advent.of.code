import os
from util import *

def parseInput(useSample=False):
  input = []
  lines = []
  assets_path = os.path.expanduser(os.path.join("~","assets", "advent.of.code", "2025"))
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
  return 0

results = []

input = parseInput(True)
time_function("Part1 Sample",part1,input,results)
time_function("Part2 Sample",part2,input,results)

input = parseInput()
time_function("Part1",part1,input,results)
time_function("Part2",part2,input,results)

print_results("2025 Day2", results)