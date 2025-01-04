#include "test.h"
#include "util/point.h"
#include <string>
#include <vector>

using namespace std;

class Day21 : public aoc_2024 {
  protected:
    using point = point<int8_t>;

    vector<string>               sequences;
    static const array<point, 4> directions;

    struct numeric_keypad {
        unordered_map<char, uint8_t> keys;  // key positions, flattened
        point                        src;   // current x,y position

        numeric_keypad() : keys(12) {
          string labels = "789456123*0A";
          for (auto l : labels)
            keys[l] = labels.find(l);
          src = point::expand(keys['A'], 3);
        }

        string move(char c) {
          string presses;

          point dst = point::expand(keys[c], 3);
          point mov = dst - src;

          // if 0, move y first to avoid *
          if (src == point(1, 3)) {
            move_y(presses, mov);
            move_x(presses, mov);
          } else {
            move_x(presses, mov);
            move_y(presses, mov);
          }
          presses += 'A';

          src = dst;

          return presses;
        }

        void move_x(string& presses, const point& d) {
          presses.append(abs(d.x), (d.x < 0) ? '<' : '>');
        }

        void move_y(string& presses, const point& d) {
          presses.append(abs(d.y), (d.y < 0) ? '^' : 'v');
        }
    } num_kp;

    void LoadInput(istringstream& input) override {
      string s;
      while (getline(input, s))
        sequences.emplace_back(s);
    }

    uint32_t getComplexity(const string& seq) {
      uint32_t size = 0;

      for (auto c : seq)
        num_kp.move(c);

      return size;
    }

    uint32_t getTotalComplexity() {
      uint32_t total = 0;
      for (const auto& seq : sequences)
        total += getComplexity(seq);
      return total;
    }
};

decltype(Day21::directions) Day21::directions = {
    point{0, -1},  // north
    point{1, 0},   // east
    point{0, 1},   // south
    point{-1, 0}   // west
};

TEST_F(Day21, NumericKeypadAvoid) {
  // Avoids *
  EXPECT_EQ(num_kp.move('0'), "<");
  EXPECT_EQ(num_kp.move('1'), "^<");
  EXPECT_EQ(num_kp.move('0'), ">V");
}

TEST_F(Day21, NumericKeypad029A) {
  string s;
  s += num_kp.move('0');
  s += num_kp.move('2');
  s += num_kp.move('9');
  s += num_kp.move('A');

  array<string, 3> p = {"<A^A>^^AvvvA", "<A^A^>^AvvvA", "<A^A^^>AvvvA"};
  EXPECT_NE(find(p.begin(), p.end(), s), p.end());
}

TEST_F(Day21, Part1Example) {
  input = "029A\n"
          "980A\n"
          "179A\n"
          "456A\n"
          "379A";
  SetUp();

  auto answer = getTotalComplexity();
  EXPECT_EQ(answer, 126384);
}

TEST_F(Day21, Part1) {
}