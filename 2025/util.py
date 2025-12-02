import time
import re

def time_function(label,func,input,results):
  start = time.perf_counter()
  result = func(input)
  end = time.perf_counter()
  elapsed = (end - start) * 1000
  results.append( [ label, result, elapsed ] )

def print_results(header, rows):
  print(header)

  rows.sort(key=lambda d: d[0])

  # Column formats (NO WIDTH)
  col_fmts = [
      "{:>}",      # left align
      "{:>}",     # right + comma
      "{:>.3f}",   # center with 2 decimals
  ]

  # 1) Format without widths
  formatted_rows_no_width = [
      [col_fmts[i].format(value) for i, value in enumerate(row)]
      for row in rows
  ]

  # 2) Compute column widths
  widths = [
      max(len(row[i]) for row in formatted_rows_no_width)
      for i in range(len(rows[0]))
  ]

  # Regex to find the format specifier inside { … }
  spec_re = re.compile(r"{:(.*)}")

  def insert_width(fmt: str, width: int) -> str:
      """
      Insert width before precision/type.
      Example: "<"       -> "<10"
              ",>"      -> ",>10"
              "^.2f"    -> "^10.2f"
      """
      spec = spec_re.match(fmt).group(1)

      # Find first of .precision or type letter
      m = re.search(r"[a-zA-Z.]",
                    spec)  # match type letter or precision dot

      if not m:
          # no precision/type: just append width
          new_spec = f"{spec}{width}"
      else:
          idx = m.start()
          new_spec = spec[:idx] + str(width) + spec[idx:]

      return "{:" + new_spec + "}"


  # 3) Apply width-inserted formats
  aligned_rows = []
  for row in rows:
      out = []
      for i, value in enumerate(row):
          fmt = insert_width(col_fmts[i], widths[i])
          out.append(fmt.format(value))
      aligned_rows.append(out)


  # 4) Print result
  for r in aligned_rows:
      print(", ".join(r))