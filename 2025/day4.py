'''
Advent of Code, 2025, Day 4

Running Part1 Sample.................. done (0.24ms). Result = 
Running Part1 (Mine).................. done (6.49ms). Result = 
No chatgpt solution explored.
Running Part2 Sample.................. done (0.40ms). Result = 
Running Part2 (Mine).................. done (231.20ms). Result = 
No chatgpt solution explored.
'''
from util import *

def parse_input(lines):
  input = []
  # pad the boundaries so we don't have to bounds check
  cols = len(lines[0]) + 2
  input.append(' '*cols)
  for l in lines:
    input.append(f' {l} ')
  input.append(' '*cols)
  return input

def part1(input):
  cnt = 0
  rows = len(input)
  cols = len(input[0])

  def tl(j,i): return input[j-1][i-1]
  def tc(j,i): return input[j-1][i+0]
  def tr(j,i): return input[j-1][i+1]
  def cl(j,i): return input[j-0][i-1]
  def cc(j,i): return input[j-0][i+0]
  def cr(j,i): return input[j-0][i+1]
  def bl(j,i): return input[j+1][i-1]
  def bc(j,i): return input[j+1][i+0]
  def br(j,i): return input[j+1][i+1]

  for j in range(1,cols-1):
    for i in range(1,rows-1):
      if cc(j,i) != '@': continue
      k_cnt = 0
      if tl(j,i) == '@': k_cnt += 1
      if tc(j,i) == '@': k_cnt += 1
      if tr(j,i) == '@': k_cnt += 1
      if cl(j,i) == '@': k_cnt += 1
      if cr(j,i) == '@': k_cnt += 1
      if bl(j,i) == '@': k_cnt += 1
      if bc(j,i) == '@': k_cnt += 1
      if br(j,i) == '@': k_cnt += 1
      if k_cnt < 4:
        cnt += 1
  return cnt

def part2(input):
  total_cnt = 0
  rows = len(input)
  cols = len(input[0])

  def tl(j,i): return input[j-1][i-1]
  def tc(j,i): return input[j-1][i+0]
  def tr(j,i): return input[j-1][i+1]
  def cl(j,i): return input[j-0][i-1]
  def cc(j,i): return input[j-0][i+0]
  def cr(j,i): return input[j-0][i+1]
  def bl(j,i): return input[j+1][i-1]
  def bc(j,i): return input[j+1][i+0]
  def br(j,i): return input[j+1][i+1]

  while True:
    local_cnt = 0
    remove = []
    for j in range(1,cols-1):
      for i in range(1,rows-1):
        if cc(j,i) != '@': continue
        k_cnt = 0
        if tl(j,i) == '@': k_cnt += 1
        if tc(j,i) == '@': k_cnt += 1
        if tr(j,i) == '@': k_cnt += 1
        if cl(j,i) == '@': k_cnt += 1
        if cr(j,i) == '@': k_cnt += 1
        if bl(j,i) == '@': k_cnt += 1
        if bc(j,i) == '@': k_cnt += 1
        if br(j,i) == '@': k_cnt += 1
        if k_cnt < 4:
          local_cnt += 1
          remove.append([j,i])
    if local_cnt == 0:
      break
    else:
      total_cnt += local_cnt
      for r in remove:
        rj = r[0]
        ri = r[1]
        input[rj] = input[rj][:ri] + 'x' + input[rj][ri+1:]
  return total_cnt

input_sample = read_sample(parse_input)
input = read_input(parse_input)
args = get_args()

if args.samples:
  time_function("Part1 Sample",part1,input_sample)
if args.input:
  time_function("Part1 (Mine)",part1,input)
if args.chatgpt:
  print( "No chatgpt solution explored." )

if args.samples:
  time_function("Part2 Sample",part2,input_sample)
if args.input:
  time_function("Part2 (Mine)",part2,input)
if args.chatgpt:
  print( "No chatgpt solution explored." )