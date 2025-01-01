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

    static const uint32_t c_unvisited = 0;
    static const uint32_t c_fails     = 1;
    static const uint32_t c_matches   = 2;

    vector<string>   patterns;
    vector<string>   designs;
    uint32_t         count;
    stack<int>       choices;
    vector<uint64_t> history;

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

    void reset() {
      count = 0;
      while (!choices.empty())
        choices.pop();
      history.clear();
    }

    void dfs_all(node* current, const string& design, int i, stack<int>& candidates) {
      char c = current->letter;
      char l = design[i];

      if (c == l) {
        if (current->term && i + 1 < design.length())
          candidates.emplace(i + 1);

        if (i + 1 < design.length())
          for (auto child : current->children)
            dfs_all(child, design, i + 1, candidates);
      }
    }

    uint64_t countPossible(const string& design) {
      uint64_t count = 0;

      history.resize(design.length(), 0);
      choices.emplace(0);

      priority_queue<int> next;
      vector<int>         last;

      next.emplace(0);

      while (!next.empty()) {
        int i = next.top();
        next.pop();

        while (last.size() && last.back() > i)
          last.pop_back();
        last.push_back(i);

        stack<int> candidates;
        for (auto child : start->children)
          dfs_all(child, design, i, candidates);

        while (!candidates.empty()) {
          int c = candidates.top();
          candidates.pop();

          if (g_config.debug)
            cout << term::cursor::set_x(i + 1) << design.substr(i, c - i) << endl;

          if (c == design.length() - 1)
            count++;
          else
            next.push(c);
        }
      }

      return count;
    }

    uint32_t getAllPatterns() {
      uint32_t answer = 0;

      createPatternGraph();

      for (const auto& design : designs) {
        if (g_config.debug)
          cout << design << " = " << endl;
        reset();
        countPossible(design);
        answer += history[0];
      }

      return answer;
    }

    void dfs_single(node* current, const string& design, int i) {
      char c = current->letter;
      char l = design[i];

      if (c == l) {
        if (current->term) {
          if (i == design.length() - 1) {
            count++;
            return;
          }

          if (i + 1 < design.length() && !history[i + 1])
            choices.emplace(i + 1);
        }

        if (i + 1 < design.length()) {
          for (auto child : current->children) {
            dfs_single(child, design, i + 1);
          }
        }
      }
    }

    bool isPossible(const string& design) {
      choices.emplace(0);

      while (!count && !choices.empty()) {
        int i = choices.top();
        choices.pop();

        history[i] = true;

        for (auto child : start->children) {
          if (count)
            break;
          assert(i < design.length());
          dfs_single(child, design, i);
        }
      }

      return count != 0;
    }

    uint32_t getPossiblePatterns() {
      uint32_t answer = 0;

      createPatternGraph();

      for (const auto& design : designs) {
        reset();
        if (isPossible(design))
          answer++;
      }

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
          "brwrr\n"
          "bggr\n"
          "gbbr\n"
          "rrbgbr\n"
          "ubwu\n"
          "bwurrg\n"
          "brgr\n"
          "bbrgwb";
  SetUp();

  uint32_t answer = getAllPatterns();
  EXPECT_EQ(answer, 16);
};

TEST_F(Day19, Part2) {
  uint32_t answer = getAllPatterns();
  EXPECT_EQ(answer, 287);
}