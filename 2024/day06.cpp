#include "common.h"

vector<string> readInput() {
  vector<string> input;
  auto           f = ifstream(getInputFile(2024, 6));
  string         s;
  while (getline(f, s))
    input.emplace_back(s);
  return input;
}

int calcDistinctPositions(vector<string>& field, bool debug) {
  int changes = 0;

  enum direction {
    NORTH,
    EAST,
    SOUTH,
    WEST
  };

  unordered_map<direction, string> direction_strings = {
      {NORTH, "NORTH"},
      {EAST, "EAST"},
      {SOUTH, "SOUTH"},
      {WEST, "WEST"},
  };

  struct coord {
      coord() = default;
      coord(int _x, int _y) : x(_x), y(_y) {};
      int x = 0;
      int y = 0;
  };

  struct {
      coord     pos;
      direction d;
      bool      on_field;

      void rotate() {
        switch (d) {
        case NORTH:
          d = EAST;
          break;
        case EAST:
          d = SOUTH;
          break;
        case SOUTH:
          d = WEST;
          break;
        case WEST:
          d = NORTH;
          break;
        }
      }
  } guard;

  unordered_map<int, vector<int>> obs_x;  // for a given x, all the onstructions along y, verticles
  unordered_map<int, vector<int>> obs_y;  // for a given y, all the obstructions along x, horizontals

  // fill in obstruction lookup
  timer_f("fill obstruction maps", [&]() -> void {
    for (int y = 0; y < field.size(); y++)
      for (int x = 0; x < field[0].size(); x++)
        if (field[y][x] == '#') {
          obs_x[y].push_back(x);
          obs_y[x].push_back(y);
        } else if (field[y][x] == '^') {
          guard.pos      = {x, y};
          guard.d        = NORTH;
          guard.on_field = true;
        }
  });

  auto update_distinct_path = [&field](const coord& p1, const coord& p2) -> int {
    int changes = 0;

    coord field_max = {(int)field[0].length() - 1, (int)field.size() - 1};
    coord start     = {max(min(p1.x, p2.x), 0), max(min(p1.y, p2.y), 0)};
    coord end       = {min(max(p1.x, p2.x), field_max.x), min(max(p1.y, p2.y), field_max.y)};

    for (int y = start.y; y <= end.y; y++)
      for (int x = start.x; x <= end.x; x++) {
        char& c = field[y][x];
        if (c == '.' || c == '^') {
          c = 'X';
          changes++;
        }
      }

    return changes;
  };

  while (guard.on_field) {
    // north
    if (guard.d == NORTH) {
      auto& obs = obs_y[guard.pos.x];
      auto  res = find_if(obs.rbegin(), obs.rend(), [gy = guard.pos.y](int fy) -> bool { return fy < gy; });
      int   y   = -1;
      if (res == obs.rend())
        guard.on_field = false;
      else
        y = *res;
      changes += update_distinct_path(guard.pos, coord{guard.pos.x, y});
      guard.pos.y = y + 1;
    }
    // east
    else if (guard.d == EAST) {
      auto& obs = obs_x[guard.pos.y];
      auto  res = find_if(obs.begin(), obs.end(), [gx = guard.pos.x](int fx) -> bool { return fx > gx; });
      int   x   = field[0].length();
      if (res == obs.end())
        guard.on_field = false;
      else
        x = *res;
      changes += update_distinct_path(guard.pos, coord{x, guard.pos.y});
      guard.pos.x = x - 1;
    }
    // south
    else if (guard.d == SOUTH) {
      auto& obs = obs_y[guard.pos.x];
      auto  res = find_if(obs.begin(), obs.end(), [gy = guard.pos.y](int fy) -> bool { return fy > gy; });
      int   y   = field.size();
      if (res == obs.end())
        guard.on_field = false;
      else
        y = *res;
      changes += update_distinct_path(guard.pos, coord{guard.pos.x, y});
      guard.pos.y = y - 1;
    }
    // west
    else if (guard.d == WEST) {
      auto& obs = obs_x[guard.pos.y];
      auto  res = find_if(obs.rbegin(), obs.rend(), [gx = guard.pos.x](int fx) -> bool { return fx < gx; });
      int   x   = -1;
      if (res == obs.rend())
        guard.on_field = false;
      else
        x = *res;
      changes += update_distinct_path(guard.pos, coord{x, guard.pos.y});
      guard.pos.x = x + 1;
    }
    guard.rotate();

    if (debug) {
      for (auto& s : field)
        cout << format("  {}", s) << endl;
      cout << format("  Guard @ y,x={},{} facing {}", guard.pos.y, guard.pos.x, direction_strings[guard.d]) << endl;
      cout << endl;
    }
  }

  return changes;
}

TEST(Day6, Part1Example) {
  return;
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

  int ans = calcDistinctPositions(field, true);

  EXPECT_EQ(ans, 41);
  cout << "Answer = " << ans << endl;
}

TEST(Day6, Part1) {
  vector<string> field = readInput();

  int ans = calcDistinctPositions(field, false);
  EXPECT_EQ(ans, 4789);
  cout << "Answer = " << ans << endl;
}