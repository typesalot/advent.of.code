#include "common.h"

class Day8 : public ::testing::Test {
  protected:
    vector<string> map;

    void SetUp() override {
      readInput();
    }

    void readInput() {
      auto   fname = getInputFile(2024, 8);
      auto   f     = ifstream(fname);
      string s;
      while (getline(f, s))
        map.emplace_back(s);
    };

    int numAntinodes() {
      int count = 0;

      return count;
    }
};

TEST_F(Day8, Part1Examples) {
  int answer = numAntinodes();
  EXPECT_EQ(answer, 14);
  cout << "Answer = " << answer << endl;
}
