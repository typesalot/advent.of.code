#include "common.h"

void readInput(const string &filename, vector<string> &output)
{
    stringstream s;
    auto f = ifstream(filename);
    string tmp;
    while (getline(f, tmp))
        output.emplace_back(tmp);
}

int _countE(const vector<string> &input)
{
    int cnt = 0;
    for (int j = 0; j < input.size(); j++)
        for (int i = 0; i < input[0].length() - 3; i++)
            if (input[j][i] == 'X')
                if (
                    input[j][i + 1] == 'M' &&
                    input[j][i + 2] == 'A' &&
                    input[j][i + 3] == 'S')
                    cnt++;
    return cnt;
}

int _countW(const vector<string> &input)
{
    int cnt = 0;
    for (int j = 0; j < input.size(); j++)
        for (int i = 3; i < input[0].length(); i++)
            if (input[j][i] == 'X')
                if (
                    input[j][i - 1] == 'M' &&
                    input[j][i - 2] == 'A' &&
                    input[j][i - 3] == 'S')
                    cnt++;
    return cnt;
}

int _countN(const vector<string> &input)
{
    int cnt = 0;
    for (int j = 3; j < input.size(); j++)
        for (int i = 0; i < input[0].length(); i++)
            if (input[j][i] == 'X')
                if (
                    input[j - 1][i] == 'M' &&
                    input[j - 2][i] == 'A' &&
                    input[j - 3][i] == 'S')
                    cnt++;
    return cnt;
}

int _countS(const vector<string> &input)
{
    int cnt = 0;
    for (int j = 0; j < input.size() - 3; j++)
        for (int i = 0; i < input[0].length(); i++)
            if (input[j][i] == 'X')
                if (
                    input[j + 1][i] == 'M' &&
                    input[j + 2][i] == 'A' &&
                    input[j + 3][i] == 'S')
                    cnt++;
    return cnt;
}

int _countNE(const vector<string> &input)
{
    int cnt = 0;
    for (int j = 3; j < input.size(); j++)
        for (int i = 0; i < input[0].length() - 3; i++)
            if (input[j][i] == 'X')
                if (
                    input[j - 1][i + 1] == 'M' &&
                    input[j - 2][i + 2] == 'A' &&
                    input[j - 3][i + 3] == 'S')
                    cnt++;
    return cnt;
}

int _countSE(const vector<string> &input)
{
    int cnt = 0;
    for (int j = 0; j < input.size() - 3; j++)
        for (int i = 0; i < input[0].length() - 3; i++)
            if (input[j][i] == 'X')
                if (
                    input[j + 1][i + 1] == 'M' &&
                    input[j + 2][i + 2] == 'A' &&
                    input[j + 3][i + 3] == 'S')
                    cnt++;
    return cnt;
}

int _countSW(const vector<string> &input)
{
    int cnt = 0;
    for (int j = 0; j < input.size() - 3; j++)
        for (int i = 3; i < input[0].length(); i++)
            if (input[j][i] == 'X')
                if (
                    input[j + 1][i - 1] == 'M' &&
                    input[j + 2][i - 2] == 'A' &&
                    input[j + 3][i - 3] == 'S')
                    cnt++;
    return cnt;
}

int _countNW(const vector<string> &input)
{
    int cnt = 0;
    for (int j = 3; j < input.size(); j++)
        for (int i = 3; i < input[0].length(); i++)
            if (input[j][i] == 'X')
                if (
                    input[j - 1][i - 1] == 'M' &&
                    input[j - 2][i - 2] == 'A' &&
                    input[j - 3][i - 3] == 'S')
                    cnt++;
    return cnt;
}

int countXmas(const vector<string> &input)
{
    int total = 0;
    total += _countN(input);
    total += _countW(input);
    total += _countE(input);
    total += _countS(input);
    total += _countNE(input);
    total += _countSW(input);
    total += _countNW(input);
    total += _countSE(input);
    return total;
}

int countCrossMas(const vector<string>& input)
{
    int cnt = 0;
    for (int j = 1; j < input.size() - 1; j++)
        for (int i = 1; i < input[0].length() - 1; i++)
            if (input[j][i] == 'A')
            {
                bool forward_mas = 
                      // --                           ++
                    ( input[j-1][i-1] == 'M' && input[j+1][i+1] == 'S' ) ||
                    ( input[j-1][i-1] == 'S' && input[j+1][i+1] == 'M' );
                bool back_mas = 
                    // +-                           -+
                    ( input[j+1][i-1] == 'M' && input[j-1][i+1] == 'S' ) ||
                    ( input[j+1][i-1] == 'S' && input[j-1][i+1] == 'M' );
                if( forward_mas && back_mas )
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

    EXPECT_EQ(_countE(input), 3);
    EXPECT_EQ(_countW(input), 2);
    EXPECT_EQ(_countN(input), 2);
    EXPECT_EQ(_countS(input), 1);
    EXPECT_EQ(_countNE(input), 4);
    EXPECT_EQ(_countSE(input), 1);
    EXPECT_EQ(_countSW(input), 1);
    EXPECT_EQ(_countNW(input), 4);
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
    readInput("input/day04.txt",input);
    int answer = countXmas(input);
    EXPECT_EQ(answer, 2370);
    cout << "Answer = " << answer << endl;
}

TEST(Day4, Part2)
{
    vector<string> input;
    readInput("input/day04.txt",input);
    int answer = countCrossMas(input);
    EXPECT_EQ(answer, 1908);
    cout << "Answer = " << answer << endl;
}