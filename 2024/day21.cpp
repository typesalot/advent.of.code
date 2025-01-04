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

    struct keypad {
        unordered_map<char, uint8_t> keys;  // key positions, flattened
        point                        src;   // current x,y position
        point                        pivot;

        keypad(const string& labels) : keys(labels.size()) {
          for (auto l : labels)
            keys[l] = labels.find(l);
          src   = point::expand(keys['A'], 3);
          pivot = point::expand(keys['*'], 3) + point(1, 0);
        }

        string move(char c) {
          string presses;

          point dst = point::expand(keys[c], 3);
          point mov = dst - src;

          if (src.y == pivot.y) {
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
    };

    struct numeric_keypad : public keypad {
        numeric_keypad() : keypad("789456123*0A") {
        }
    };

    struct move_keypad : public keypad {
        move_keypad() : keypad("*^A<v>") {
        }
    };

    numeric_keypad numeric;
    move_keypad    mov1;
    move_keypad    mov2;

    void LoadInput(istringstream& input) override {
      string s;
      while (getline(input, s))
        sequences.emplace_back(s);
    }

    uint32_t getComplexity(const string& seq) {
      uint32_t size = 0;

      string presses;
      string tmp1;
      string tmp2;
      for (auto c : seq)
        tmp1 += numeric.move(c);
      for (auto t1 : tmp1)
        tmp2 += mov1.move(t1);
      for (auto t2 : tmp2)
        presses += mov2.move(t2);

      uint32_t n = atoi(seq.substr(0, seq.length() - 1).c_str());
      uint32_t s = presses.size();

      return n * s;
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

TEST_F(Day21, NumericAvoid) {
  // Avoids *
  EXPECT_EQ(numeric.move('0'), "<A");
  EXPECT_EQ(numeric.move('1'), "^<A");
  EXPECT_EQ(numeric.move('0'), ">vA");
}

TEST_F(Day21, Numeric029A) {
  string s;
  s += numeric.move('0');
  s += numeric.move('2');
  s += numeric.move('9');
  s += numeric.move('A');

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
  auto answer = getTotalComplexity();
  EXPECT_EQ(answer, 293472);  // too high
}