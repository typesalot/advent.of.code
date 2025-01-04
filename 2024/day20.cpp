#include "test.h"
#include "util/point.h"
#include "util/term.h"
#include <cstdlib>
#include <functional>

using namespace std;

class Day20 : public aoc_2024 {
  protected:
    using point = point_int;

    vector<term::string>              map;
    int32_t                           w;
    int32_t                           h;
    point                             start;
    point                             end;
    unordered_map<uint32_t, uint32_t> savings;
    rect_int                          bounds;
    vector<bool>                      visited;
    vector<vector<int>>               cheats;
    array<point, 4>                   directions;

    void LoadInput(istringstream& input) override {
      string s;
      while (getline(input, s)) {
        auto s_loc = s.find('S');
        auto e_loc = s.find('E');
        if (s_loc != -1)
          start = {(int)s_loc, (int)map.size()};
        if (e_loc != -1)
          end = {(int)e_loc, (int)map.size()};

        map.push_back(s);
      }

      h      = map.size();
      w      = map[0].length();
      bounds = {point{0, 0}, point{w - 1, h - 1}};

      visited.resize(w * h, false);
      cheats.resize(w * h);

      directions = {
          point{0, -1},  // north
          point{1, 0},   // east
          point{0, 1},   // south
          point{-1, 0}   // west
      };
    }

    void print_map() {
      if (!g_config.debug)
        return;
      static bool move = false;
      if (move)
        cout << term::cursor::move_up(h);
      move = true;
      for (const auto& m : map)
        cout << m << endl;
    }

    void reset_map() {
      if (!g_config.debug)
        return;
      for (auto& m : map)
        m.reset();
    }

    bool in_bounds(const point& p) const {
      return bounds.in_bounds(p);
    }

    void checkCheats() {
      for (int j = 0; j < h; j++) {
        for (int i = 0; i < w; i++) {
          int v = j * w + i;
          if (isnumber(map[j][i]))
            EXPECT_EQ(cheats[v].size(), map[j][i] - '0');
        }
      }
    }

    void enumerateCheats(const point& cur, uint32_t cur_dist, uint32_t maxTime) {
      set<point>   enum_visited;
      deque<point> next;
      next.push_back(cur);

      point    last;
      uint32_t d = 0;

      while (!next.empty() && d < maxTime) {
        uint32_t size = next.size();
        point    p    = next.front();

        while (size) {
          enum_visited.emplace(p);

          if (debug()) {
            map[p.y].background(p.x).green();
            print_map();
          }

          for (const auto& dir : directions) {
            point walk      = p + dir;
            uint  walk_flat = walk.flatten(w);

            // don't cheat out of bounds
            if (!in_bounds(walk))
              continue;

            char walk_char = map[walk.y][walk.x];

            // don't cheat if not ghosting
            if (d == 0 && (walk_char == '.' || walk_char == 'E' || isnumber(walk_char)))
              continue;

            // don't cheat into a wall, but save it for navigation
            if (walk_char == '#') {
              if (d < maxTime && !enum_visited.contains(walk))
                next.emplace_back(walk);
              continue;
            }

            // don't cheat into the past
            if (visited[walk_flat])
              continue;

            // don't visit the same cheat twice
            if (enum_visited.contains(walk))
              continue;

            cheats[walk.flatten(w)].push_back(cur_dist + d + 1);
            enum_visited.emplace(walk);

            if (debug()) {
              map[walk.y].background(walk.x).blue();
              print_map();
            }
          }

          // get next #, skipping duplicates
          last = p;
          do {
            next.pop_front();
            p = next.front();
            size--;
          } while (p == last);
        }

        d++;
      }
    }

    void calcSavings(uint32_t maxTime) {
      point    cur_pos  = start;
      uint32_t cur_dist = 0;

      while (true) {
        uint32_t cur_flat = cur_pos.flatten(w);

        // update visited and print map
        visited[cur_flat] = true;
        if (debug()) {
          map[cur_pos.y].background(cur_pos.x).red();
          print_map();
        }

        // handle if we're currently on a cheated position
        auto& cheat_distances = cheats[cur_flat];
        for (const auto& cheat_dist : cheat_distances) {
          int32_t saving = cur_dist - cheat_dist;
          if (saving >= 0)
            savings[saving]++;
        }

        // exit if we've reached the end
        if (cur_pos == end)
          break;

        // enumerate future cheat positions
        decltype(map) tmp;
        if (debug())
          tmp = map;
        enumerateCheats(cur_pos, cur_dist, maxTime);
        if (debug()) {
          map = tmp;
          print_map();
        }

        // non-cheat move
        point next;
        for (const auto& dir : directions) {
          next = cur_pos + dir;
          if (!bounds.in_bounds(next) || map[next.y][next.x] == '#' || visited[next.flatten(w)])
            continue;
          break;
        }

        // move and increase distance
        cur_pos = next;
        cur_dist++;
      }
    }

    uint32_t getCheatCount(uint32_t s, const function<bool(int, int)>& pred) {
      uint32_t count = 0;
      for (const auto& [k, v] : savings) {
        if (pred(k, s))
          count += v;
      }

      return count;
    }
};

TEST_F(Day20, enumCaseA) {
  input = "...............\n"
          "......0.0......\n"
          ".....0#0#0.....\n"
          ".....1#S#1.....\n"
          ".....0###0.....\n"
          "......010......\n"
          "..............E\n";
  SetUp();

  visited[start.flatten(w)] = true;

  enumerateCheats(start, 0, 2);
  checkCheats();
}

TEST_F(Day20, enumCaseB) {
  input = "...............\n"
          "......1.1......\n"
          ".....1#1#1.....\n"
          ".....1#S#1.....\n"
          ".....1###1.....\n"
          "......111......\n"
          "..............E\n";
  SetUp();

  visited[start.flatten(w)] = true;

  enumerateCheats(start, 0, 3);
  checkCheats();
}

TEST_F(Day20, enumCaseC) {
  input = "...............\n"
          "...###.#.......\n"
          "...S...#.......\n"
          "...#####.......\n"
          "...............\n"
          "...............\n"
          "..............E\n";
  SetUp();

  visited[start.flatten(w)] = true;

  enumerateCheats(start, 0, 8);
}

TEST_F(Day20, Part1Example) {
  input = "###############\n"
          "#...#...#.....#\n"
          "#.#.#.#.#.###.#\n"
          "#S#...#.#.#...#\n"
          "#######.#.#.###\n"
          "#######.#.#...#\n"
          "#######.#.###.#\n"
          "###..E#...#...#\n"
          "###.#######.###\n"
          "#...###...#...#\n"
          "#.#####.#.###.#\n"
          "#.#...#.#.#...#\n"
          "#.#.#.#.#.#.###\n"
          "#...#...#...###\n"
          "###############";
  SetUp();

  auto eq = [](int x, int y) -> bool { return x == y; };

  calcSavings(2);

  EXPECT_EQ(getCheatCount(2, eq), 14);
  EXPECT_EQ(getCheatCount(4, eq), 14);
  EXPECT_EQ(getCheatCount(6, eq), 2);
  EXPECT_EQ(getCheatCount(8, eq), 4);
  EXPECT_EQ(getCheatCount(10, eq), 2);
  EXPECT_EQ(getCheatCount(12, eq), 3);
  EXPECT_EQ(getCheatCount(20, eq), 1);
  EXPECT_EQ(getCheatCount(36, eq), 1);
  EXPECT_EQ(getCheatCount(38, eq), 1);
  EXPECT_EQ(getCheatCount(40, eq), 1);
  EXPECT_EQ(getCheatCount(64, eq), 1);
}

TEST_F(Day20, Part1) {
  calcSavings(2);

  auto     ge     = [](int x, int y) -> bool { return x >= y; };
  uint32_t answer = getCheatCount(100, ge);
  EXPECT_EQ(answer, 1452);
}

TEST_F(Day20, Part2Example) {
  input = "###############\n"
          "#...#...#.....#\n"
          "#.#.#.#.#.###.#\n"
          "#S#...#.#.#...#\n"
          "#######.#.#.###\n"
          "#######.#.#...#\n"
          "#######.#.###.#\n"
          "###..E#...#...#\n"
          "###.#######.###\n"
          "#...###...#...#\n"
          "#.#####.#.###.#\n"
          "#.#...#.#.#...#\n"
          "#.#.#.#.#.#.###\n"
          "#...#...#...###\n"
          "###############";
  SetUp();

  auto eq = [](int x, int y) -> bool { return x == y; };

  calcSavings(20);

  EXPECT_EQ(getCheatCount(50, eq), 32);
  EXPECT_EQ(getCheatCount(52, eq), 31);
  EXPECT_EQ(getCheatCount(54, eq), 29);
  EXPECT_EQ(getCheatCount(56, eq), 39);
  EXPECT_EQ(getCheatCount(58, eq), 25);
  EXPECT_EQ(getCheatCount(60, eq), 23);
  EXPECT_EQ(getCheatCount(62, eq), 20);
  EXPECT_EQ(getCheatCount(64, eq), 19);
  EXPECT_EQ(getCheatCount(66, eq), 12);
  EXPECT_EQ(getCheatCount(68, eq), 14);
  EXPECT_EQ(getCheatCount(70, eq), 12);
  EXPECT_EQ(getCheatCount(72, eq), 22);
  EXPECT_EQ(getCheatCount(74, eq), 4);
  EXPECT_EQ(getCheatCount(76, eq), 3);
}