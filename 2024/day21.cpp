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
        unordered_map<char, uint16_t>           keys;
        point                                   avoid;
        unordered_map<uint32_t, vector<string>> cache;

        keypad(const string& labels) : keys(labels.size()) {
          for (auto l : labels)
            keys[l] = labels.find(l);
          avoid = point::expand(keys['*'], 3);

          for (auto s : labels) {
            for (auto d : labels) {
              point src = key(s);
              point dst = key(d);

              if (src == avoid || dst == avoid)
                continue;

              auto            k       = make_key(src, dst);
              vector<string>& choices = cache[k];

              point mov = dst - src;
              gen(src, dst, {0, 0}, mov.x, mov.y, "", choices);
            }
          }
        }

        union key {
            struct {
                uint8_t x1 : 8;
                uint8_t y1 : 8;
                uint8_t x2 : 8;
                uint8_t y2 : 8;
            } m;

            uint32_t value;
        };

        uint32_t make_key(const point& p1, const point& p2) {
          union key k;
          k.m.x1 = p1.x;
          k.m.y1 = p1.y;
          k.m.x2 = p2.x;
          k.m.y2 = p2.y;
          return k.value;
        }

        point origin() {
          return key('A');
        }

        point key(char c) {
          return point::expand(keys[c], 3);
        }

        void move(const point& src, const point& dst, vector<string>& choices) {
          choices = cache[make_key(src, dst)];
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

    keypad kp_mov;
    keypad kp_numeric;

    Day21() : kp_mov("*^A<v>"), kp_numeric("789456123*0A") {};

    uint32_t robot(vector<string>& in, vector<string>& out, bool min_only = false) {
      vector<int> lengths;

      int low = numeric_limits<int>::max();

      for (uint32_t i = 0; i < in.size(); i++) {
        int&  l   = lengths.emplace_back(0);
        point src = kp_mov.origin();
        for (auto c : in[i]) {
          auto dst   = kp_mov.key(c);
          auto delta = dst - src;

          l   = l + delta.manhattan() + 1;  // +1 for 'A'
          src = src + delta;
        }
        low = min<int>(low, l);
      }

      if (min_only)
        return low;

      for (uint32_t i = 0; i < in.size(); i++) {
        if (lengths[i] != low)
          continue;

        vector<string> a;
        point          src = kp_mov.origin();
        for (auto c : in[i]) {
          vector<string> choices;

          auto dst   = kp_mov.key(c);
          auto delta = dst - src;

          kp_mov.move(src, dst, choices);
          src = src + delta;

          a = combine(a, choices);
        }

        out.insert(out.end(), a.begin(), a.end());
      }

      return low;
    }

    uint32_t getComplexity(const string& seq, uint32_t robots) {
      uint32_t size = 0;

      int low = numeric_limits<int>::max();

      array<vector<string>, 3> ring;

      uint32_t i = 0;

      // numeric input requirement
      auto src = kp_numeric.origin();
      for (auto c : seq) {
        vector<string> choices;
        auto           dst   = kp_numeric.key(c);
        auto           delta = dst - src;
        kp_numeric.move(src, dst, choices);
        src += delta;
        ring[i] = combine(ring[i], choices);
      }

      // let the robots do their thing
      while (robots) {
        // last robot doesn't need to generate output
        bool min_only = (robots == 1);

        auto& in  = ring[i % 3];
        auto& out = ring[(i + 1) % 3];
        out.clear();

        low = robot(in, out, min_only);
        robots--;
        i++;
      }

      uint32_t n = atoi(seq.substr(0, seq.length() - 1).c_str());
      return n * low;
    }

    uint32_t getTotalComplexity(uint32_t robots) {
      uint32_t total = 0;
      for (const auto& seq : sequences)
        total += getComplexity(seq, robots);
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

TEST_F(Day21, Part1Example) {
  input = "029A\n"
          "980A\n"
          "179A\n"
          "456A\n"
          "379A";
  SetUp();

  auto answer = getTotalComplexity(2);
  EXPECT_EQ(answer, 126384);
}

TEST_F(Day21, Part1) {
  auto answer = getTotalComplexity(2);
  EXPECT_EQ(answer, 278568);
}

TEST_F(Day21, Part2) {
  auto answer = getTotalComplexity(5);
  EXPECT_EQ(answer, 278568);
}