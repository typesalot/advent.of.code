import time

def time_function(label,func,input):
  print(f"Running {label}... ", end = "")
  start = time.perf_counter()
  result = func(input)
  end = time.perf_counter()
  elapsed = (end - start) * 1000
  print(f"done ({elapsed:,.2f}ms). Result = {result}")