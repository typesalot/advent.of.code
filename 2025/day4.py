'''
Advent of Code, 2025, Day 4

Running Part1 Sample.................. done (0.24ms). Result = 
Running Part1 (Mine).................. done (6.53ms). Result = X
Running Part1 (ChatGPT)............... done (25.55ms). Result = X
Running Part2 Sample.................. done (0.47ms). Result = 
Running Part2 (Mine).................. done (229.56ms). Result = Y
Running Part2 (ChatGPT)............... done (34.57ms). Result = Y
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

from collections import deque
import sys

# 8-directional neighbors (Moore neighborhood)
NEIGHBORS = [
    (-1, -1), (-1, 0), (-1, 1),
    (0, -1),           (0, 1),
    (1, -1),  (1, 0),  (1, 1),
]

def read_grid(input):
    lines = [line.rstrip("\n") for line in input if line.strip() != ""]
    return lines

def build_graph(grid):
    """
    Build:
      - nodes: all (r, c) where grid[r][c] == '@'
      - degree: number of adjacent '@' in 8 directions
    Returns:
      nodes: set of (r, c)
      degree: dict[(r, c)] -> int
      neighbors: dict[(r, c)] -> list[(nr, nc)]
    """
    h = len(grid)
    w = len(grid[0]) if h > 0 else 0

    nodes = set()
    for r in range(h):
        for c in range(w):
            if grid[r][c] == '@':
                nodes.add((r, c))

    # Precompute neighbors only within nodes
    neighbors = {pos: [] for pos in nodes}
    degree = {pos: 0 for pos in nodes}

    for r, c in nodes:
        for dr, dc in NEIGHBORS:
            nr, nc = r + dr, c + dc
            if (nr, nc) in nodes:
                neighbors[(r, c)].append((nr, nc))
                degree[(r, c)] += 1

    return nodes, degree, neighbors

def solve_part1(degree):
    """Count rolls initially accessible (degree < 4)."""
    return sum(1 for d in degree.values() if d < 4)

def solve_part2(nodes, degree, neighbors):
    """
    Dynamic removal:
      - Any node with degree < 4 can be removed.
      - When removed, all its neighbors lose 1 degree.
      - This may cause more nodes to become removable.
    This is exactly a 'k-core peeling' with k = 4.
    """
    # Copy degrees so we don't mutate the original dict used for part 1
    deg = dict(degree)

    removed = set()
    q = deque()

    # Start with all nodes that are immediately accessible
    for pos, d in deg.items():
        if d < 4:
            q.append(pos)

    total_removed = 0

    while q:
        pos = q.popleft()
        if pos in removed:
            continue
        removed.add(pos)
        total_removed += 1

        # Removing pos decreases degree of all neighbors
        for nb in neighbors[pos]:
            if nb in removed:
                continue
            before = deg[nb]
            deg[nb] = before - 1
            # A node becomes accessible as soon as its degree drops below 4.
            # That first happens exactly when it becomes 3.
            if deg[nb] == 3:
                q.append(nb)

    return total_removed

def part1_chat_gpt(input):
    _, degree, _ = build_graph(input)
    return solve_part1(degree)

def part2_chat_gpt(input):
    nodes, degree, neighbors = build_graph(input)
    return solve_part2(nodes, degree, neighbors)

input_sample = read_sample(parse_input)
input = read_input(parse_input)
input_chatgpt = read_input(read_grid)
args = get_args()

if args.samples:
  time_function("Part1 Sample",part1,input_sample)
if args.input:
  time_function("Part1 (Mine)",part1,input)
if args.chatgpt:
  time_function("Part1 (ChatGPT)",part1_chat_gpt,input_chatgpt)

if args.samples:
  time_function("Part2 Sample",part2,input_sample)
if args.input:
  time_function("Part2 (Mine)",part2,input)
if args.chatgpt:
  time_function("Part2 (ChatGPT)",part2_chat_gpt,input_chatgpt)