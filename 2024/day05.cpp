#include "common.h"

using rules_t   = unordered_map<int, vector<int>>;
using updates_t = vector<vector<int>>;

void readInput(rules_t& rules, updates_t& updates) {
  vector<vector<int>> input;

  auto inputFile = getInputFile(2024, 5);
  auto f         = ifstream(inputFile);

  string s;
  bool   read_rules = true;

  while (getline(f, s)) {
    if (s == "") {
      read_rules = false;
      continue;
    }
    if (read_rules) {
      auto parts  = split(s, '|');
      int  before = stoi(parts[0]);
      int  after  = stoi(parts[1]);
      rules[before].push_back(after);
    } else {
      auto& v     = updates.emplace_back();
      auto  parts = split(s, ',');
      for (auto p : parts)
        v.push_back(stoi(p));
    }
  }
}

int checkUpdate(updates_t::value_type& update, rules_t& rules) {
  int  last    = -1;
  bool correct = true;
  for (auto page : update) {
    if (last != -1) {
      const auto& rule = rules[last];
      auto        res  = std::find(rule.begin(), rule.end(), page);
      if (res == rule.end()) {
        correct = false;
        break;
      }
    }
    last = page;
  }
  return (correct) ? update[update.size() / 2] : 0;
}

int calcUpdates(rules_t& rules, updates_t& updates, bool fix) {
  int total = 0;

  for (auto& update : updates)
    total += checkUpdate(update, rules);

  return total;
}

TEST(Day5, Part1) {
  rules_t   rules;
  updates_t updates;
  readInput(rules, updates);

  int ans = calcUpdates(rules, updates, false);
  EXPECT_EQ(ans, 5064);
  cout << "Answer = " << ans << endl;
}

TEST(Day5, Part2) {
  rules_t   rules;
  updates_t updates;
  readInput(rules, updates);
  
  int ans = calcUpdates(rules, updates, true);
  // EXPECT_EQ(ans, 5064);
  cout << "Answer = " << ans << endl;
}