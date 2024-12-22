#include "test.h"
#include "util/config.h"
#include "util/point.h"
#include "util/term.h"
#include <fstream>
#include <gtest/gtest.h>
#include <string>

using namespace std;

class Day15 : public ::testing::Test {
    using point = point_int;

  protected:
    vector<term::string> map;
    string               movements;
    point                robot;

    void SetUp() override {
      auto fname = getInputFile(2024, 15);
      auto f     = ifstream(fname);
      readInput(f);
    };

    void print_map() {
      if (!g_config.debug)
        return;

      static bool move = false;
      if (move)
        cout << term::cursor::move_up(map.size());
      move = true;
      for (const auto& m : map)
        cout << m << endl;
    }

    template <typename src_t>
    void readInput(src_t& src) {
      map.clear();
      movements.clear();

      bool onmap = true;

      string s;
      while (getline(src, s)) {
        if (s == "") {
          onmap = false;
          continue;
        }

        if (onmap) {
          uint32_t x = s.find('@');
          if (x != -1) {
            robot.x = x;
            robot.y = map.size();
          }

          map.push_back(s);
        } else
          movements += s;
      }
    }

    void move(const point& offset) {
      auto adjusted = robot + offset;
      char c        = map[adjusted.y][adjusted.x];
      if (c == '.') {
        swap(map[robot.y][robot.x], map[adjusted.y][adjusted.x]);
        robot = adjusted;
        print_map();
      } else if (c == 'O') {
        bool  found   = false;
        point vacancy = adjusted;
        char  v_char  = c;

        // scan at increments of offset
        while (v_char != '#' && !found) {
          vacancy = vacancy + offset;
          v_char  = map[vacancy.y][vacancy.x];
          if (v_char == '.')
            found = true;
        }

        // swap back to robot location
        if (found) {
          while (vacancy != robot) {
            auto next = vacancy - offset;
            swap(map[vacancy.y][vacancy.x], map[next.y][next.x]);
            vacancy = next;

            print_map();
          }

          robot = vacancy + offset;
        }

      } else {
        assert(c == '#');
      }
    }

    int getBoxesGpsTotal() {
      int total = 0;

      print_map();

      for (char dir : movements) {
        if (dir == '<')
          move(point{-1, 0});
        else if (dir == '>')
          move(point{1, 0});
        else if (dir == '^')
          move(point{0, -1});
        else
          move(point{0, 1});
      }

      // calculate gps positions
      for (uint32_t j = 0; j < map.size(); j++)
        for (uint32_t i = 0; i < map[0].length(); i++)
          if (map[j][i] == 'O')
            total += 100 * j + i;

      return total;
    }
};

template <>
void Day15::readInput(string& s) {
  auto input = istringstream(s);
  readInput(input);
}

TEST_F(Day15, PushLeft) {
  string s;

  s = string{"########\n"
             "#...O@.#\n"
             "########\n"
             "\n"
             "<<<<<<\n"};
  readInput(s);
  EXPECT_EQ(getBoxesGpsTotal(), 101);

  s = string{"########\n"
             "#..O.@.#\n"
             "########\n"
             "\n"
             "<<<<<<\n"};
  readInput(s);
  EXPECT_EQ(getBoxesGpsTotal(), 101);

  s = string{"########\n"
             "#.OO.@.#\n"
             "########\n"
             "\n"
             "<<<<<<\n"};
  readInput(s);
  EXPECT_EQ(getBoxesGpsTotal(), 101 + 102);

  s = string{"########\n"
             "#OOOO@.#\n"
             "########\n"
             "\n"
             "<<<<<<\n"};
  readInput(s);
  EXPECT_EQ(getBoxesGpsTotal(), 101 + 102 + 103 + 104);
}

TEST_F(Day15, PushRight) {
  string s;

  s = string{"########\n"
             "#..@O..#\n"
             "########\n"
             "\n"
             ">>>>>>\n"};
  readInput(s);
  EXPECT_EQ(getBoxesGpsTotal(), 106);

  s = string{"########\n"
             "#.@O...#\n"
             "########\n"
             "\n"
             ">>>>>>\n"};
  readInput(s);
  EXPECT_EQ(getBoxesGpsTotal(), 106);

  s = string{"########\n"
             "#@.O.O.#\n"
             "########\n"
             "\n"
             ">>>>>>\n"};
  readInput(s);
  EXPECT_EQ(getBoxesGpsTotal(), 106 + 105);

  s = string{"########\n"
             "#.@.OO.#\n"
             "########\n"
             "\n"
             ">>>>>>\n"};
  readInput(s);
  EXPECT_EQ(getBoxesGpsTotal(), 106 + 105);

  s = string{"########\n"
             "#.@OOOO#\n"
             "########\n"
             "\n"
             ">>>>>>\n"};
  readInput(s);
  EXPECT_EQ(getBoxesGpsTotal(), 106 + 105 + 104 + 103);
}

TEST_F(Day15, PushUp) {
  string s;

  s = string{"#####\n"
             "#...#\n"
             "#...#\n"
             "#...#\n"
             "#.O.#\n"
             "#.@.#\n"
             "#...#\n"
             "#####\n"
             "\n"
             "^^^^^^\n"};
  readInput(s);
  EXPECT_EQ(getBoxesGpsTotal(), 102);

  s = string{"#####\n"
             "#...#\n"
             "#...#\n"
             "#.O.#\n"
             "#...#\n"
             "#.@.#\n"
             "#...#\n"
             "#####\n"
             "\n"
             "^^^^^^\n"};
  readInput(s);
  EXPECT_EQ(getBoxesGpsTotal(), 102);

  s = string{"#####\n"
             "#...#\n"
             "#.O.#\n"
             "#...#\n"
             "#.O.#\n"
             "#.@.#\n"
             "#...#\n"
             "#####\n"
             "\n"
             "^^^^^^\n"};
  readInput(s);
  EXPECT_EQ(getBoxesGpsTotal(), 102 + 202);

  s = string{"#####\n"
             "#.O.#\n"
             "#.O.#\n"
             "#.O.#\n"
             "#.O.#\n"
             "#.@.#\n"
             "#...#\n"
             "#####\n"
             "\n"
             "^^^^^^\n"};
  readInput(s);
  EXPECT_EQ(getBoxesGpsTotal(), 102 + 202 + 302 + 402);
}

TEST_F(Day15, Part1Example) {
  string s = {"##########\n"
              "#..O..O.O#\n"
              "#......O.#\n"
              "#.OO..O.O#\n"
              "#..O@..O.#\n"
              "#O#..O...#\n"
              "#O..O..O.#\n"
              "#.OO.O.OO#\n"
              "#....O...#\n"
              "##########\n"
              "\n"
              "<vv>^<v^>v>^vv^v>v<>v^v<v<^vv<<<^><<><>>v<vvv<>^v^>^<<<><<v<<<v^vv^v>^\n"
              "vvv<<^>^v^^><<>>><>^<<><^vv^^<>vvv<>><^^v>^>vv<>v<<<<v<^v>^<^^>>>^<v<v\n"
              "><>vv>v^v^<>><>>>><^^>vv>v<^^^>>v^v^<^^>v^^>v^<^v>v<>>v^v^<v>v^^<^^vv<\n"
              "<<v<^>>^^^^>>>v^<>vvv^><v<<<>^^^vv^<vvv>^>v<^^^^v<>^>vvvv><>>v^<<^^^^^\n"
              "^><^><>>><>^^<<^^v>>><^<v>^<vv>>v>>>^v><>^v><<<<v>>v<v<v>vvv>^<><<>^><\n"
              "^>><>^v<><^vvv<^^<><v<<<<<><^v<<<><<<^^<v<^^^><^>>^<v^><<<^>>^v<v^v<v^\n"
              ">^>>^v>vv>^<<^v<>><<><<v<<v><>v<^vv<<<>^^v^>^^>>><<^v>>v^v><^^>>^<>vv^\n"
              "<><^^>^^^<><vvvvv^v<v<<>^v<v>v<<^><<><<><<<^^<<<^<<>><<><^^^>^^<>^>v<>\n"
              "^^>vv<^v^v<vv>^<><v<^v>^^^>>>^^vvv^>vvv<>>>^<^>>>>>^<<^v>^vvv<>^<><<v>\n"
              "v^^>>><<^^<>>^v^<v^vv<>v^<<>^<^v^v><^<<<><<^<v><v<>vv>>v><v^<vv<>v^<<^\n"};

  readInput(s);

  int total = getBoxesGpsTotal();
  EXPECT_EQ(total, 10092);
}

TEST_F(Day15, Part1) {
  int total = getBoxesGpsTotal();
  EXPECT_EQ(total, 1511865);
}