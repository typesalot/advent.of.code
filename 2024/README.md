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
| 12  | :black_circle: | :black_circle:  |
| 13  | :black_circle: | :black_circle:  |
| 14  | :black_circle: | :black_circle:  |
| 15  | :black_circle: | :black_circle:  |
| 16  | :black_circle: | :black_circle:  |
| 17  | :black_circle: | :black_circle:  |
| 18  | :black_circle: | :black_circle:  |
| 19  | :black_circle: | :black_circle:  |
| 20  | :black_circle: | :black_circle:  |
| 21  | :black_circle: | :black_circle:  |
| 22  | :black_circle: | :black_circle:  |
| 23  | :black_circle: | :black_circle:  |
| 24  | :black_circle: | :black_circle:  |
| 25  | :black_circle: | :black_circle:  |

## Journal

### Day 10: Hoof It

Path finding both to unique destinations and unique paths. I misunderstood the problem initially and solved Part2 as Part1. Since the max depth is only 9, I used recursion to navigate the map. I also took a dynamic programming approach to keep a history of traveled score/rating per location to avoid re-navigating previous paths.

### Day 11: Plutonian Pebbles

A lot of common subcases that can be cached to avoid re-calculation, with the added complexity that the cache needs to be blink-level aware. I used recursion as well since Part2's 75 blinks means a manageable function depth.