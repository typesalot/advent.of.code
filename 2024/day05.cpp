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

void readInput(vector<pair<int, int>>& rules, updates_t& updates) {
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
      rules.emplace_back(before, after);
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

  int jump = 1;
  for (int i = 1; i < update.size();) {
    int from = update[i - 1];
    int to   = update[i];
    if (!is_valid_order(from, to)) {
      jump    = max(jump, i);
      correct = false;
      swap(update[i], update[i - 1]);
      if (i > 1)
        i--;
    } else {
      if (i < jump)
        i = jump;
      i++;
    }
  }

  return (!correct) ? update[update.size() / 2] : 0;
}

// adapted from: https://github.com/00Greenwood/AdventOfCode2024/blob/main/Solutions/Day5.cxx
int fixUpdate2(updates_t::value_type& update, rules_t& rules) {
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

  if (!correct)
    std::sort(update.begin(), update.end(), is_valid_order);

  return (!correct) ? update[update.size() / 2] : 0;
}

// source: https://github.com/PeterCullenBurbery/advent-of-code-002/blob/main/C%2B%2B-023/2024-005.cpp
bool is_update_ordered(const std::vector<int>& update, const std::vector<std::pair<int, int>>& rules) {
  std::unordered_map<int, int> index_map;

  for (size_t i = 0; i < update.size(); ++i) {
    index_map[update[i]] = static_cast<int>(i);
  }

  for (const auto& rule : rules) {
    int x = rule.first;
    int y = rule.second;
    if (index_map.find(x) != index_map.end() && index_map.find(y) != index_map.end()) {
      if (index_map[x] > index_map[y]) {
        return false;
      }
    }
  }

  return true;
}

struct topological_sort_update_timers_t {
    int graph_init  = 0;
    int graph_build = 0;
    int queue_init  = 0;
    int top_sort    = 0;

    void clear() {
      memset(this, 0, sizeof(topological_sort_update_timers_t));
    }
} topological_sort_update_timers;

// source: https://github.com/PeterCullenBurbery/advent-of-code-002/blob/main/C%2B%2B-023/2024-005.cpp
std::vector<int> topological_sort_update(const std::vector<int>&                 update,
                                         const std::vector<std::pair<int, int>>& rules) {
  std::unordered_map<int, std::vector<int>> graph;
  std::unordered_map<int, int>              in_degree;
  std::unordered_set<int>                   nodes(update.begin(), update.end());

  timer t;

  // Initialize graph and in-degree
  t.start();
  for (const auto& node : nodes) {
    graph[node]     = std::vector<int>();
    in_degree[node] = 0;
  }
  topological_sort_update_timers.graph_init += t.ms();

  // Build the graph based on rules
  t.start();
  for (const auto& rule : rules) {
    int x = rule.first;
    int y = rule.second;
    if (nodes.find(x) != nodes.end() && nodes.find(y) != nodes.end()) {
      graph[x].push_back(y);
      in_degree[y]++;
    }
  }
  topological_sort_update_timers.graph_build += t.ms();

  // Initialize queue with nodes having in-degree 0
  t.start();
  std::queue<int> q;
  for (const auto& node : nodes) {
    if (in_degree[node] == 0) {
      q.push(node);
    }
  }
  topological_sort_update_timers.queue_init += t.ms();

  t.start();
  std::vector<int> sorted_update;
  while (!q.empty()) {
    int current = q.front();
    q.pop();
    sorted_update.push_back(current);

    for (const auto& neighbor : graph[current]) {
      in_degree[neighbor]--;
      if (in_degree[neighbor] == 0) {
        q.push(neighbor);
      }
    }
  }
  topological_sort_update_timers.top_sort += t.ms();

  // Check if topological sort was possible (i.e., no cycles)
  if (sorted_update.size() != nodes.size()) {
    // Cycle detected or unable to sort
    return std::vector<int>();
  }

  return sorted_update;
}

int calcUpdates(rules_t& rules, updates_t& updates, bool fix) {
  int total = 0;

  for (auto& update : updates)
    total += (fix) ? fixUpdate2(update, rules) : checkUpdate(update, rules);

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

/*
[ RUN      ] Day5.is_update_ordered
        rule count = 1176
     updates count = 194
        read input = 2ms
  checking updates = 7ms
[       OK ] Day5.is_update_ordered (9 ms)
*/
TEST(Day5, is_update_ordered) {
  timer t;

  vector<pair<int, int>> rules;
  updates_t              updates;
  readInput(rules, updates);
  cout << "        rule count = " << rules.size() << endl;
  cout << "     updates count = " << updates.size() << endl;
  cout << "        read input = " << t << "ms" << endl;

  t.start();
  for (const auto& update : updates) {
    if (!is_update_ordered(update, rules)) {
    }
  }
  cout << "  checking updates = " << t << "ms" << endl;
}

// [       OK ] Day5.topological_sort (21 ms)
TEST(Day5, topological_sort) {
  vector<pair<int, int>> rules;
  updates_t              updates;
  readInput(rules, updates);

  std::vector<int> incorrect_middle_pages;

  long long sum_incorrect_middle_pages = 0;
  auto      get_middle_page            = [](const std::vector<int>& update) { return update[update.size() / 2]; };

  for (const auto& update : updates) {
    if (!is_update_ordered(update, rules)) {
      std::vector<int> corrected_update = topological_sort_update(update, rules);
      sum_incorrect_middle_pages += get_middle_page(corrected_update);
    }
  }

  cout << "   graph_init = " << topological_sort_update_timers.graph_init << "ms" << endl;
  cout << "  graph_build = " << topological_sort_update_timers.graph_build << "ms" << endl;
  cout << "   queue_init = " << topological_sort_update_timers.queue_init << "ms" << endl;
  cout << "     top_sort = " << topological_sort_update_timers.top_sort << "ms" << endl;
  topological_sort_update_timers.clear();

  EXPECT_EQ(sum_incorrect_middle_pages, 5152);
}

// [       OK ] Day5.Part2 (3 ms)
TEST(Day5, Part2) {
  rules_t   rules;
  updates_t updates;
  readInput(rules, updates);

  int ans = calcUpdates(rules, updates, true);
  EXPECT_EQ(ans, 5152);
  cout << "Answer = " << ans << endl;
}