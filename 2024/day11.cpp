#include "common.h"

class Day11 : public testing::Test {
  protected:
    vector<int> stones;

    void SetUp() override {
      string fname = getInputFile(2024, 11);
      auto   f     = ifstream(fname);
      string s;
      getline(f, s);
      auto parts = split(s, ' ');
      for (auto& p : parts)
        stones.push_back(stoi(p));
    }

    uint32_t digit_count(uint32_t num) {
      uint32_t p = 0;
      uint32_t n = num;
      while (n) {
        p++;
        n /= 10;
      }

      return p;
    }

    auto split_num(uint32_t num) -> pair<uint32_t, uint32_t> {
      uint32_t cnt = digit_count(num) / 2;
      uint32_t p   = 1;
      uint32_t n   = num;
      while (cnt--) {
        p *= 10;
        n /= 10;
      }
      return {n, num % p};
    }

    void blink() {
      auto itr = stones.begin();

      while (itr != stones.end()) {
        if (*itr == 0) {
          *itr = 1;
          itr++;
        } else if (digit_count(*itr) % 2 == 0) {
          auto n      = itr + 2;
          auto [u, l] = split_num(*itr);

          *itr = l;
          itr  = stones.insert(itr, u);

          itr++;
        } else {
          *itr *= 2024;
          itr++;
        }
      }
    }

    uint64_t numStones(uint32_t blinkCnt) {
      while (blinkCnt--)
        blink();
      return stones.size();
    }
};

TEST_F(Day11, SplitNum) {
  uint32_t u, l;

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