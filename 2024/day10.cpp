#include "common.h"

class Day10 : public ::testing::Test {
  protected:
    vector<string> map;

    void SetUp() override {
      auto   fname = getInputFile(2024, 10);
      auto   f     = ifstream(fname);
      string s;
      while (getline(f, s))
        map.emplace_back(s);
    }

    uint32_t getScore() {
      uint32_t score = 0;

      return score;
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