# 2024

| Day | Part1          | Part2           |
| --- | -------------- | --------------- |
| 1   | :green_circle: | :green_circle:  |
| 2   | :green_circle: | :green_circle:  |
| 3   | :green_circle: | :green_circle:  |
| 4   | :green_circle: | :green_circle:  |
| 5   | :green_circle: | :green_circle:  |
| 6   | :green_circle: | :orange_circle: |
| 7   | :green_circle: | :green_circle:  |
| 8   | :green_circle: | :green_circle:  |
| 9   | :green_circle: | :green_circle:  |
| 10  | :green_circle: | :green_circle:  |
| 11  | :green_circle: | :green_circle:  |
| 12  | :green_circle: | :black_circle:  |
| 13  | :green_circle: | :black_circle:  |
| 14  | :green_circle: | :green_circle:  |
| 15  | :green_circle: | :black_circle:  |
| 16  | :green_circle: | :black_circle:  |
| 17  | :green_circle: | :green_circle:  |
| 18  | :green_circle: | :green_circle:  |
| 19  | :green_circle: | :green_circle:  |
| 20  | :green_circle: | :green_circle:  |
| 21  | :green_circle: | :black_circle:  |
| 22  | :black_circle: | :black_circle:  |
| 23  | :black_circle: | :black_circle:  |
| 24  | :black_circle: | :black_circle:  |
| 25  | :black_circle: | :black_circle:  |

## Journal

### Day 10: Hoof It

Path finding both to unique destinations and unique paths. I misunderstood the problem initially and solved Part2 as Part1. Since the max depth is only 9, I used recursion to navigate the map. I also took a dynamic programming approach to keep a history of traveled score/rating per location to avoid re-navigating previous paths.

### Day 11: Plutonian Pebbles

A lot of common subcases that can be cached to avoid re-calculation, with the added complexity that the cache needs to be blink-level aware. I used recursion as well since Part2's 75 blinks means a manageable function depth.

### Day 12: Garden Groups

Pricing a fence using area and perimeter and sides.

### Day 13: Claw Contraption

How to efficiently compute the number of button presses to reach a target. I did part1 with a simple permutation and cache to early exit common cases, but it doesn't scale for part2. Will need to revisit.

### Day 14: Restroom Redoubt

Have to correctly handle the wrapping in both the underflow and overflow cases and it pretty straight-forward. Finding the tree in Part2 is cute and it takes a little bit of guess work to figure out what the tree is supposed to look like.

### Day 15: Warehouse Woes

Moving boxes; fun puzzle.

### Day 16: Reindeer Maze

I solved with a slightly modified dijkstra that included the change in direction as an added cost. The extra 1000 turning weight really makes the algorithm want to exhaust all the straight paths. It is pretty entertaining to watch.

### Day 17: Chronospatial Computer

Quite a fun 3-bit cpu. Part2 was challenging to handle all the corner cases. Brute forcing was impossible so I had to iteratively reconstruct the correct A using the associative XOR property ( (A^B)^C =  A^(B^C) ) with each element of the input program. The tricky part is how the correct 3bit value for C plays into how much shifting occurs. I started to wonder if I over complicated the solution. Ultimately, the runtime is 0ms and just ~100 lines so I guess I'm fine with the solution.

### Day 18: RAM Run

For Part1, I re-used almost exactly the dijkstra algorithm from Day 16. For Part2, I only refreshed the shortest path when a new corrupted byte landed on the path, which greatly reduced unnecessary dikjstra calls. 

### Day 19: Linen Layout

I converted the input into a graph and then dfs to figure out for each terminal point which combinations are allowed. I then employed some dynamic programming to keep a history of previously explored branches and their results to avoid loops and re-tracing. The permutations in Part2 require back propagation of previously computed paths.

### Day 20: Race Condition

I implemented a single pass approach that for a given position i, it saves the future cheat locations and the current distance from S->i. When fairly reaching that cheat location, the algorithm calculates the distance saved. A little care is taken to consider that different positions may cheat into the same future square. Part2 increases the time allowed to phase/ghost through walls, greatly increasing the cheat potential and computational time.

### Day 21: Keypad Conundrum

Generating the permutations efficiently was the challenge to making Part1 efficient. Although I will have to rethink my approach since it doesn't scale with 25 robots.