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
  def is_valid(num):
    snum = str(num)
    sl = len(snum)
    l = sl // 2
    while l > 0:
      c = snum.count(snum[0:l],l) + 1
      if c * l == sl:
        return False
      l = l - 1
    return True
  
  total = 0
  for r in input:
    for num in range(r[0],r[1]+1):
      if not is_valid(num):
        total = total + num
  return total

def part2_better(input):
  def is_valid(n: int) -> bool:
      s = str(n)
      # [1:-1] trims off the outer characters so that only true repeated-pattern 
      # strings will be found inside the doubled string.
      # Example:  abab      s
      #           abababab  (s+s)
      #            bababa   [1:-1]
      #             ----    s (yes)
      #           abac
      #           abacabac
      #            bacaba
      #            ??????   no 'abac'
      #           824824824
      #           824824824824824824
      #            2482482482482482
      #              --------- s, yes
      if s in (s + s)[1:-1]:
          return False
      return True
  
  total = 0
  for r in input:
    for num in range(r[0],r[1]+1):
      if not is_valid(num):
        total = total + num
  return total

input_sample = parseInput(True)
input = parseInput()

time_function("Part1 Sample",part1,input_sample)
time_function("Part1",part1,input)

time_function("Part2 Sample",part2,input_sample)
time_function("Part2 Mine",part2,input)
time_function("Part2 Better",part2_better,input)