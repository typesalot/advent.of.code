#include "common.h"

class Day11 : public testing::Test {
  protected:
    vector<uint64_t>                             stones;
    array<unordered_map<uint64_t, uint64_t>, 75> cache;

    void SetUp() override {
      string fname = getInputFile(2024, 11);
      auto   f     = ifstream(fname);
      string s;
      getline(f, s);
      auto parts = split(s, ' ');
      for (auto& p : parts)
        stones.push_back(stoi(p));
    }

    uint64_t digit_count(uint64_t num) {
      uint64_t p = 0;
      uint64_t n = num;
      while (n) {
        p++;
        n /= 10;
      }

      return p;
    }

    auto split_num(uint64_t num) -> pair<uint64_t, uint64_t> {
      uint64_t cnt = digit_count(num) / 2;
      uint64_t p   = 1;
      uint64_t n   = num;
      while (cnt--) {
        p *= 10;
        n /= 10;
      }
      return {n, num % p};
    }

    uint64_t blink(uint64_t num, uint32_t cnt) {
      uint64_t total = 1;

      if (cnt != 0) {
        if (cache[cnt - 1].contains(num))
          return cache[cnt - 1][num];

        if (num == 0) {
          total = blink(1, cnt - 1);
        } else if (digit_count(num) % 2 == 0) {
          auto [u, l] = split_num(num);
          total       = blink(u, cnt - 1) + blink(l, cnt - 1);
        } else {
          total = blink(num * 2024, cnt - 1);
        }

        cache[cnt - 1][num] = total;
      }

      return total;
    }

    uint64_t numStones(uint64_t cnt) {
      uint64_t total = 0;
      for (auto s : stones)
        total += blink(s, cnt);
      return total;
    }
};

TEST_F(Day11, SplitNum) {
  uint64_t u, l;

  tie(u, l) = split_num(12);
  EXPECT_EQ(u, 1);
  EXPECT_EQ(l, 2);

  tie(u, l) = split_num(1234);
  EXPECT_EQ(u, 12);
  EXPECT_EQ(l, 34);

  tie(u, l) = split_num(123456);
  EXPECT_EQ(u, 123);
  EXPECT_EQ(l, 456);
}

TEST_F(Day11, Part1Example) {
  stones          = {125, 17};
  uint64_t answer = numStones(6);
  EXPECT_EQ(answer, 22);

  stones = {125, 17};
  answer = numStones(25);
  EXPECT_EQ(answer, 55312);
}

TEST_F(Day11, Part1) {
  uint64_t answer = numStones(25);
  EXPECT_EQ(answer, 217812);
}

TEST_F(Day11, Part2) {
  uint64_t answer = numStones(75);
  EXPECT_EQ(answer, 259112729857522);
}