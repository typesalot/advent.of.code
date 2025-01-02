#include "test.h"
#include "util/point.h"
#include "util/term.h"

using namespace std;

class Day20 : public aoc_2024 {
  protected:
    using point = point_int;

    vector<term::string>              map;
    int32_t                           w;
    int32_t                           h;
    point                             start;
    point                             end;
    vector<point>                     path;
    uint32_t                          baseline;
    unordered_map<uint32_t, uint32_t> savings;
    array<point, 4>                   directions;
    rect_int                          bounds;
    vector<bool>                      visited;

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

      directions = {
          point{0, -1},  // north
          point{1, 0},   // east
          point{0, 1},   // south
          point{-1, 0}   // west
      };

      visited.resize(w * h, false);
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

    bool canCheat(const point& cheat_dir) {
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

    uint32_t distance() {
      uint32_t d = 0;

      rect_int bounds = {point{0, 0}, point{w - 1, h - 1}};

      vector<int> cheats;
      cheats.resize(w * h, numeric_limits<int>::min());

      point p = start;
      while (true) {
        // update visited and print map
        visited[p.flatten(w)] = true;
        if (debug()) {
          map[p.y].background(p.x).red();
          print_map();
        }

        // handle if we're currently on a cheated position
        uint32_t cheat_distance = cheats[p.flatten(w)];
        if (cheat_distance != numeric_limits<int>::min()) {
          uint32_t s = d - cheat_distance;
          savings[s]++;
        }

        if (p == end)
          break;

        point next;

        // enumerate cheats
        for (const auto& dir : directions) {
          next = p + dir;

          if (map[next.y][next.x] == '#') {
            point c = next + dir;

            if (canCheat(c)) {
              cheats[c.flatten(w)] = d + 1;

              if (debug()) {
                map[c.y].background(c.x).blue();
                print_map();
              }
            }
          }
        }

        // non-cheat move
        for (const auto& dir : directions) {
          next = p + dir;
          if (!bounds.in_bounds(next) || visited[next.flatten(w)] || map[next.y][next.x] == '#')
            continue;
          break;
        }
        p = next;
        d++;
      }

      return d;
    }

    uint32_t getCheatCount(uint32_t picoSavings) {
      distance();

      return savings[picoSavings];
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

  EXPECT_EQ(getCheatCount(2), 14);
  EXPECT_EQ(getCheatCount(4), 14);
  EXPECT_EQ(getCheatCount(6), 2);
  EXPECT_EQ(getCheatCount(8), 4);
  EXPECT_EQ(getCheatCount(10), 2);
  EXPECT_EQ(getCheatCount(12), 3);
  EXPECT_EQ(getCheatCount(20), 1);
  EXPECT_EQ(getCheatCount(36), 1);
  EXPECT_EQ(getCheatCount(38), 1);
  EXPECT_EQ(getCheatCount(40), 1);
  EXPECT_EQ(getCheatCount(64), 1);
}

TEST_F(Day20, Part1) {
  uint32_t answer = getCheatCount(100);
  EXPECT_EQ(answer, 14);
}