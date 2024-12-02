# source: https://www.reddit.com/r/adventofcode/comments/1h4ncyr/comment/m03qfic/?utm_source=share&utm_medium=web3x&utm_name=web3xcss&utm_term=1&utm_content=share_button

from itertools import pairwise

def is_safe(levels):
    diff = set(y - x for x, y in pairwise(levels))
    return (diff <= {1, 2, 3}) or (diff <= {-1, -2, -3})


def is_tolerated(levels):
    for i in range(len(levels)):
        if is_safe(levels[:i] + levels[i+1:]):
            return True
    return False


with open("input/day02.input.txt") as f:
    reports = [[int(x) for x in l.split()] for l in f.readlines()]


# ==== PART 1 ====
print(sum(is_safe(r) for r in reports))

# ==== PART 2 ====
print(sum(is_safe(r) or is_tolerated(r) for r in reports))