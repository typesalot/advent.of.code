#include "test.h"
#include "util/point.h"
#include "util/term.h"
#include <queue>

using namespace std;

class Day18 : public aoc_2024 {
  protected:
    using point = point_uint;

    uint32_t             width;
    uint32_t             height;
    uint32_t             fall_count;
    queue<point>         spawns;
    vector<term::string> map;

    void LoadInput(istringstream& input) override {
      map.resize(height);
      for (auto& m : map)
        m = string(width, '.');

      string s;
      while (getline(input, s)) {
        size_t f = s.find(",");
        point& p = spawns.emplace();
        p.x      = stoi(s.substr(0, f));
        p.y      = stoi(s.substr(f + 1, s.length() - f));
        if (fall_count) {
          map[p.y][p.x] = '#';
          fall_count--;
        }
      }
    }

    void print_map() {
      static bool move = false;
      if (move)
        cout << term::cursor::move_up(height);
      move = true;
      for (const auto& m : map)
        cout << m << endl;
    }

    uint32_t countShortestPath() {
      uint32_t answer = 0;

      print_map();

      return answer;
    }
};

TEST_F(Day18, Part1Example) {
  input      = "5,4\n"
               "4,2\n"
               "4,5\n"
               "3,0\n"
               "2,1\n"
               "6,3\n"
               "2,4\n"
               "1,5\n"
               "0,6\n"
               "3,3\n"
               "2,6\n"
               "5,1\n"
               "1,2\n"
               "5,5\n"
               "2,5\n"
               "6,5\n"
               "1,4\n"
               "0,4\n"
               "6,4\n"
               "1,1\n"
               "6,1\n"
               "1,0\n"
               "0,5\n"
               "1,6\n"
               "2,0\n";
  width      = 7;
  height     = 7;
  fall_count = 12;
  SetUp();

  uint32_t answer = countShortestPath();
  EXPECT_EQ(answer, 22);
}