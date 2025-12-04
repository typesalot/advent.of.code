from util import *

def parse_input(lines):
  return lines

def part1(input):
  def max_joltage(line):
    loc_map = {}
    unq_digits = set()
    for i in range(len(line)):
      c = line[i]
      loc_map.setdefault(c,[]).append(i)
      unq_digits.add(c)
    
    unq_digits = sorted(unq_digits,reverse=True)
    for u in unq_digits:
      for k in unq_digits:
        i = loc_map[u][-1]
        j = loc_map[k][-1]
        if u == k:
          if len(loc_map[u]) == 1:
            continue
          else:
            j = i
        
        if i <= j:
          result = int(u) * 10 + int(k)
          return result
    return 0
  
  j_sum = 0
  for i in input:
    j_sum = j_sum + max_joltage(i)

  return j_sum

def part2(input):
  return 0

input_sample = read_sample(parse_input)
input = read_input(parse_input)

time_function("Part1 Sample",part1,input_sample)
time_function("Part1",part1,input)
# 16412 too low
# 16898 too low
# 17467 too low
# 17694 correct

time_function("Part2 Sample",part2,input_sample)
time_function("Part2",part2,input)