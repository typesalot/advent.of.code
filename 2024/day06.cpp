#include "common.h"

vector<string> readInput() {
  vector<string> input;
  auto           f = ifstream(getInputFile(2024, 6));
  string         s;
  while (getline(f, s))
    input.emplace_back(s);
  return input;
}

enum direction {
  NORTH,
  EAST,
  SOUTH,
  WEST
};

struct coord {
    coord() = default;
    coord(int _x, int _y) : x(_x), y(_y) {};
    int x = 0;
    int y = 0;
};

struct segment {
    segment(coord _s, coord _e) : start(_s), end(_e) {
    }

    coord start;
    coord end;
};

struct guard_t {
    coord     pos;
    direction d;

    direction next() {
      const array<direction, 4> lookup = {EAST, SOUTH, WEST, NORTH};
      return lookup[d];
    }

    void rotate() {
      d = next();
    }
} guard;

int calcDistinctPositions(vector<string>& field, bool debug) {
  int changes = 0;

  unordered_map<direction, string> direction_strings = {
      {NORTH, "NORTH"},
      {EAST, "EAST"},
      {SOUTH, "SOUTH"},
      {WEST, "WEST"},
  };

  unordered_map<int, vector<int>> obs_x;    // for a given x, all the onstructions along y, verticles
  unordered_map<int, vector<int>> obs_y;    // for a given y, all the obstructions along x, horizontals
  array<vector<segment>, 4>       history;  // decision history for each turn

  bool on_field = true;

  // fill in obstruction lookup
  for (int y = 0; y < field.size(); y++)
    for (int x = 0; x < field[0].size(); x++)
      if (field[y][x] == '#') {
        obs_x[y].push_back(x);
        obs_y[x].push_back(y);
      } else if (field[y][x] == '^') {
        guard.pos = {x, y};
        guard.d   = NORTH;
      }

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

  while (on_field) {
    coord new_pos = guard.pos;

    // north
    if (guard.d == NORTH) {
      auto& obs = obs_y[guard.pos.x];
      auto  res = find_if(obs.rbegin(), obs.rend(), [gy = guard.pos.y](int fy) -> bool { return fy < gy; });
      int   y   = -1;
      if (res == obs.rend())
        on_field = false;
      else
        y = *res + 1;
      changes += update_distinct_path(guard.pos, coord{guard.pos.x, y});
      new_pos.y = y;
    }
    // east
    else if (guard.d == EAST) {
      auto& obs = obs_x[guard.pos.y];
      auto  res = find_if(obs.begin(), obs.end(), [gx = guard.pos.x](int fx) -> bool { return fx > gx; });
      int   x   = field[0].length();
      if (res == obs.end())
        on_field = false;
      else
        x = *res - 1;
      changes += update_distinct_path(guard.pos, coord{x, guard.pos.y});
      new_pos.x = x;
    }
    // south
    else if (guard.d == SOUTH) {
      auto& obs = obs_y[guard.pos.x];
      auto  res = find_if(obs.begin(), obs.end(), [gy = guard.pos.y](int fy) -> bool { return fy > gy; });
      int   y   = field.size();
      if (res == obs.end())
        on_field = false;
      else
        y = *res - 1;
      changes += update_distinct_path(guard.pos, coord{guard.pos.x, y});
      new_pos.y = y;
    }
    // west
    else if (guard.d == WEST) {
      auto& obs   = obs_x[guard.pos.y];
      auto  res   = find_if(obs.rbegin(), obs.rend(), [gx = guard.pos.x](int fx) -> bool { return fx < gx; });
      int   dst_x = -1;
      if (res == obs.rend())
        on_field = false;
      else {
        dst_x = *res + 1;

        auto& cardinal_history = history[guard.next()];
        for (auto& previous : cardinal_history) {
        }
      }
      changes += update_distinct_path(guard.pos, coord{dst_x, guard.pos.y});
      new_pos.x = dst_x;
    }

    // save guard history and then rotate
    history[guard.d].emplace_back(guard.pos, new_pos);
    guard.pos = new_pos;
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
  vector<string> field = {
      // clang-format off
     //0123456789
      "....#.....", // 0
      ".........#", // 1
      "..........", // 2
      "..#.......", // 3
      ".......#..", // 4
      "..........", // 5
      ".#........", // 6
      "....^...#.", // 7
      "#.........", // 8
      "......#...", // 9
      // clang-format on
  };

  int ans = calcDistinctPositions(field, true);

  EXPECT_EQ(ans, 41);
  cout << "Answer = " << ans << endl;
}

TEST(Day6, Part1) {
  return;
  vector<string> field = readInput();

  int ans = calcDistinctPositions(field, false);
  EXPECT_EQ(ans, 4789);
  cout << "Answer = " << ans << endl;
}