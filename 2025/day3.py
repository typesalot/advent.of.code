'''
Advent of Code, 2025, Day 3

Running Part1 Sample.................. done (0.04ms). Result = 
Running Part1 (Mine).................. done (2.62ms). Result = 
Running Part1 (ChatGPT)............... done (2.00ms). Result = 
Running Part1 (ChatGPT,Optimized)..... done (1.89ms). Result = 
Running Part2 Sample.................. done (0.04ms). Result = 
Running Part2 (Mine).................. done (3.81ms). Result = 
Running Part2 (ChatGPT)............... done (1.92ms). Result = 
Running Part2 (ChatGPT,Optimized)..... done (1.81ms). Result = 
'''
from util import *
from collections import deque

def parse_input(lines):
  return lines

def max_joltage(line,remaining):
  # compute a map of digit locations
  # create a second set of all unique digits
  loc_map = {}
  unq_digits = set()
  for i in range(len(line)):
    c = line[i]
    loc_map.setdefault(c,deque()).append(i)
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
      q = loc_map[d]
      while q and q[0] <= last:
        q.popleft()
      if not q:
        continue
      u = q[0]
      if u + remaining > len(line):
        continue
      if u > last:
        last = u
        digits.append(d)
        loc_map[d].popleft()
        found = True
        break
    if found:
      remaining = remaining - 1

  result = int("".join(str(d) for d in digits))
  return result

def max_joltage_gpt(line, remaining):
    # line is a string of digits, e.g. "987654321111111"
    n = len(line)
    remove = n - remaining  # how many digits we are allowed to drop
    stack = []

    for ch in line:
        # While we can still remove digits AND the last chosen digit
        # is smaller than current, pop it to make number larger.
        while remove > 0 and stack and stack[-1] < ch:
            stack.pop()
            remove -= 1
        stack.append(ch)

    # If we still have removals left, drop from the end
    if remove > 0:
        stack = stack[:-remove]

    # Keep exactly `remaining` digits (in case of any off-by-one)
    digits = stack[:remaining]

    return int("".join(digits)) if digits else 0

def max_joltage_gpt_optimized(line, remaining):
    # Assume `line` is a clean string of digits (no newline).
    n = len(line)
    remove = n - remaining

    stack = []
    append = stack.append
    pop = stack.pop

    # Localize frequently-used vars
    _stack = stack
    _remove = remove

    for ch in line:
        # Pop while we can still remove and improve the number
        # Use local vars to avoid globals / attribute lookups
        while _remove and _stack and _stack[-1] < ch:
            pop()
            _remove -= 1
        append(ch)

    # Write back local remove (not strictly needed if you only use _remove)
    remove = _remove

    # If we still have removals left, bulk-delete from the end
    if remove:
        del stack[-remove:]

    # Truncate to exactly `remaining` just in case
    if len(stack) > remaining:
        del stack[remaining:]

    # Single C-level parse; keep digits as chars throughout
    return int("".join(stack)) if stack else 0

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

def part1_gpt(input):
  j_sum = 0
  for i in input:
    j_sum = j_sum + max_joltage_gpt(i,2)
  return j_sum

def part2_gpt(input):
  j_sum = 0
  for i in input:
    j_sum = j_sum + max_joltage_gpt(i,12)
  return j_sum

def part1_gpt_optimized(input):
  j_sum = 0
  for i in input:
    j_sum = j_sum + max_joltage_gpt_optimized(i,2)
  return j_sum

def part2_gpt_optimized(input):
  j_sum = 0
  for i in input:
    j_sum = j_sum + max_joltage_gpt_optimized(i,12)
  return j_sum

input_sample = read_sample(parse_input)
input = read_input(parse_input)
args = get_args()

if args.samples:
  time_function("Part1 Sample",part1,input_sample)
if args.input:
  time_function("Part1 (Mine)",part1,input)
if args.chatgpt:
  time_function("Part1 (ChatGPT)",part1_gpt,input)
  time_function("Part1 (ChatGPT,Optimized)",part1_gpt_optimized,input)

if args.samples:
  time_function("Part2 Sample",part2,input_sample)
if args.input:
  time_function("Part2 (Mine)",part2,input)
if args.chatgpt:
  time_function("Part2 (ChatGPT)",part2_gpt,input)
  time_function("Part2 (ChatGPT,Optimized)",part2_gpt_optimized,input)