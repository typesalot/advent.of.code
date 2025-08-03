from aoc import bench, readInput
import re

StrVector = list[str]

m = re.compile(r"[0-9]")
nums = [ "one", "two", "three", "four", "five", "six", "seven", "eight", "nine" ]
nums_rev = [n[::-1] for n in nums]
m_long0 = re.compile( "[0-9]|" + "|".join(nums) )
m_long1 = re.compile( "[0-9]|" + "|".join(nums_rev) )

def calcCalibrationValue(line : str) -> int:
  l = int( m.search(line).group() )
  r = int( m.search(line[::-1]).group() )
  return l*10 + r

def calcCalibrationValueFixed(line : str) -> int:
  l = m_long0.search(line)
  r = m_long1.search(line[::-1])
  if len(l.group()) == 1:
    l = int(l.group())
  else:
    l = nums.index(l.group()) + 1
  if len(r.group()) == 1:
    r = int(r.group())
  else:
    r = nums_rev.index(r.group()) + 1
  return l*10 + r

def calcCalibrationValues( lines : StrVector ) -> int:
  val = 0
  for l in lines:
    val = val + calcCalibrationValue(l)
  return val

def calcCalibrationValuesFixed( lines : StrVector ) -> int:
  val = 0
  for l in lines:
    val = val + calcCalibrationValueFixed(l)
  return val

def part1() -> None:
  lines = readInput(1)
  answer = calcCalibrationValues(lines)
  print(f"\tPart1 = {answer}")

def part2() -> None:
  lines = readInput(1)
  answer = calcCalibrationValuesFixed(lines)
  print(f"\tPart2 = {answer}")

print("Day01")
bench(part1)
bench(part2)