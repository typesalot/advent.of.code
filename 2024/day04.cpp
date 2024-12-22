#include "test.h"

using namespace std;

class Day4 : public aoc_2024 {
  protected:
    vector<string> puzzle;

    void LoadInput(istringstream& input) override {
      string s;
      while (getline(input, s))
        puzzle.emplace_back(s);
    }

    int countXmas() {
      int cnt = 0;
      int h   = puzzle.size();
      int w   = puzzle[0].length();

      for (int j = 0; j < h; j++)
        for (int i = 0; i < w; i++)
          if (puzzle[j][i] == 'X') {
            if (i >= 3 && puzzle[j][i - 1] == 'M' && puzzle[j][i - 2] == 'A' && puzzle[j][i - 3] == 'S')
              cnt++;
            if (i + 3 < w && puzzle[j][i + 1] == 'M' && puzzle[j][i + 2] == 'A' && puzzle[j][i + 3] == 'S')
              cnt++;
            if (j >= 3 && puzzle[j - 1][i] == 'M' && puzzle[j - 2][i] == 'A' && puzzle[j - 3][i] == 'S')
              cnt++;
            if (j + 3 < h && puzzle[j + 1][i] == 'M' && puzzle[j + 2][i] == 'A' && puzzle[j + 3][i] == 'S')
              cnt++;
            if (j >= 3 && i + 3 < w && puzzle[j - 1][i + 1] == 'M' && puzzle[j - 2][i + 2] == 'A' &&
                puzzle[j - 3][i + 3] == 'S')
              cnt++;
            if (j + 3 < h && i + 3 < w && puzzle[j + 1][i + 1] == 'M' && puzzle[j + 2][i + 2] == 'A' &&
                puzzle[j + 3][i + 3] == 'S')
              cnt++;
            if (j + 3 < h && i >= 3 && puzzle[j + 1][i - 1] == 'M' && puzzle[j + 2][i - 2] == 'A' &&
                puzzle[j + 3][i - 3] == 'S')
              cnt++;
            if (j >= 3 && i >= 3 && puzzle[j - 1][i - 1] == 'M' && puzzle[j - 2][i - 2] == 'A' &&
                puzzle[j - 3][i - 3] == 'S')
              cnt++;
          }

      return cnt;
    }

    int countCrossMas() {
      int cnt = 0;
      for (int j = 1; j < puzzle.size() - 1; j++)
        for (int i = 1; i < puzzle[0].length() - 1; i++)
          if (puzzle[j][i] == 'A') {
            bool forward_mas =
                // --                           ++
                (puzzle[j - 1][i - 1] == 'M' && puzzle[j + 1][i + 1] == 'S') ||
                (puzzle[j - 1][i - 1] == 'S' && puzzle[j + 1][i + 1] == 'M');
            bool back_mas =
                // +-                           -+
                (puzzle[j + 1][i - 1] == 'M' && puzzle[j - 1][i + 1] == 'S') ||
                (puzzle[j + 1][i - 1] == 'S' && puzzle[j - 1][i + 1] == 'M');
            if (forward_mas && back_mas)
              cnt++;
          }
      return cnt;
    }
};

TEST_F(Day4, Part1Examples) {
  input =
      // 0123456789
      "MMMSXXMASM\n"  // 0
      "MSAMXMSMSA\n"  // 1
      "AMXSXMAAMM\n"  // 2
      "MSAMASMSMX\n"  // 3
      "XMASAMXAMM\n"  // 4
      "XXAMMXXAMA\n"  // 5
      "SMSMSASXSS\n"  // 6
      "SAXAMASAAA\n"  // 7
      "MAMMMXMMMM\n"  // 8
      "MXMXAXMASX";   // 9
  SetUp();

  EXPECT_EQ(countXmas(), 18);
}

TEST_F(Day4, Part1) {
  int answer = countXmas();
  EXPECT_EQ(answer, 2370);
}

TEST_F(Day4, Part2Examples) {
  input =
      // 0123456789
      "MMMSXXMASM\n"  // 0
      "MSAMXMSMSA\n"  // 1
      "AMXSXMAAMM\n"  // 2
      "MSAMASMSMX\n"  // 3
      "XMASAMXAMM\n"  // 4
      "XXAMMXXAMA\n"  // 5
      "SMSMSASXSS\n"  // 6
      "SAXAMASAAA\n"  // 7
      "MAMMMXMMMM\n"  // 8
      "MXMXAXMASX";   // 9
  SetUp();

  EXPECT_EQ(countCrossMas(), 9);
}

TEST_F(Day4, Part2) {
  int answer = countCrossMas();
  EXPECT_EQ(answer, 1908);
}
