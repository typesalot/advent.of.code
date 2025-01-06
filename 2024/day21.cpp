#include "test.h"
#include "util/point.h"
#include <string>
#include <vector>

using namespace std;

class Day21 : public aoc_2024 {
  protected:
    using point = point<int16_t>;

    vector<string> sequences;

    struct keypad {
        unordered_map<char, uint16_t> keys;
        point                         avoid;

        keypad(const string& labels) : keys(labels.size()) {
          for (auto l : labels)
            keys[l] = labels.find(l);
          avoid = point::expand(keys['*'], 3);
        }

        point origin() {
          return key('A');
        }

        point key(char c) {
          return point::expand(keys[c], 3);
        }

        void move(const point& src, const point& dst, vector<string>& choices) {
          point mov = dst - src;

          gen(src, dst, {0, 0}, mov.x, mov.y, "", choices);
        }

      private:
        void gen(const point& p, const point& dst, const point& dir, int x, int y, string s, vector<string>& choices) {
          static const point directions[] = {{0, -1},   // north
                                             {1, 0},    // east
                                             {0, 1},    // south
                                             {-1, 0}};  // west

          point next = p + dir;
          if (next == avoid)
            return;
          if (next == dst) {
            choices.emplace_back(s + 'A');
            return;
          }

          if (x < 0)
            gen(next, dst, directions[3], x + 1, y, s + '<', choices);
          else if (x > 0)
            gen(next, dst, directions[1], x - 1, y, s + '>', choices);
          if (y < 0)
            gen(next, dst, directions[0], x, y + 1, s + '^', choices);
          else if (y > 0)
            gen(next, dst, directions[2], x, y - 1, s + 'v', choices);
        }
    };

    void LoadInput(istringstream& input) override {
      string s;
      while (getline(input, s))
        sequences.emplace_back(s);
    }

    uint32_t getComplexity(const string& seq) {
      uint32_t size = 0;

      int low = numeric_limits<int>::max();

      keypad kp_numeric("789456123*0A");
      keypad kp_mov("*^A<v>");

      vector<string> all0;
      vector<string> all1;

      auto src = kp_numeric.origin();
      for (auto c : seq) {
        vector<string> choices;

        auto dst   = kp_numeric.key(c);
        auto delta = dst - src;

        kp_numeric.move(src, dst, choices);

        src += delta;

        all0 = combine(all0, choices);
      }

      // robot1
      vector<int> lengths;
      low = numeric_limits<int>::max();
      for (uint32_t i = 0; i < all0.size(); i++) {
        int&  l   = lengths.emplace_back(0);
        point src = kp_mov.origin();
        for (auto c : all0[i]) {
          auto dst   = kp_mov.key(c);
          auto delta = dst - src;

          l   = l + delta.manhattan() + 1;  // +1 for 'A'
          src = src + delta;
        }
        low = min<int>(low, l);
      }
      for (uint32_t i = 0; i < all0.size(); i++) {
        if (lengths[i] != low)
          continue;

        vector<string> a;
        point          src = kp_mov.origin();
        for (auto c : all0[i]) {
          vector<string> choices;

          auto dst   = kp_mov.key(c);
          auto delta = dst - src;

          kp_mov.move(src, dst, choices);
          src = src + delta;

          a = combine(a, choices);
        }

        all1.insert(all1.end(), a.begin(), a.end());
      }

      // robot2
      lengths.clear();
      low = numeric_limits<int>::max();

      for (uint32_t i = 0; i < all1.size(); i++) {
        int&  l   = lengths.emplace_back(0);
        point src = kp_mov.origin();
        for (auto c : all1[i]) {
          auto dst   = kp_mov.key(c);
          auto delta = dst - src;

          l   = l + delta.manhattan() + 1;  // +1 for 'A'
          src = src + delta;
        }
        low = min<int>(low, l);
      }

      uint32_t n = atoi(seq.substr(0, seq.length() - 1).c_str());
      return n * low;
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

TEST_F(Day21, NumericAvoid) {
  // vector<string> c;
  // vector<string> n;

  // // Avoids *
  // numeric.move('0', n);
  // c = combine(c, n);
  // n.clear();
  // numeric.move('1', n);
  // c = combine(c, n);
  // n.clear();
  // numeric.move('0', n);
  // c = combine(c, n);

  // EXPECT_EQ(c.size(), 1);
  // EXPECT_EQ(c[0], "<A^<A>vA");
}

TEST_F(Day21, Numeric029A) {
  // vector<string> c;
  // vector<string> n;

  // numeric.move('0', n);
  // c = combine(c, n);
  // n.clear();
  // numeric.move('2', n);
  // c = combine(c, n);
  // n.clear();
  // numeric.move('9', n);
  // c = combine(c, n);
  // n.clear();
  // numeric.move('A', n);
  // c = combine(c, n);

  // array<string, 3> p = {"<A^A>^^AvvvA", "<A^A^>^AvvvA", "<A^A^^>AvvvA"};
  // for (const auto& r : c)
  //   EXPECT_NE(find(p.begin(), p.end(), r), p.end());
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