#include "common.h"

int calcDistinctPositions(vector<string>& field) {
  int pos = 0;

  unordered_map<int, vector<int>> obs_x;
  unordered_map<int, vector<int>> obs_y;

  // fill in obstruction lookup
  timer_f("fill obstruction maps", [&]() -> void {
    for (int j = 0; j < field.size(); j++)
      for (int i = 0; i < field[0].size(); i++)
        if (field[j][i] == '#') {
          obs_x[i].push_back(j);
          obs_y[j].push_back(i);
        }
  });

  return pos;
}

TEST(Day6, Part1Example) {
  vector<string> field = {
      // clang-format off
     //0123456789
      "....#.....", // 0
      ".........#", // 1
      "..........", // 2
      "..#.......", // 3
      ".......#..", // 4
      "..........", // 5
      ".#..^.....", // 6
      "........#.", // 7
      "#.........", // 8
      "......#...", // 9
      // clang-format on
  };

  int ans = calcDistinctPositions(field);
  EXPECT_EQ(ans, 41);
  cout << "Answer = " << ans << endl;
}