import time
import os
import inspect
import argparse

def get_args():
  parser = argparse.ArgumentParser(description="Demo boolean flags")

  parser.add_argument(
    "--samples",
    action="store_true",
    help="Use sample input" )
  
  parser.add_argument(
    "--input",
    action="store_true",
    help="Use real input" )

  parser.add_argument(
    "--chatgpt",
    action="store_true",
    help="Show ChatGPT solutions" )
  
  args = parser.parse_args()
  if not args.input and not args.chatgpt and not args.samples:
    args.input = True
    args.chatgpt = True
    args.samples = True
  
  return args

def time_function(label,func,input):
  print(f"Running {label:.<30} ", end = "")
  start = time.perf_counter()
  result = func(input)
  end = time.perf_counter()
  elapsed = (end - start) * 1000
  print(f"done ({elapsed:,.2f}ms). Result = {result}")

def read_sample(parse):
   return read_input(parse,True)

def read_input(parse, useSample = False):
  lines = []

  # walk up the stack until we find the parent day##.py filename to use as a key
  # to grab the corresponding input
  fname = __file__
  stack_index = 1
  while fname == __file__:
    frame = inspect.stack()[stack_index]
    fname = frame.filename
    stack_index = stack_index + 1

  if useSample:
    fname = os.path.basename(fname).replace('.py', 'sample.txt')
  else:
    fname = os.path.basename(fname).replace('.py', '.txt')

  filePath = os.path.expanduser(os.path.join("~","assets", "advent.of.code", "2025"))
  fname = os.path.join(filePath,fname)

  f = open(fname,'r')
  lines = [ l.strip() for l in f.readlines() ]
  f.close()
  return parse(lines)