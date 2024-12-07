#include "common.h"

namespace day07 {

struct equation {
    uint64_t         target = 0;
    vector<uint32_t> nums;

    friend ostream& operator<<(ostream& o, const equation& eq) {
      o << format("target = {}, [", eq.target);
      for (int i = 0; i < eq.nums.size(); i++) {
        o << eq.nums[i];
        if (i != eq.nums.size() - 1)
          o << ",";
      }
      o << "]";
      return o;
    }
};

vector<equation> readInput() {
  vector<equation> equations;

  auto   f = ifstream(getInputFile(2024, 7));
  string s;
  try {
    while (getline(f, s)) {
      auto& eq = equations.emplace_back();

      auto p             = s.find(':');
      auto target_string = s.substr(0, p);
      p++;
      auto comps_string = s.substr(p, s.length() - p);

      eq.target  = stoull(target_string);
      auto parts = split(comps_string, ' ');
      for (auto p : parts)
        eq.nums.push_back(stoi(p));
    }
  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    std::cerr << s << '\n';
  }

  return equations;
};

uint64_t checks = 0;

bool is_valid(equation& eq) {
  unordered_map<uint32_t, int> history;

  uint64_t target       = eq.target;
  auto&    nums         = eq.nums;
  int      max_decision = 1 << (nums.size() - 1);

  auto check = [&](int decision) -> bool {
    uint64_t total = nums[0];

    for (int i = 1; i < nums.size(); i++) {
      uint32_t mask = 1 << ((nums.size() - 1) - i);

      checks++;

      uint32_t is_mull = decision & mask;
      if (is_mull)
        total *= nums[i];
      else
        total += nums[i];
    }

    return total == target;
  };

  for (int i = 0; i < max_decision; i++)
    if (check(i))
      return true;

  return false;
}

enum op {
  op_add,
  op_mull,
  op_concat
};

uint64_t concat(uint64_t a, uint32_t b) {
  int _b = b;
  while (_b) {
    _b /= 10;
    a *= 10;
  }
  uint64_t value = a + b;
  return value;
}

bool check_valid_concat(equation& eq, uint64_t curr_total, int i) {
  auto& nums = eq.nums;

  if (i == nums.size())
    return curr_total == eq.target;

  if (check_valid_concat(eq, curr_total + nums[i], i + 1))
    return true;
  if (check_valid_concat(eq, curr_total * nums[i], i + 1))
    return true;
  if (check_valid_concat(eq, concat(curr_total, nums[i]), i + 1))
    return true;
  return false;
}

bool is_valid_concat(equation& eq) {
  bool valid = check_valid_concat(eq, eq.nums[0], 1);
  return valid;
}

uint64_t calcValidEquations(vector<equation>& equations, bool concat) {
  uint64_t sum = 0;

  checks = 0;
  for (auto& eq : equations) {
    if (concat && is_valid_concat(eq))
      sum += eq.target;
    else if (is_valid(eq))
      sum += eq.target;
  }
  return sum;
}

TEST(Day7, Part1Examples) {
  vector<equation> example = {
      // clang-format off
      {190, {10, 19}},          // *
      {3267, {81, 40, 27}},     // *+ OR +*
      {83, {17, 5}},
      {156, {15, 6}},
      {7290, {6, 8, 6, 15}},
      {161011, {16, 10, 13}},
      {192, {17, 8, 14}},
      {21037, {9, 7, 18, 13}},
      {292, {11, 6, 16, 20}}
      // clang-format on
  };

  uint64_t answer = calcValidEquations(example, false);
  EXPECT_EQ(answer, 3749);
  cout << "Answer = " << answer << endl;
  cout << "Checks = " << checks << endl;
}

TEST(Day7, Part1) {
  vector<equation> equations = readInput();

  uint64_t answer = calcValidEquations(equations, false);
  EXPECT_EQ(answer, 4364915411363);
  cout << "Answer = " << answer << endl;
  cout << "Checks = " << checks << endl;
}

TEST(Day7, Part2Examples) {
  vector<equation> example = {
      // clang-format off
      {190, {10, 19}},
      {3267, {81, 40, 27}},
      {83, {17, 5}},
      {156, {15, 6}},
      {7290, {6, 8, 6, 15}},
      {161011, {16, 10, 13}},
      {192, {17, 8, 14}},
      {21037, {9, 7, 18, 13}},
      {292, {11, 6, 16, 20}}
      // clang-format on
  };

  uint64_t answer = calcValidEquations(example, true);
  EXPECT_EQ(answer, 11387);
  cout << "Answer = " << answer << endl;
  cout << "Checks = " << checks << endl;
}

TEST(Day7, Part2) {
  vector<equation> equations = readInput();

  uint64_t answer = calcValidEquations(equations, true);
  EXPECT_EQ(answer, 38322057216320);
  cout << "Answer = " << answer << endl;
  cout << "Checks = " << checks << endl;
}

}  // namespace day07