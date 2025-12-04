from util import *

def parse_input(lines):
  return lines

def max_joltage(line,remaining):
  # compute a map of digit locations
  # create a second set of all unique digits
  loc_map = {}
  unq_digits = set()
  for i in range(len(line)):
    c = line[i]
    loc_map.setdefault(c,[]).append(i)
    unq_digits.add(c)
  
  # sort the set from largest to smallest so we can take a greedy approach
  unq_digits = sorted(unq_digits,reverse=True)
  
  # greedily the largest available numbers moving from left to right
  digits = []
  last = -1
  while remaining > 0:
    found = False

    # for each unique digit available...
    for d in unq_digits:

      # look up the next available local in the map...
      for u in loc_map[d]:

        # skip it if there is not enough space after it for
        # any of the remaining characters
        if u + remaining > len(line):
          continue

        # only accumulate it if its position is to the right (greater)
        # than the last
        if u > last:
          last = u
          digits.append(d)
          loc_map[d] = loc_map[d][1:]
          found = True
          break
      
      if found:
        remaining = remaining - 1
        break

  result = int("".join(str(d) for d in digits))
  return result

def part1(input):
  j_sum = 0
  for i in input:
    j_sum = j_sum + max_joltage(i,2)
  return j_sum

def part2(input):
  j_sum = 0
  for i in input:
    j_sum = j_sum + max_joltage(i,12)
  return j_sum

input_sample = read_sample(parse_input)
input = read_input(parse_input)

time_function("Part1 Sample",part1,input_sample)
time_function("Part1",part1,input)
time_function("Part2 Sample",part2,input_sample)
time_function("Part2",part2,input)