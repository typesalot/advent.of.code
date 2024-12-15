#include "common.h"

class Day12 : public testing::Test {
  protected:
    vector<term::string> plot;

    void SetUp() override {
      auto   fname = getInputFile(2024, 12);
      auto   f     = ifstream(fname);
      string s;
      while (getline(f, s))
        plot.push_back(s);
    };

    uint32_t getPrice() {
      uint32_t price = 0;

      return price;
    }
};

TEST_F(Day12, Part1Example) {
  plot = {"RRRRIICCFF",
          "RRRRIICCCF",
          "VVRRRCCFFF",
          "VVRCCCJFFF",
          "VVVVCJJCFE",
          "VVIVCCJJEE",
          "VVIIICJJEE",
          "MIIIIIJJEE",
          "MIIISIJEEE",
          "MMMISSJEEE"};

  uint32_t price = getPrice();
  EXPECT_EQ(price, 1930);
}

TEST_F(Day12, Part1) {
}