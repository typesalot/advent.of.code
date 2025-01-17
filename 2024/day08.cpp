#include "test.h"
#include "util/point.h"
#include <format>

using namespace std;

class Day8 : public ::testing::Test {
    using point = point_uint;

  protected:
    vector<string>      map;
    vector<vector<int>> colors;
    rect_uint           bounds;

    void SetUp() override {
      readInput();
    }

    void readInput() {
      auto   fname = getInputFile(2024, 8);
      auto   f     = ifstream(fname);
      string s;
      while (getline(f, s))
        map.emplace_back(s);
      colors.resize(map.size());
      for (auto& c : colors)
        c.resize(map[0].length());
    };

    void printMap() {
      static bool move = false;
      if (move) {
        for (const auto& r : map)
          cout << "\033[A";
      }
      move = true;

      for (int j = 0; j < map.size(); j++) {
        cout << "\033[0m  ";
        for (int i = 0; i < map[0].length(); i++)
          cout << format("\033[{}m{}", colors[j][i], map[j][i]);
        cout << endl;
      }
    }

    void markAntinode(const point& p) {
      if (map[p.y][p.x] == '.')
        map[p.y][p.x] = '#';
      colors[p.y][p.x] = 41;
    }

    void markStation(const point& p) {
      colors[p.y][p.x] = 44;
    }

    void clearColors() {
      for (auto& r : colors)
        for (auto& c : r)
          c = 0;
    }

    int numAntinodes(bool resonate) {
      int count = 0;

      bounds.top_left     = {0, 0};
      bounds.bottom_right = {static_cast<uint32_t>(map[0].length() - 1), static_cast<uint32_t>(map.size() - 1)};

      set<point> antinodes;

      // enumerate antennas
      unordered_map<char, vector<point>> attenas;
      for (uint32_t j = 0; j < map.size(); j++) {
        for (uint32_t i = 0; i < map[j].length(); i++) {
          char type = map[j][i];
          if (type == '.')
            continue;
          attenas[type].emplace_back(i, j);
          if (resonate)
            antinodes.insert(point{i, j});
        }
      }

      for (const auto& [type, locations] : attenas) {
        for (int i = 0; i < locations.size(); i++)
          for (int j = i + 1; j < locations.size(); j++) {
            const auto& p1 = locations[i];
            const auto& p2 = locations[j];

            auto delta = p2 - p1;

            auto p3 = p1 - delta;
            if (resonate) {
              while (bounds.in_bounds(p3)) {
                antinodes.insert(p3);
                p3 = p3 - delta;
              }
            } else if (bounds.in_bounds(p3))
              antinodes.insert(p3);

            auto p4 = p2 + delta;
            if (resonate) {
              while (bounds.in_bounds(p4)) {
                antinodes.insert(p4);
                p4 = p4 + delta;
              }
            } else if (bounds.in_bounds(p4))
              antinodes.insert(p4);
          }
      }

      count = antinodes.size();
      return count;
    }
};

TEST_F(Day8, Part1Example) {
  map = {
      // clang-format off
   //0123456789AB
    "............", // 0
    "........0...", // 1
    ".....0......", // 2
    ".......0....", // 3
    "....0.......", // 4
    "......A.....", // 5
    "............", // 6
    "............", // 7
    "........A...", // 8
    ".........A..", // 9
    "............", // A
    "............"  // B
      // clang-format on
  };

  int answer = numAntinodes(false);
  EXPECT_EQ(answer, 14);
}

TEST_F(Day8, Part1) {
  int answer = numAntinodes(false);
  EXPECT_EQ(answer, 222);
}

TEST_F(Day8, Part2Example) {
  map = {
      // clang-format off
   //0123456789AB
    "............", // 0
    "........0...", // 1
    ".....0......", // 2
    ".......0....", // 3
    "....0.......", // 4
    "......A.....", // 5
    "............", // 6
    "............", // 7
    "........A...", // 8
    ".........A..", // 9
    "............", // A
    "............"  // B
      // clang-format on
  };

  int answer = numAntinodes(true);
  EXPECT_EQ(answer, 34);
}

TEST_F(Day8, Part2) {
  int answer = numAntinodes(true);
  EXPECT_EQ(answer, 884);
}
