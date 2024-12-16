#include "common.h"

class Day15 : public ::testing::Test {
  protected:
    vector<string> input;

    void SetUp() override {
      auto   fname = getInputFile(2024, 15);
      auto   f     = ifstream(fname);
      string s;
      while (getline(f, s))
        input.push_back(s);
    };
};

TEST_F(Day15, Part1Example) {
}