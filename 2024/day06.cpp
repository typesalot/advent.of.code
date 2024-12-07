#include "common.h"

vector<string> readInput() {
  vector<string> input;
  auto           f = ifstream(getInputFile(2024, 6));
  string         s;
  while (getline(f, s))
    input.emplace_back(s);
  return input;
}

enum colors_t {
  color_unvisited   = 37,  // white
  color_obstruction = 93,  // bright yellow
  color_guard       = 94,  // blue
  color_visited     = 34,  // bright blue
};

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
    int y = 0;
    int x = 0;
};

struct segment {
    segment() = default;

    segment(coord _s, coord _e) : start(_s), end(_e) {
    }

    coord start;
    coord end;
};

int orient(const coord& a, const coord& b, const coord& c) {
  return ((b.y - a.y) * (c.x - b.x)) - ((b.x - a.x) * (c.y - b.y));
}

bool intersect(segment& s1, segment& s2) {
  bool result = false;

  std::array<int, 4> o;

  auto check = [&](int i, coord& p1, coord& p2, coord& p3) -> bool {
    o[i] = orient(p1, p2, p3);
    if (o[i] == 0)
      return (min(p1.x, p2.x) <= p3.x && p3.x <= max(p1.x, p2.x)) &&
             (min(p1.y, p2.y) <= p3.y && p3.y <= max(p1.y, p2.y));
    return false;
  };
  result = check(0, s1.start, s1.end, s2.start);
  if (result)
    return true;

  result = check(1, s1.start, s1.end, s2.end);
  if (result)
    return true;
  else if (o[0] * o[1] >= 0) {
    return false;
  }

  result = check(2, s2.start, s2.end, s1.start);
  if (result)
    return true;

  result = check(3, s2.start, s2.end, s1.end);
  if (result)
    return true;
  else if (o[2] * o[3] >= 0) {
    return false;
  }

  return true;
}

struct guard_t {
    coord     pos;
    direction d;

    // next direction if rotate 90deg cw
    direction next() {
      const array<direction, 4> lookup = {EAST, SOUTH, WEST, NORTH};
      return lookup[d];
    }

    // direction facing opposite/mirror
    direction mirror() {
      const array<direction, 4> lookup = {SOUTH, WEST, NORTH, EAST};
      return lookup[d];
    }

    void rotate() {
      d = next();
    }
} guard;

tuple<int, int> calcDistinctPositions(vector<string>& field, bool debug) {
  int changes = 0;
  int cycles  = 0;

  vector<vector<int>> colors;
  vector<coord>       planted;

  unordered_map<int, vector<int>> obs_x;    // for a given x, all the obstructions along y, verticles
  unordered_map<int, vector<int>> obs_y;    // for a given y, all the obstructions along x, horizontals
  array<vector<segment>, 4>       history;  // decision history for each turn

  bool on_field = true;

  // fill in obstruction lookup
  for (int y = 0; y < field.size(); y++) {
    auto& color = colors.emplace_back();
    for (int x = 0; x < field[0].size(); x++) {
      if (field[y][x] == '#') {
        obs_x[y].push_back(x);
        obs_y[x].push_back(y);
        color.emplace_back(color_obstruction);
      } else if (field[y][x] == '^') {
        guard.pos = {x, y};
        guard.d   = NORTH;
        color.emplace_back(color_guard);
      } else {
        color.emplace_back(color_unvisited);
      }
    }
  }

  auto print_field = [&field, &colors, &debug]() -> void {
    if (!debug)
      return;
    cout << endl << "  0123456789" << endl;
    for (int j = 0; j < field.size(); j++) {
      cout << "  ";
      for (int i = 0; i < field[0].length(); i++) {
        cout << "\033[";
        if (guard.pos.y == j && guard.pos.x == i) {
          cout << color_guard << ";" << 47;
          cout << "m" << direction_strings[guard.d][0] << "\033[0m";
        } else {
          cout << colors[j][i];
          cout << "m" << field[j][i] << "\033[0m";
        }
      }
      cout << " " << j << endl;
    }
  };

  auto update_distinct_path = [&field, &colors](const coord& p1, const coord& p2) -> int {
    int changes = 0;

    coord field_max = {(int)field[0].length() - 1, (int)field.size() - 1};
    coord start     = {max(min(p1.x, p2.x), 0), max(min(p1.y, p2.y), 0)};
    coord end       = {min(max(p1.x, p2.x), field_max.x), min(max(p1.y, p2.y), field_max.y)};

    for (int y = start.y; y <= end.y; y++)
      for (int x = start.x; x <= end.x; x++) {
        char& c     = field[y][x];
        int&  color = colors[y][x];
        if (c == '.' || c == '^') {
          c     = 'X';
          color = color_visited;
          changes++;
        }
      }

    return changes;
  };

  auto find_obstruction = [&](coord& pos, direction d) -> tuple<coord, bool> {
    coord result           = pos;
    bool  found            = false;
    bool  is_verticle      = (d == NORTH || d == SOUTH);
    auto& obstructions_map = (is_verticle) ? obs_y : obs_x;
    auto& obs              = (is_verticle) ? obstructions_map[pos.x] : obstructions_map[pos.y];

    switch (d) {
    case NORTH: {
      auto res = find_if(obs.rbegin(), obs.rend(), [gy = pos.y](int fy) -> bool { return fy < gy; });
      found    = res != obs.rend();
      result.y = (found) ? *res : -1;
    } break;
    case EAST: {
      auto res = find_if(obs.begin(), obs.end(), [gx = pos.x](int fx) -> bool { return fx > gx; });
      found    = res != obs.end();
      result.x = (found) ? *res : field[0].length();
    } break;
    case SOUTH: {
      auto res = find_if(obs.begin(), obs.end(), [gy = pos.y](int fy) -> bool { return fy > gy; });
      found    = res != obs.end();
      result.y = (found) ? *res : field.size();
    } break;
    case WEST: {
      auto res = find_if(obs.rbegin(), obs.rend(), [gx = pos.x](int fx) -> bool { return fx < gx; });
      found    = res != obs.rend();
      result.x = (found) ? *res : -1;
    } break;
    }
    return {result, found};
  };

  while (on_field) {
    coord   new_pos = guard.pos;
    coord   obstacle;
    coord   obstacle_mirror;
    segment path;

    // north
    if (guard.d == NORTH) {
      tie(obstacle, on_field) = find_obstruction(guard.pos, guard.d);
      changes += update_distinct_path(guard.pos, coord{guard.pos.x, obstacle.y + 1});
      new_pos.y = obstacle.y + 1;
    }
    // east
    else if (guard.d == EAST) {
      tie(obstacle, on_field) = find_obstruction(guard.pos, guard.d);
      changes += update_distinct_path(guard.pos, coord{obstacle.x - 1, guard.pos.y});
      new_pos.x = obstacle.x - 1;
    }
    // south
    else if (guard.d == SOUTH) {
      tie(obstacle, on_field) = find_obstruction(guard.pos, guard.d);
      changes += update_distinct_path(guard.pos, coord{guard.pos.x, obstacle.y - 1});
      new_pos.y = obstacle.y - 1;
    }
    // west
    else if (guard.d == WEST) {
      tie(obstacle, on_field) = find_obstruction(guard.pos, guard.d);
      changes += update_distinct_path(guard.pos, coord{obstacle.x + 1, guard.pos.y});
      new_pos.x = obstacle.x + 1;
    }

    path           = {guard.pos, new_pos};
    auto& segments = history[guard.next()];
    for (auto& segment : segments) {
      if (intersect(path, segment)) {
        cycles++;
      }
    }

    // save guard history and then rotate
    bool mirror_inbounds           = true;
    tie(obstacle, mirror_inbounds) = find_obstruction(guard.pos, guard.mirror());
    obstacle.x                     = min<int>(max(obstacle.x, 0), field[0].length() - 1);
    obstacle.y                     = min<int>(max(obstacle.y, 0), field.size() - 1);
    path                           = {obstacle, new_pos};
    history[guard.d].emplace_back(path);
    guard.pos = new_pos;
    guard.rotate();

    print_field();
  }

  return {changes, cycles};
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

  auto [changes, cycles] = calcDistinctPositions(field, true);

  EXPECT_EQ(changes, 41);
  EXPECT_EQ(cycles, 6);
  cout << "Answer (changes) = " << changes << endl;
  cout << "Answer (cycles)  = " << cycles << endl;
}

TEST(Day6, Part1) {
  vector<string> field = readInput();

  auto [changes, cycles] = calcDistinctPositions(field, false);
  EXPECT_EQ(changes, 4789);
  EXPECT_EQ(changes, 374);  // too low
  cout << "Answer (changes) = " << changes << endl;
  cout << "Answer (cycles)  = " << cycles << endl;
}