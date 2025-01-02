#include "test.h"
#include "util/point.h"
#include "util/term.h"
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
    bool                              generateSavings = true;

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

    bool canCheat(const point& cheat_dir, const vector<bool>& visited) {
      // don't cheat out of bounds
      if (!bounds.in_bounds(cheat_dir))
        return false;

      // don't cheat into a wall
      if (map[cheat_dir.y][cheat_dir.x] == '#')
        return false;

      // don't cheat into a previous location (forward progress only)
      if (visited[cheat_dir.flatten(w)])
        return false;

      return true;
    }

    void calcSavings() {
      vector<bool>        visited;
      vector<vector<int>> cheats;
      array<point, 4>     directions = {
          point{0, -1},  // north
          point{1, 0},   // east
          point{0, 1},   // south
          point{-1, 0}   // west
      };

      visited.resize(w * h, false);
      cheats.resize(w * h);

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
          uint32_t cheat_cost = 2;  // walking through a wall takes 2 picoseconds
          uint32_t saving     = cur_dist - cheat_dist - cheat_cost;

          savings[saving]++;
        }

        // exit if we've reached the end
        if (cur_pos == end)
          break;

        point next;

        // enumerate future cheat positions
        for (const auto& dir : directions) {
          next = cur_pos + dir;

          if (map[next.y][next.x] == '#') {
            point cheat_pos = next + dir;

            if (canCheat(cheat_pos, visited)) {
              cheats[cheat_pos.flatten(w)].push_back(cur_dist);

              if (debug()) {
                map[cheat_pos.y].background(cheat_pos.x).blue();
                print_map();
              }
            }
          }
        }

        // non-cheat move
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
      if (generateSavings) {
        calcSavings();
        generateSavings = false;
      }

      uint32_t count = 0;
      for (const auto& [k, v] : savings) {
        if (pred(k, s))
          count += v;
      }

      return count;
    }
};

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
  auto     ge     = [](int x, int y) -> bool { return x >= y; };
  uint32_t answer = getCheatCount(100, ge);
  EXPECT_EQ(answer, 1452);
}