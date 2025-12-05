'''
Advent of Code, 2025, Day 4

'''
from util import *

def parse_input(lines):
  return lines

def part1(input):
  return 0

def part2(input):
  return 0

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