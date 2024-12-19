#include "common.h"

class Day14 : public ::testing::Test {
  protected:
    using point = point_int;

    static constexpr uint32_t top_left     = 0;
    static constexpr uint32_t top_right    = 1;
    static constexpr uint32_t bottom_left  = 2;
    static constexpr uint32_t bottom_right = 3;
    using quadrants                        = tuple<uint32_t, uint32_t, uint32_t, uint32_t>;

    struct robot {
        point pos;
        point vel;
    };

    // part2
    bool           find_tree;
    uint32_t       tree_time = 0;
    vector<string> arena;

    vector<robot> robots;

    void SetUp() override {
      auto fname = getInputFile(2024, 14);
      auto f     = ifstream(fname);
      readInput(f);
    };

    template <typename src_t>
    void readInput(src_t& src) {
      robots.clear();

      auto parse_point = [](const string& s) -> point {
        auto   eq    = s.find('=');
        auto   comma = s.find(',');
        string x     = s.substr(eq + 1, comma - eq - 1);
        string y     = s.substr(comma + 1, s.length() - comma - 1);
        return {stoi(x), stoi(y)};
      };

      string s;
      while (getline(src, s)) {
        auto parts = split(s, ' ');

        robot& r = robots.emplace_back();

        r.pos = parse_point(parts[0]);
        r.vel = parse_point(parts[1]);
      }
    }

    void updateQuadrant(quadrants& q, int32_t x, int32_t y, int32_t w, int32_t h) {
      uint32_t mid_w = (w - 1) / 2;
      uint32_t mid_h = (h - 1) / 2;

      assert(x >= 0 && x < w);
      assert(y >= 0 && y < h);

      if (x <= mid_w - 1 && y <= mid_h - 1)
        get<top_left>(q)++;
      else if (x >= mid_w + 1 && y <= mid_h - 1)
        get<top_right>(q)++;
      else if (x <= mid_w - 1 && y >= mid_h + 1)
        get<bottom_left>(q)++;
      else if (x >= mid_w + 1 && y >= mid_h + 1)
        get<bottom_right>(q)++;
    }

    void print_arena(int32_t arena_h) {
      if (!g_config.debug)
        return;

      static bool move = false;
      if (move)
        cout << term::cursor::move_up(arena_h);
      move = true;
      for (auto& r : arena)
        fill(r.begin(), r.end(), '.');
      for (const auto& robot : robots) {
        const int32_t& x = robot.pos.x;
        const int32_t& y = robot.pos.y;
        char&          c = arena[y][x];
        if (c == '.')
          c = '#';
      }
      for (auto& r : arena)
        cout << r << endl;
    }

    bool is_tree() {
      bool result = false;

      sort(robots.begin(), robots.end(), [](const robot& l, const robot& r) -> bool {
        return (r.pos.y == l.pos.y) ? r.pos.x > l.pos.x : r.pos.y > l.pos.y;
      });

      uint32_t span = 0;
      for (int i = 0; i < robots.size() - 1 && !result; i++) {
        const robot& cur  = robots[i];
        const robot& next = robots[i + 1];
        if (cur.pos.y == next.pos.y) {
          int32_t delta = abs(next.pos.x - cur.pos.x);
          assert(delta >= 0);
          if (delta <= 1)
            span++;
          else
            span = 0;
        } else
          span = 0;

        result = span >= 15;
      }

      return result;
    }

    auto getSafetyFactor(int32_t arena_w, int32_t arena_h, uint32_t time) -> quadrants {
      quadrants answer;

      if (g_config.debug) {
        arena.resize(arena_h);
        for (auto& r : arena)
          r = string(arena_w, '.');
      }

      for (uint32_t t = 0; t < time || find_tree; t++) {
        for (auto& r : robots) {
          r.pos = r.pos + r.vel;

          auto& x = r.pos.x;
          auto& y = r.pos.y;

          // x wrap
          if (x >= arena_w)
            x = x % arena_w;
          else if (x < 0)
            x = arena_w - (-1 * x + arena_w) % arena_w;

          // y wrap
          if (y >= arena_h)
            y = y % arena_h;
          else if (y < 0)
            y = arena_h - (-1 * y + arena_h) % arena_h;
        }

        if (find_tree) {
          if (is_tree()) {
            print_arena(arena_h);

            tree_time = t + 1;
            find_tree = false;
            time      = 0;
            break;
          }
        }
      }

      if (!find_tree)
        for (auto& r : robots)
          updateQuadrant(answer, r.pos.x, r.pos.y, arena_w, arena_h);

      return answer;
    }
};

TEST_F(Day14, Part1Custom) {
  uint32_t ul, ur, bl, br;
  string   s;

  // Custom
  s =
      // clang-format off
    "p=0,0 v=0,-2\n"
    "p=0,0 v=-1,0\n"
    "p=0,1 v=0,1\n"
    "p=0,0 v=2,0\n"
    "p=0,0 v=1,0\n";
  // clang-format on

  auto input = istringstream(s);
  readInput(input);

  tie(ul, ur, bl, br) = getSafetyFactor(11, 7, 30);

  // Short Example
  s =
      // clang-format off
    "p=2,4 v=2,-3\n";
  // clang-format on

  input = istringstream(s);
  readInput(input);

  tie(ul, ur, bl, br) = getSafetyFactor(11, 7, 5);
  EXPECT_EQ(ul, 0);
  EXPECT_EQ(ur, 0);
  EXPECT_EQ(bl, 0);
  EXPECT_EQ(br, 0);
  EXPECT_EQ(robots[0].pos.x, 1);
  EXPECT_EQ(robots[0].pos.y, 3);
}

TEST_F(Day14, Part1Example) {
  string s =
      // clang-format off
    //"p=0,0 v=0,-1\n"

    "p=2,4 v=2,-3\n"
    "p=0,4 v=3,-3\n"
    "p=6,3 v=-1,-3\n"
    "p=10,3 v=-1,2\n"
    "p=2,0 v=2,-1\n"
    "p=0,0 v=1,3\n"
    "p=3,0 v=-2,-2\n"
    "p=7,6 v=-1,-3\n"
    "p=3,0 v=-1,-2\n"
    "p=9,3 v=2,3\n"
    "p=7,3 v=-1,2\n"
    "p=9,5 v=-3,-3\n";
  // clang-format on
  auto input = istringstream(s);
  readInput(input);

  auto [ul, ur, bl, br] = getSafetyFactor(11, 7, 100);
  uint32_t safety       = ul * ur * bl * br;
  EXPECT_EQ(safety, 12);
}

TEST_F(Day14, Part1) {
  auto [ul, ur, bl, br] = getSafetyFactor(101, 103, 100);
  uint32_t safety       = ul * ur * bl * br;
  EXPECT_EQ(safety, 229421808);
}

TEST_F(Day14, Part2) {
  find_tree = true;

  getSafetyFactor(101, 103, -1);
  EXPECT_EQ(tree_time, 6577);
}