#include "common.h"
#include "util/point.h"
#include <algorithm>

vector<string> readInput() {
  vector<string> input;
  auto           f = ifstream(getInputFile(2024, 6));
  string         s;
  while (getline(f, s))
    input.emplace_back(s);
  return input;
}

using point_t = point_int;

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

struct segment {
    segment() = default;

    segment(point_t _s, point_t _e) : start(_s), end(_e) {
    }

    point_t start;
    point_t end;
};

std::optional<point_t> intersection(point_t& p1, point_t& p2, point_t& p3, point_t& p4) {
  int A = p2.x - p1.x;
  int B = p4.x - p3.x;
  int C = p2.y - p1.y;
  int D = p4.y - p3.y;

  int denominator = A * D - B * C;
  if (denominator == 0) {
    // Parallel or collinear
    return std::nullopt;
  }

  int numeratorT = (p3.x - p1.x) * D - (p3.y - p1.y) * B;
  int numeratorU = (p3.x - p1.x) * C - (p3.y - p1.y) * A;

  // Check if 0 <= t <= 1 and 0 <= u <= 1 using integer arithmetic.
  // If denominator > 0:
  //   0 <= numeratorT <= denominator and 0 <= numeratorU <= denominator
  // If denominator < 0:
  //   denominator <= numeratorT <= 0 and denominator <= numeratorU <= 0

  bool intersects = false;
  if (denominator > 0) {
    if (numeratorT >= 0 && numeratorT <= denominator && numeratorU >= 0 && numeratorU <= denominator) {
      intersects = true;
    }
  } else {  // denominator < 0
    if (numeratorT <= 0 && numeratorT >= denominator && numeratorU <= 0 && numeratorU >= denominator) {
      intersects = true;
    }
  }

  if (!intersects) {
    return std::nullopt;
  }

  // Intersection exists. Compute intersection coordinates.
  // Here we use double just for the final calculation.
  double  t = static_cast<double>(numeratorT) / static_cast<double>(denominator);
  point_t result;
  result.x = p1.x + t * A;
  result.y = p1.y + t * C;

  return result;
}

std::optional<point_t> intersection(segment& s1, segment& s2) {
  return intersection(s1.start, s1.end, s2.start, s2.end);
}

struct guard_t {
    point_t   pos;
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

    bool operator==(const guard_t& rhs) const {
      return pos == rhs.pos && d == rhs.d;
    }
} guard;

tuple<int, int, bool> calcDistinctPositions(vector<string>& field, bool detect_loop) {
  int  changes = 0;
  int  cycles  = 0;
  bool in_loop = false;

  vector<vector<int>> colors;
  vector<point_t>     planted;

  unordered_map<int, vector<int>> obs_x;    // for a given x, all the obstructions along y, verticles
  unordered_map<int, vector<int>> obs_y;    // for a given y, all the obstructions along x, horizontals
  array<vector<segment>, 4>       history;  // decision history for each turn
  vector<guard_t>                 breadcrumbs;

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

  auto print_field = [&field, &colors, &planted]() -> void {
    cout << endl << "  0123456789" << endl;
    for (int j = 0; j < field.size(); j++) {
      cout << "  ";
      for (int i = 0; i < field[0].length(); i++) {
        cout << "\033[";
        int  fg = colors[j][i];
        int  bg = 0;
        char c  = field[j][i];

        auto p = find(planted.cbegin(), planted.cend(), point_t{j, i});
        if (p != planted.end()) {
          c  = '?';
          bg = 43;
        } else if (guard.pos.y == j && guard.pos.x == i) {
          fg = color_guard;
          c  = direction_strings[guard.d][0];
          bg = 47;
        }
        cout << fg << ";" << bg << "m" << c << "\033[0m";
      }
      cout << " " << j << endl;
    }
  };

  auto update_distinct_path = [&field, &colors](const point_t& p1, const point_t& p2) -> int {
    int changes = 0;

    point_t field_max = {(int)field[0].length() - 1, (int)field.size() - 1};
    point_t start     = {max(min(p1.x, p2.x), 0), max(min(p1.y, p2.y), 0)};
    point_t end       = {min(max(p1.x, p2.x), field_max.x), min(max(p1.y, p2.y), field_max.y)};

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

  auto find_obstruction = [&](point_t& pos, direction d) -> tuple<point_t, bool> {
    point_t result           = pos;
    bool    found            = false;
    bool    is_verticle      = (d == NORTH || d == SOUTH);
    auto&   obstructions_map = (is_verticle) ? obs_y : obs_x;
    auto&   obs              = (is_verticle) ? obstructions_map[pos.x] : obstructions_map[pos.y];

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

  while (on_field && !in_loop) {
    if (detect_loop) {
      if (find(breadcrumbs.begin(), breadcrumbs.end(), guard) != breadcrumbs.end()) {
        in_loop = true;
        continue;
      }
      breadcrumbs.push_back(guard);
    }

    point_t new_pos = guard.pos;
    point_t obstacle;
    segment path;

    // north
    if (guard.d == NORTH) {
      tie(obstacle, on_field) = find_obstruction(guard.pos, guard.d);
      changes += update_distinct_path(guard.pos, point_t{guard.pos.x, obstacle.y + 1});
      new_pos.y = obstacle.y + 1;
    }
    // east
    else if (guard.d == EAST) {
      tie(obstacle, on_field) = find_obstruction(guard.pos, guard.d);
      changes += update_distinct_path(guard.pos, point_t{obstacle.x - 1, guard.pos.y});
      new_pos.x = obstacle.x - 1;
    }
    // south
    else if (guard.d == SOUTH) {
      tie(obstacle, on_field) = find_obstruction(guard.pos, guard.d);
      changes += update_distinct_path(guard.pos, point_t{guard.pos.x, obstacle.y - 1});
      new_pos.y = obstacle.y - 1;
    }
    // west
    else if (guard.d == WEST) {
      tie(obstacle, on_field) = find_obstruction(guard.pos, guard.d);
      changes += update_distinct_path(guard.pos, point_t{obstacle.x + 1, guard.pos.y});
      new_pos.x = obstacle.x + 1;
    }

    path           = {guard.pos, new_pos};
    auto& segments = history[guard.next()];
    for (auto& segment : segments) {
      auto itr = intersection(path, segment);
      if (itr)
        planted.emplace_back(point_t{itr->y, itr->x});
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

    if (g_config.debug)
      print_field();
  }

  return {changes, planted.size(), in_loop};
}

int brute_force_part2(const vector<string>& field) {
  int brute_force_cycles = 0;
  for (int j = 0; j < field.size(); j++) {
    for (int i = 0; i < field[0].length(); i++) {
      if (field[j][i] == '.') {
        vector<string> modified = field;
        modified[j][i]          = '#';
        auto [_, __, in_loop]   = calcDistinctPositions(modified, true);
        if (in_loop)
          brute_force_cycles++;
      }
    }
  }
  return brute_force_cycles;
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

  auto [changes, cycles, in_loop] = calcDistinctPositions(field, false);

  if (in_loop)
    cout << "Loop detected!" << endl;
  else {
    EXPECT_EQ(changes, 41);
    EXPECT_EQ(cycles, 6);
    cout << "Answer (changes) = " << changes << endl;
    cout << "Answer (cycles)  = " << cycles << endl;
  }
}

TEST(Day6, Part1) {
  vector<string> field = readInput();

  auto [changes, cycles, in_loop] = calcDistinctPositions(field, false);
  EXPECT_EQ(changes, 4789);
  // EXPECT_EQ(cycles, 1304);
  cout << "Answer (changes) = " << changes << endl;
  cout << "Answer (cycles)  = " << cycles << " --> wrong, should be 1304" << endl;
}