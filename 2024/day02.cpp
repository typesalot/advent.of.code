#include "common.h"

int _numSafeReportsWithTolerance(const vector<vector<int>>& reports);

void readInput(const string& input, vector<vector<int>>& reports) {
  auto   f = ifstream(input);
  string line;
  while (getline(f, line)) {
    auto         elems  = split(line, ' ');
    vector<int>& report = reports.emplace_back();
    for (auto e : elems)
      report.emplace_back(stoi(e));
  }
}

std::tuple<bool, int> isSafe(const vector<int>& report, int skip = -1) {
  bool safe = false;

  bool ltz     = false;
  bool ltz_set = false;
  int  i       = 0;
  int  j       = 0;

  for (i = 0; i < report.size() - 1; i++) {
    // Boundary conditions for skipping an element contribution; used in part2
    if (skip != -1) {
      if (i == skip)
        continue;
      j = i + 1;
      if (j == skip)
        j++;
      if (j == report.size())
        continue;
    } else
      j = i + 1;

    int grad = report[i] - report[j];

    // check1: gradient magnitude
    if (grad > 3 || grad < -3 || grad == 0)
      break;

    // check1: direction
    if (!ltz_set) {
      ltz     = grad < 0;
      ltz_set = true;
    } else if (ltz && grad > 0)
      break;
    else if (!ltz && grad < 0)
      break;
  }

  safe = (i == report.size() - 1);

  return {safe, i};
}

int numSafeReports(const string& input) {
  vector<vector<int>> reports;
  readInput(input, reports);

  int safe = 0;

  for (const auto& report : reports) {
    auto [is_safe, i] = isSafe(report);
    if (is_safe)
      safe++;
  }

  return safe;
}

int numSafeReportsWithTolerance(const string& input) {
  vector<vector<int>> reports;
  readInput(input, reports);
  return _numSafeReportsWithTolerance(reports);
}

int _numSafeReportsWithTolerance(const vector<vector<int>>& reports) {
  int safe = 0;

  int tmp;
  for (const auto& report : reports) {
    auto [is_safe, i] = isSafe(report);
    if (!is_safe) {
      // safety window = [i-1,i,i+1]
      // corner case: removing i-1, but only for i=1 when removing the 0th element might make a safe gradient
      if (i == 1)
        tie(is_safe, tmp) = isSafe(report, i - 1);

      // common case 1: removing i makes i-1 -> i+1 safe
      if (!is_safe)
        tie(is_safe, tmp) = isSafe(report, i);

      // common case 2: removing i+1 makes i-1 -> i safe
      if (!is_safe)
        tie(is_safe, tmp) = isSafe(report, i + 1);

      if (is_safe)
        safe++;
    } else
      safe++;
  }

  return safe;
}

TEST(Day2, Examples) {
  EXPECT_EQ(_numSafeReportsWithTolerance({{7, 6, 4, 2, 1}}), 1);  // Safe without removing any level.
  EXPECT_EQ(_numSafeReportsWithTolerance({{1, 2, 7, 8, 9}}), 0);  // Unsafe regardless of which level is removed.
  EXPECT_EQ(_numSafeReportsWithTolerance({{9, 7, 6, 2, 1}}), 0);  // Unsafe regardless of which level is removed.
  EXPECT_EQ(_numSafeReportsWithTolerance({{1, 3, 2, 4, 5}}), 1);  // Safe by removing the second level, 3.
  EXPECT_EQ(_numSafeReportsWithTolerance({{8, 6, 4, 4, 1}}), 1);  // Safe by removing the third level, 4.
  EXPECT_EQ(_numSafeReportsWithTolerance({{1, 3, 6, 7, 9}}), 1);  // Safe without removing any level.
  EXPECT_EQ(_numSafeReportsWithTolerance({{49, 52, 55, 57, 58, 60, 65}}), 1);
  EXPECT_EQ(_numSafeReportsWithTolerance({{81, 85, 88, 89, 91, 93}}), 1);
  EXPECT_EQ(_numSafeReportsWithTolerance({{47, 50, 48, 45, 42, 39, 38}}),
            1);  // Removing the first element makes safe gradient
}

TEST(Day2, Part1) {
  int answer = numSafeReports("input/day02.txt");
  EXPECT_EQ(answer, 220);
  cout << "Answer = " << answer << endl;
}

TEST(Day2, Part2) {
  int answer = numSafeReportsWithTolerance("input/day02.txt");
  EXPECT_EQ(answer, 296);
  cout << "Answer = " << answer << endl;
}