#include "test.h"
#include "util/point.h"
#include <string>
#include <vector>

using namespace std;

class Day21 : public aoc_2024 {
  protected:
    using point = point<int16_t>;

    vector<string> sequences;

    static const array<point, 4> directions;
    static constexpr int         north = 0;
    static constexpr int         east  = 1;
    static constexpr int         south = 2;
    static constexpr int         west  = 3;

    struct keypad {
        unordered_map<char, uint16_t> keys;  // key positions, flattened
        point                         src;   // current x,y position
        point                         dst;
        vector<string>                choices;
        point                         avoid;

        keypad(const string& labels) : keys(labels.size()) {
          for (auto l : labels)
            keys[l] = labels.find(l);
          src   = point::expand(keys['A'], 3);
          avoid = point::expand(keys['*'], 3);
        }

        vector<string> move(char c) {
          dst       = point::expand(keys[c], 3);
          point mov = dst - src;
          point none;

          choices.clear();

          gen(src, none, mov.x, mov.y, "");
          src = dst;

          return choices;
        }

      private:
        void gen(const point& p, const point& dir, int x, int y, string s) {
          point next = p + dir;
          if (next == avoid)
            return;
          if (next == dst) {
            choices.emplace_back(s + 'A');
            return;
          }

          if (x < 0)
            gen(next, directions[west], x + 1, y, s + '<');
          else if (x > 0)
            gen(next, directions[east], x - 1, y, s + '>');
          if (y < 0)
            gen(next, directions[north], x, y + 1, s + '^');
          else if (y > 0)
            gen(next, directions[south], x, y - 1, s + 'v');
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

      int low = numeric_limits<int>::max();

      vector<string> all0;
      vector<string> all1;
      vector<string> all2;
      for (auto s : seq) {
        auto choices = numeric.move(s);
        all0         = combine(all0, choices);
      }

      for (const auto& c : all0) {
        vector<string> a;

        for (auto s : c) {
          auto choices = mov1.move(s);
          a            = combine(a, choices);
        }

        int a_low = numeric_limits<int>::max();
        for (auto n : a)
          a_low = std::min<int>(a_low, n.length());
        if (a_low <= low) {
          if (a_low == low)
            all1.insert(all1.end(), a.begin(), a.end());
          else
            all1 = a;
          low = a_low;
        }
      }

      low = numeric_limits<int>::max();

      for (const auto& c : all1) {
        vector<string> a;

        for (auto s : c) {
          auto choices = mov2.move(s);
          a            = combine(a, choices);
        }

        int a_low = numeric_limits<int>::max();
        for (auto n : a)
          a_low = std::min<int>(a_low, n.length());
        if (a_low < low) {
          all2 = a;
          low  = a_low;
        }
      }

      // for (auto& a : all2)
      //   cout << a << endl;

      uint32_t n = atoi(seq.substr(0, seq.length() - 1).c_str());
      uint32_t s = low;

      return n * s;
    }

    uint32_t getTotalComplexity() {
      uint32_t total = 0;
      for (const auto& seq : sequences)
        total += getComplexity(seq);
      return total;
    }

    auto combine(const vector<string>& l, const vector<string>& r) -> vector<string> {
      if (l.size() == 0)
        return r;
      if (r.size() == 0)
        return l;

      vector<string> result;
      for (const auto& s : l)
        for (const auto& t : r)
          result.emplace_back(s + t);
      return result;
    };
};

decltype(Day21::directions) Day21::directions = {point{0, -1},   // north
                                                 point{1, 0},    // east
                                                 point{0, 1},    // south
                                                 point{-1, 0}};  // west

TEST_F(Day21, NumericAvoid) {
  vector<string> c;
  vector<string> n;

  // Avoids *
  n = numeric.move('0');
  c = combine(c, n);
  n = numeric.move('1');
  c = combine(c, n);
  n = numeric.move('0');
  c = combine(c, n);

  EXPECT_EQ(c.size(), 1);
  EXPECT_EQ(c[0], "<A^<A>vA");
}

TEST_F(Day21, Numeric029A) {
  vector<string> c;
  vector<string> n;

  n = numeric.move('0');
  c = combine(c, n);
  n = numeric.move('2');
  c = combine(c, n);
  n = numeric.move('9');
  c = combine(c, n);
  n = numeric.move('A');
  c = combine(c, n);

  array<string, 3> p = {"<A^A>^^AvvvA", "<A^A^>^AvvvA", "<A^A^^>AvvvA"};
  for (const auto& r : c)
    EXPECT_NE(find(p.begin(), p.end(), r), p.end());
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
  EXPECT_EQ(answer, 278568);
}