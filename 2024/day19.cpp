#include "test.h"
#include "util/string.h"
#include "util/term.h"
#include <algorithm>
#include <stack>
#include <string>
#include <unordered_map>

using namespace std;

class Day19 : public aoc_2024 {
  protected:
    struct node;

    vector<string> patterns;
    vector<string> designs;

    struct node {
        node(char _l = '\0') : letter(_l) {};

        ~node() {
          for (auto c : children)
            delete c;
        }

        char          letter;
        bool          term = false;
        vector<node*> children;
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

    void TearDown() override {
      delete start;
    }

    void createPatternGraph() {
      start = new node();

      for (const auto& pattern : patterns) {
        node* current = start;

        // find nearest leaf
        int i = 0;
        while (i < pattern.size()) {
          auto& children = current->children;

          auto n = find_if(children.begin(), children.end(), [l = pattern[i]](const node* n) -> bool {
            return n->letter == l;
          });

          if (n != children.end()) {
            current = *n;
            i++;
          } else {
            break;
          }
        }

        // insert the rest of the characters at that leaf location
        for (; i < pattern.size(); i++)
          current = current->children.emplace_back(new node(pattern[i]));

        // mark the final leaf as a terminal character
        current->term = true;
      }
    }

    void dfs(node* current, const string& design, int i, queue<int>& candidates) {
      char c = current->letter;
      char l = design[i];

      if (c == l) {
        if (current->term && i + 1 <= design.length())
          candidates.emplace(i + 1);

        if (i + 1 < design.length())
          for (auto child : current->children)
            dfs(child, design, i + 1, candidates);
      }
    }

    uint64_t countPossible(const string& design, bool possible) {
      if (g_config.debug)
        cout << design << " = " << endl;

      stack<int>       next;                        // next character position to process
      bitset<64>       visited;                     // whether or not a node has been visited
      vector<uint64_t> counts(design.length(), 0);  // number of paths to the exit for a given position i
      stack<int>       last;                        // position path up to the current i
      queue<int>       candidates;                  // list of possible next positions to be filtered

      next.emplace(0);

      while (!next.empty()) {
        int i = next.top();
        next.pop();

        while (last.size() && last.top() > i) {
          auto prev = last.top();
          last.pop();
          counts[last.top()] += counts[prev];
        }

        if (i == design.length()) {
          if (possible)
            return 1;
          counts[last.top()]++;
          continue;
        }

        last.emplace(i);
        visited.set(i);

        for (auto child : start->children)
          dfs(child, design, i, candidates);

        while (!candidates.empty()) {
          int c = candidates.front();
          candidates.pop();

          if (g_config.debug)
            cout << term::cursor::set_x(i + 1) << design.substr(i, c - i) << endl;

          if (visited.test(c))
            counts[i] += counts[c];
          else
            next.push(c);
        }
      }

      while (last.size() > 1) {
        auto prev = last.top();
        last.pop();
        counts[last.top()] += counts[prev];
      }

      // final count is how many back fills made it back to the start
      return counts[0];
    }

    uint64_t getAllPatterns() {
      uint64_t answer = 0;

      createPatternGraph();

      for (const auto& design : designs)
        answer += countPossible(design, false);

      return answer;
    }

    uint64_t getPossiblePatterns() {
      uint64_t answer = 0;

      createPatternGraph();

      for (const auto& design : designs)
        answer += countPossible(design, true);

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
  uint32_t answer = getPossiblePatterns();
  EXPECT_EQ(answer, 287);
}

TEST_F(Day19, Part2Example) {
  input = "r, wr, b, g, bwu, rb, gb, br\n"
          "\n"
          "gbbr\n"
          "brwrr\n"
          "bggr\n"
          "rrbgbr\n"
          "ubwu\n"
          "bwurrg\n"
          "brgr\n"
          "bbrgwb";
  SetUp();

  uint64_t answer = getAllPatterns();
  EXPECT_EQ(answer, 16);
};

TEST_F(Day19, Part2) {
  uint64_t answer = getAllPatterns();
  EXPECT_EQ(answer, 571894474468161);
}