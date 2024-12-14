#include "common.h"

class Day10 : public ::testing::Test {
  protected:
    vector<term::string>              map;
    unordered_map<uint32_t, uint32_t> history;
    uint32_t                          h = 0;
    uint32_t                          w = 0;

    uint32_t make_key(uint32_t x, uint32_t y) const {
      return y * w + x;
    }

    auto decompose_key(uint32_t key) -> pair<uint32_t, uint32_t> {
      uint32_t y = key / h;
      uint32_t x = key % h;
      return {x, y};
    }

    void SetUp() override {
      auto   fname = getInputFile(2024, 10);
      auto   f     = ifstream(fname);
      string s;
      while (getline(f, s))
        map.emplace_back(s);
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

    void set_location(uint32_t x, uint32_t y) {
      if (!g_config.debug)
        return;
      map[y].background(x).red();
      print_map();
    }

    void reset_colors() {
      if (!g_config.debug)
        return;
      for (auto& m : map)
        m.reset();
    }

    uint32_t score(uint32_t x, uint32_t y) {
      uint32_t key         = make_key(x, y);
      uint32_t local_score = 0;

      set_location(x, y);

      if (!history.contains(key)) {
        char cur_elevation = map[y][x];

        if (cur_elevation == '9') {
          local_score = 1;
        } else {
          if (y + 1 < h && map[y + 1][x] - cur_elevation == 1)
            local_score += score(x, y + 1);
          if (y != 0 && map[y - 1][x] - cur_elevation == 1)
            local_score += score(x, y - 1);
          if (x + 1 < w && map[y][x + 1] - cur_elevation == 1)
            local_score += score(x + 1, y);
          if (x != 0 && map[y][x - 1] - cur_elevation == 1)
            local_score += score(x - 1, y);
        }

        history[key] = local_score;
      }

      return local_score;
    }

    uint32_t rating(uint32_t x, uint32_t y) {
      uint32_t  key         = make_key(x, y);
      uint32_t& local_score = history[key];

      set_location(x, y);

      if (local_score == 0) {
        char cur_elevation = map[y][x];

        if (cur_elevation == '9') {
          local_score = 1;
        } else {
          if (y + 1 < h && map[y + 1][x] - cur_elevation == 1)
            local_score += rating(x, y + 1);
          if (y != 0 && map[y - 1][x] - cur_elevation == 1)
            local_score += rating(x, y - 1);
          if (x + 1 < w && map[y][x + 1] - cur_elevation == 1)
            local_score += rating(x + 1, y);
          if (x != 0 && map[y][x - 1] - cur_elevation == 1)
            local_score += rating(x - 1, y);
        }
      }

      return local_score;
    }

    uint32_t getScore() {
      uint32_t total_score = 0;

      h = map.size();
      w = map[0].length();

      for (uint32_t j = 0; j < h; j++)
        for (uint32_t i = 0; i < w; i++)
          if (map[j][i] == '0') {
            history.clear();
            reset_colors();
            total_score += score(i, j);
          }

      return total_score;
    }

    uint32_t getRating() {
      uint32_t total_score = 0;

      h = map.size();
      w = map[0].length();

      for (uint32_t j = 0; j < h; j++)
        for (uint32_t i = 0; i < w; i++)
          if (map[j][i] == '0') {
            reset_colors();
            total_score += rating(i, j);
          }

      return total_score;
    }
};

TEST_F(Day10, Part1Example) {
  map = {
      // clang-format off
      "89010123",
      "78121874",
      "87430965",
      "96549874",
      "45678903",
      "32019012",
      "01329801",
      "10456732"
      // clang-format on
  };

  uint32_t score = getScore();
  EXPECT_EQ(score, 36);
}

TEST_F(Day10, Part1) {
  uint32_t score = getScore();
  EXPECT_EQ(score, 776);
}

TEST_F(Day10, Part2Example) {
  map = {
      // clang-format off
      "89010123",
      "78121874",
      "87430965",
      "96549874",
      "45678903",
      "32019012",
      "01329801",
      "10456732"
      // clang-format on
  };

  uint32_t score = getRating();
  EXPECT_EQ(score, 81);
}

TEST_F(Day10, Part2) {
  uint32_t score = getRating();
  EXPECT_EQ(score, 1657);
}