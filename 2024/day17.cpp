#include "test.h"
#include "util/string.h"
#include <queue>

using namespace std;

class Day17 : public aoc_2024 {
  public:
    vector<int> output;

    struct cpu_t {
        uint32_t a;
        uint32_t b;
        uint32_t c;
    } cpu;

    queue<int> program;

    void LoadInput(istringstream& input) override {
      string s;
      size_t offset;

      getline(input, s);
      offset = s.find(':') + 2;
      cpu.a  = stoi(s.substr(offset, s.length() - offset));
      getline(input, s);
      offset = s.find(':') + 2;
      cpu.b  = stoi(s.substr(offset, s.length() - offset));
      getline(input, s);
      offset = s.find(':') + 2;
      cpu.c  = stoi(s.substr(offset, s.length() - offset));

      getline(input, s);
      getline(input, s);
      offset     = s.find(":") + 2;
      auto parts = split(s.substr(offset, s.length() - offset), ',');
      for (const auto& p : parts)
        program.emplace(stoi(p));
    }

    string getOutputString() {
      stringstream s;
      for (int i = 0; i < output.size(); i++) {
        s << output[i];
        if (i < output.size() - 1)
          s << ',';
      }
      return s.str();
    }

    void execute() {
    }
};

TEST_F(Day17, Part1Example) {
  input = "Register A: 729\n"
          "Register B: 0\n"
          "Register C: 0\n"
          "\n"
          "Program: 0,1,5,4,3,0";
  SetUp();

  execute();

  string output = getOutputString();
  EXPECT_EQ(output, "4,6,3,5,6,3,5,2,1,0");
}

TEST_F(Day17, Part1) {
}