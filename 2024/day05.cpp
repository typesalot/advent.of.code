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
  bool correct = true;

  auto is_valid_order = [&rules](int from, int to) -> bool {
    const auto& rule = rules[from];
    auto        res  = std::find(rule.begin(), rule.end(), to);
    return res != rule.end();
  };

  int from = -1;
  for (auto to : update) {
    if (from != -1)
      correct = is_valid_order(from, to);
    if (!correct)
      break;
    from = to;
  }

  return (correct) ? update[update.size() / 2] : 0;
}

int fixUpdate(updates_t::value_type& update, rules_t& rules) {
  bool correct = true;

  auto is_valid_order = [&rules](int from, int to) -> bool {
    const auto& rule = rules[from];
    auto        res  = std::find(rule.begin(), rule.end(), to);
    return res != rule.end();
  };

  int jump = 0;
  for (int i = 1; i < update.size();) {
    int from = update[i - 1];
    int to   = update[i];
    if (!is_valid_order(from, to)) {
      jump    = i;
      correct = false;
      swap(update[i], update[i - 1]);
      if (i > 1)
        i--;
    } else {
      if (jump) {
        i    = jump;
        jump = 0;
      }
      i++;
    }
  }

  return (!correct) ? update[update.size() / 2] : 0;
}

int calcUpdates(rules_t& rules, updates_t& updates, bool fix) {
  int total = 0;

  for (auto& update : updates)
    total += (fix) ? fixUpdate(update, rules) : checkUpdate(update, rules);

  return total;
}

TEST(Day5, Part1Examples) {
  // clang-format off
  vector<pair<int, int>> pairs = {
    {47, 53}, // 0
    {97, 13}, // 1
    {97, 61}, // 2
    {97, 47}, // 3
    {75, 29}, // 4
    {61, 13}, // 5
    {75, 53}, // 6
    {29, 13}, // 7
    {97, 29}, // 8
    {53, 29}, // 9
    {61, 53}, // 10
    {97, 53}, // 11
    {61, 29}, // 12
    {47, 13}, // 13
    {75, 47}, // 14
    {97, 75}, // 15
    {47, 61}, // 16
    {75, 61}, // 17
    {47, 29}, // 18
    {75, 13}, // 19
    {53, 13}  // 20
  };
  // clang-format on

  rules_t rules;
  for (auto [l, r] : pairs)
    rules[l].push_back(r);

  updates_t updates = {
      {75, 47, 61, 53, 29},  // correct, mid = 61
      {97, 61, 53, 29, 13},  // correct, mid = 53
      {75, 29, 13},          // correct, mid = 29
      {75, 97, 47, 61, 53},  // incorrect 75->97 violates rule #15 97:15
      {61, 13, 29},          // incorrect 13->29 violates rules #7 29:13
      {97, 13, 75, 29, 47},  // incorrect 13->75 violates rule #19 75:13
                             // 61+53+29 = 143
  };

  int ans = calcUpdates(rules, updates, false);
  EXPECT_EQ(ans, 143);
}

TEST(Day5, Part1) {
  rules_t   rules;
  updates_t updates;
  readInput(rules, updates);

  int ans = calcUpdates(rules, updates, false);
  EXPECT_EQ(ans, 5064);
  cout << "Answer = " << ans << endl;
}

TEST(Day5, Part2Examples) {
  // clang-format off
  vector<pair<int, int>> pairs = {
    {47, 53}, // 0
    {97, 13}, // 1
    {97, 61}, // 2
    {97, 47}, // 3
    {75, 29}, // 4
    {61, 13}, // 5
    {75, 53}, // 6
    {29, 13}, // 7
    {97, 29}, // 8
    {53, 29}, // 9
    {61, 53}, // 10
    {97, 53}, // 11
    {61, 29}, // 12
    {47, 13}, // 13
    {75, 47}, // 14
    {97, 75}, // 15
    {47, 61}, // 16
    {75, 61}, // 17
    {47, 29}, // 18
    {75, 13}, // 19
    {53, 13}  // 20
  };
  // clang-format on

  rules_t rules;
  for (auto [l, r] : pairs)
    rules[l].push_back(r);

  updates_t updates = {
      {97, 13, 75, 29, 47},  // incorrect 13->75 violates rule #19 75:13
                             //     <----
                             // {97,75,13,29,47} swap 13,75
                             //        <----
                             // {97,75,29,13,47} swap 13,29
                             //           <----
                             // {97,75,29,47,13} swap 13,47
                             //        <----
                             // {97,75,47,29,13} swap 29,47, mid = 47
      {75, 47, 61, 53, 29},  // correct
      {97, 61, 53, 29, 13},  // correct
      {75, 29, 13},          // correct
      {75, 97, 47, 61, 53},  // incorrect 75->97 violates rule #15 97:15
                             // {97,75,47,61,53}, mid = 47
      {61, 13, 29},          // incorrect 13->29 violates rules #7 29:13
                             // {61,29,13}, mid = 29
                             // total = 47+29+47 = 123
  };

  int ans = calcUpdates(rules, updates, true);
  EXPECT_EQ(ans, 123);
}

TEST(Day5, Part2) {
  rules_t   rules;
  updates_t updates;
  readInput(rules, updates);

  int ans = calcUpdates(rules, updates, true);
  EXPECT_EQ(ans, 5152);
  cout << "Answer = " << ans << endl;
}