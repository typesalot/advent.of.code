#include "test.h"
#include "util/string.h"
#include <algorithm>
#include <string>

using namespace std;

class Day19 : public aoc_2024 {
  protected:
    vector<string> patterns;
    vector<string> designs;

    struct node {
        node(char _l = '\0') : letter(_l) {};
        ~node() = default;

        char         letter;
        bool         term = false;
        vector<node> children;
    };

    node* start = nullptr;

    void LoadInput(istringstream& input) override {
      string s;
      getline(input, s);
      auto parts = split(s, ',', true);
      patterns.insert(patterns.begin(), parts.begin(), parts.end());
      getline(input, s);
      while (getline(input, s))
        designs.push_back(s);
    }

    void createPatternGraph() {
      start = new node();

      for (const auto& pattern : patterns) {
        node* current = start;

        // find nearest leaf
        int i = 0;
        while (i < pattern.size()) {
          auto& children = current->children;

          auto n = find_if(children.begin(), children.end(), [l = pattern[i]](const node& n) -> bool {
            return n.letter == l;
          });

          if (n != children.end()) {
            current = &(*n);
            i++;
          } else {
            break;
          }
        }

        // insert the rest of the characters at that leaf location
        for (; i < pattern.size(); i++)
          current = &current->children.emplace_back(pattern[i]);

        // mark the final leaf as a terminal character
        current->term = true;
      }
    }

    bool isPossible(const string& design) {
      bool possible = true;

      node* current = start;

      // find nearest leaf
      int i = 0;
      while (i < design.size() && possible) {
        char  letter   = design[i];
        auto& children = current->children;

        auto n =
            find_if(children.begin(), children.end(), [l = letter](const node& n) -> bool { return n.letter == l; });

        if (n != children.end()) {
          current = &(*n);
          i++;
        } else if (current == start) {
          possible = false;
        } else {
          possible = current->term;
          current  = start;
        }
      }

      possible = possible && current->term;

      return possible;
    }

    uint32_t getPossiblePatterns() {
      uint32_t answer = 0;

      createPatternGraph();

      for (const string& s : designs)
        if (isPossible(s))
          answer++;

      return answer;
    }
};

TEST_F(Day19, Part1Example) {
  input = "r, wr, b, g, bwu, rb, gb, br\n"
          "\n"
          "brwrr\n"
          "bggr\n"
          "gbbr\n"
          "rrbgbr\n"
          "ubwu\n"
          "bwurrg\n"
          "brgr\n"
          "bbrgwb";
  SetUp();

  uint32_t answer = getPossiblePatterns();
  EXPECT_EQ(answer, 6);
}

TEST_F(Day19, Part1) {
}

TEST_F(Day19, Part2Example){};

TEST_F(Day19, Part2) {
}