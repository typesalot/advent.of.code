#include "common.h"

class Day15 : public ::testing::Test {
  protected:
    vector<term::string> map;
    string               movements;

    void SetUp() override {
      auto fname = getInputFile(2024, 15);
      auto f     = ifstream(fname);
      readInput(f);
    };

    template <typename src_t>
    void readInput(src_t& src) {
      map.clear();
      movements.clear();

      bool onmap = true;

      string s;
      while (getline(src, s)) {
        if (s == "") {
          onmap = false;
          continue;
        }

        if (onmap)
          map.push_back(s);
        else
          movements += s;
      }
    }

    int getBoxesGpsTotal() {
      int total = 0;

      return total;
    }
};

TEST_F(Day15, Part1Example) {
  string s =
      // clang-format off
    "########\n"
    "#..O.O.#\n"
    "##@.O..#\n"
    "#...O..#\n"
    "#.#.O..#\n"
    "#...O..#\n"
    "#......#\n"
    "########\n"
    "\n"
    "<^^>>>vv<v>>v<<\n";
  // clang-format on

  auto input = istringstream(s);
  readInput(input);

  int total = getBoxesGpsTotal();
  // EXPECT_EQ(total, 10092);
}