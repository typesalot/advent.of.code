#include "common.h"

class Day13 : public ::testing::Test {
  protected:
    using point = point_int;

    struct machine {
        point a;
        point b;
        point prize;
    };

    vector<machine>               machines;
    vector<pair<int, int>>        matches;
    unordered_map<uint32_t, bool> cache;

    union cache_key {
        struct {
            uint32_t a : 16;
            uint32_t b : 16;
        };

        uint32_t value;
    };

    void SetUp() override {
      string fname = getInputFile(2024, 13);
      auto   f     = ifstream(fname);
      readInput(f);
    }

    template <typename source_t>
    void readInput(source_t& src) {
      machines.clear();

      string s;
      int    i = 0;
      while (getline(src, s)) {
        i++;
        if (i % 4 == 0)
          continue;
        else if (i % 4 == 1)
          machines.emplace_back();

        auto   x_f   = s.find('X') + 2;
        auto   comma = s.find(',');
        auto   y_f   = s.find('Y') + 2;
        string x     = s.substr(x_f, comma - x_f);
        string y     = s.substr(y_f, s.length() - y_f);

        if (i % 4 == 1)
          machines.back().a = {stoi(x), stoi(y)};
        else if (i % 4 == 2)
          machines.back().b = {stoi(x), stoi(y)};
        else if (i % 4 == 3)
          machines.back().prize = {stoi(x), stoi(y)};
      }
    }

    void token_path(int a, int b, const machine& m) {
      cache_key key;
      assert(a < (1 << 16) - 1);
      assert(b < (1 << 16) - 1);
      key.a = a;
      key.b = b;

      cache[key.value] = true;

      int x = a * m.a.x + b * m.b.x;
      int y = a * m.a.y + b * m.b.y;

      if (y > m.prize.y && x > m.prize.x)
        return;
      if (y == m.prize.y && x == m.prize.x) {
        matches.emplace_back(a, b);
        return;
      }

      key.a++;
      if (!cache[key.value])
        token_path(a + 1, b, m);  // press A...
      key.a--;
      key.b++;
      if (!cache[key.value])
        token_path(a, b + 1, m);  // or press B...
    };

    optional<uint32_t> getMinTokens(const machine& m) {
      uint32_t tokens = numeric_limits<uint32_t>::max();

      cache.clear();
      matches.clear();
      token_path(0, 0, m);

      if (matches.size()) {
        for (const auto [a, b] : matches)
          tokens = min<uint32_t>(tokens, a * 3 + b);
      } else {
        return nullopt;
      }

      return tokens;
    }

    uint32_t minTokens() {
      uint32_t tokens = 0;

      for (const auto& m : machines) {
        auto t = getMinTokens(m);
        if (t)
          tokens += t.value();
      }

      return tokens;
    }
};

TEST_F(Day13, Part1Example) {
  string s =
      // clang-format off
    "Button A: X+94, Y+34\n"
    "Button B: X+22, Y+67\n"
    "Prize: X=8400, Y=5400\n"
    "\n"
    "Button A: X+26, Y+66\n"
    "Button B: X+67, Y+21\n"
    "Prize: X=12748, Y=12176\n"
    "\n"
    "Button A: X+17, Y+86\n"
    "Button B: X+84, Y+37\n"
    "Prize: X=7870, Y=6450\n"
    "\n"
    "Button A: X+69, Y+23\n"
    "Button B: X+27, Y+71\n"
    "Prize: X=18641, Y=10279\n";
  // clang-format on

  auto input = istringstream(s);
  readInput(input);

  int tokens = minTokens();
  EXPECT_EQ(tokens, 480);
}

TEST_F(Day13, Part1) {
  int tokens = minTokens();
  EXPECT_EQ(tokens, 31623);
}