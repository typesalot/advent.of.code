#include "common.h"

class Day14 : public ::testing::Test {
  protected:
    using point = point_int;

    struct robot {
        point pos;
        point vel;
    };

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

    int getSafetyFactor() {
      int safety = 0;
      return safety;
    }
};

TEST_F(Day14, Part1Example) {
  string s =
      // clang-format off
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
    "p=2,4 v=2,-3\n"
    "p=9,5 v=-3,-3\n";
  // clang-format on
  auto input = istringstream(s);
  readInput(input);

  int safety = getSafetyFactor();
  // EXPECT_EQ(safety, 12);
}