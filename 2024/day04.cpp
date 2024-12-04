#include "common.h"

void readInput(const string &filename, vector<string> &output)
{
    stringstream s;
    auto f = ifstream(filename);
    string tmp;
    while (getline(f, tmp))
        output.emplace_back(tmp);
}

int countXmas(const vector<string> &input)
{
    int cnt = 0;
    int h = input.size();
    int w = input[0].length();
    for (int j = 0; j < h; j++)
        for (int i = 0; i < w; i++)
            if (input[j][i] == 'X')
            {
                if (i >= 3 &&
                    input[j][i - 1] == 'M' &&
                    input[j][i - 2] == 'A' &&
                    input[j][i - 3] == 'S')
                    cnt++;
                if (i + 3 < w &&
                    input[j][i + 1] == 'M' &&
                    input[j][i + 2] == 'A' &&
                    input[j][i + 3] == 'S')
                    cnt++;
                if (j >= 3 &&
                    input[j - 1][i] == 'M' &&
                    input[j - 2][i] == 'A' &&
                    input[j - 3][i] == 'S')
                    cnt++;
                if (j + 3 < h &&
                    input[j + 1][i] == 'M' &&
                    input[j + 2][i] == 'A' &&
                    input[j + 3][i] == 'S')
                    cnt++;
                if (j >= 3 && i + 3 < w &&
                    input[j - 1][i + 1] == 'M' &&
                    input[j - 2][i + 2] == 'A' &&
                    input[j - 3][i + 3] == 'S')
                    cnt++;
                if (j + 3 < h && i + 3 < w &&
                    input[j + 1][i + 1] == 'M' &&
                    input[j + 2][i + 2] == 'A' &&
                    input[j + 3][i + 3] == 'S')
                    cnt++;
                if (j + 3 < h && i >= 3 &&
                    input[j + 1][i - 1] == 'M' &&
                    input[j + 2][i - 2] == 'A' &&
                    input[j + 3][i - 3] == 'S')
                    cnt++;
                if (j >= 3 && i >= 3 &&
                    input[j - 1][i - 1] == 'M' &&
                    input[j - 2][i - 2] == 'A' &&
                    input[j - 3][i - 3] == 'S')
                    cnt++;
            }
    return cnt;
}

int countCrossMas(const vector<string> &input)
{
    int cnt = 0;
    for (int j = 1; j < input.size() - 1; j++)
        for (int i = 1; i < input[0].length() - 1; i++)
            if (input[j][i] == 'A')
            {
                bool forward_mas =
                    // --                           ++
                    (input[j - 1][i - 1] == 'M' && input[j + 1][i + 1] == 'S') ||
                    (input[j - 1][i - 1] == 'S' && input[j + 1][i + 1] == 'M');
                bool back_mas =
                    // +-                           -+
                    (input[j + 1][i - 1] == 'M' && input[j - 1][i + 1] == 'S') ||
                    (input[j + 1][i - 1] == 'S' && input[j - 1][i + 1] == 'M');
                if (forward_mas && back_mas)
                    cnt++;
            }
    return cnt;
}

TEST(Day4, Part1Examples)
{
    vector<string> input = {
        // 0123456789
        "MMMSXXMASM", // 0
        "MSAMXMSMSA", // 1
        "AMXSXMAAMM", // 2
        "MSAMASMSMX", // 3
        "XMASAMXAMM", // 4
        "XXAMMXXAMA", // 5
        "SMSMSASXSS", // 6
        "SAXAMASAAA", // 7
        "MAMMMXMMMM", // 8
        "MXMXAXMASX"  // 9
    };

    EXPECT_EQ(countXmas(input), 18);
}

TEST(Day4, Part2Examples)
{
    vector<string> input = {
        // 0123456789
        "MMMSXXMASM", // 0
        "MSAMXMSMSA", // 1
        "AMXSXMAAMM", // 2
        "MSAMASMSMX", // 3
        "XMASAMXAMM", // 4
        "XXAMMXXAMA", // 5
        "SMSMSASXSS", // 6
        "SAXAMASAAA", // 7
        "MAMMMXMMMM", // 8
        "MXMXAXMASX"  // 9
    };

    EXPECT_EQ(countCrossMas(input), 9);
}

TEST(Day4, Part1)
{
    vector<string> input;
    readInput("input/day04.txt", input);
    int answer = countXmas(input);
    EXPECT_EQ(answer, 2370);
    cout << "Answer = " << answer << endl;
}

TEST(Day4, Part2)
{
    vector<string> input;
    readInput("input/day04.txt", input);
    int answer = countCrossMas(input);
    EXPECT_EQ(answer, 1908);
    cout << "Answer = " << answer << endl;
}