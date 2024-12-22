#include "test.h"
#include "util/string.h"
#include <functional>
#include <source_location>
#include <vector>

using namespace std;

class Day17 : public aoc_2024 {
  public:
    vector<int> output;

    struct reg_t {
        uint32_t a;
        uint32_t b;
        uint32_t c;
        uint32_t ip = 0;
    } reg;

    vector<uint32_t> program;
    bool             terminated;

    using function_t = void (Day17::*)();
    array<function_t, 8> instructions;

    void LoadInput(istringstream& input) override {
      string s;
      size_t offset;

      getline(input, s);
      offset = s.find(':') + 2;
      reg.a  = stoi(s.substr(offset, s.length() - offset));
      getline(input, s);
      offset = s.find(':') + 2;
      reg.b  = stoi(s.substr(offset, s.length() - offset));
      getline(input, s);
      offset = s.find(':') + 2;
      reg.c  = stoi(s.substr(offset, s.length() - offset));

      getline(input, s);
      getline(input, s);
      offset     = s.find(":") + 2;
      auto parts = split(s.substr(offset, s.length() - offset), ',');
      program.resize(parts.size());
      for (int i = 0; i < parts.size(); i++)
        program[i] = stoi(parts[i]);

      // assign opcode functions
      instructions = {
          &Day17::adv,  // 0
          &Day17::bxl,  // 1
          &Day17::bst,  // 2
          &Day17::jnz,  // 3
          &Day17::bxc,  // 4
          &Day17::out,  // 5
          &Day17::bdv,  // 6
          &Day17::cdv   // 7
      };

      terminated = false;
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

    // 0 - division; A = A / ( 1 << combo ) truncated
    void adv() {
      if (g_config.debug)
        cout << source_location::current().function_name() << endl;

      uint32_t value = combo();
      reg.a          = reg.a / (1 << value);
    }

    // 1 - bitwise XOR; B ^ literal operand
    void bxl() {
      if (g_config.debug)
        cout << source_location::current().function_name() << endl;

      uint32_t value = literal();
      reg.b          = reg.b ^ value;
    }

    // 2 - bitwise mod8; B = combo % 8
    void bst() {
      if (g_config.debug)
        cout << source_location::current().function_name() << endl;

      uint32_t value = combo();
      reg.b          = value % 8;
    }

    // 3 - jump-not-zero; nop if A == 0 else ip = literal
    void jnz() {
      if (g_config.debug)
        cout << source_location::current().function_name() << endl;

      uint32_t value = literal();
      if (!reg.a)
        return;
      reg.ip = value;
    }

    // 4 - bitwise XOR (legacy); B = B ^ C
    void bxc() {
      if (g_config.debug)
        cout << source_location::current().function_name() << endl;

      literal();  // reads, but ignores ("legacy" reason is probably because adv reads a combo)
      reg.b = reg.b ^ reg.c;
    }

    // 5 - saves output; output.push( combo % 8 )
    void out() {
      if (g_config.debug)
        cout << source_location::current().function_name() << endl;

      uint32_t value = combo();
      value %= 8;
      output.push_back(value);
    }

    // 6 - division; B = A / ( 1 << combo ) truncated
    void bdv() {
      if (g_config.debug)
        cout << source_location::current().function_name() << endl;

      uint32_t value = combo();
      reg.b          = reg.a / (1 << value);
    }

    // 7 - division; C = A / ( 1 << combo ) truncated
    void cdv() {
      if (g_config.debug)
        cout << source_location::current().function_name() << endl;

      uint32_t value = combo();
      reg.c          = reg.a / (1 << value);
    }

    // read and increment program value
    uint32_t read() {
      // shouldn't happen in a well-formed binary
      if (reg.ip >= program.size()) {
        assert(false);
        terminated = true;
        return 0;
      }
      return program[reg.ip++];
    }

    // combo operand; [0-3] literal + [4-7] indirect
    uint32_t combo() {
      uint32_t value = read();
      if (0 <= value && value <= 3)
        return value;
      if (value == 4)
        return reg.a;
      if (value == 5)
        return reg.b;
      if (value == 6)
        return reg.c;
      // 7 will not appear in valid programs
      assert(false);
      return 0;
    }

    // literal operand
    uint32_t literal() {
      return read();
    }

    void execute() {
      while (!terminated) {
        int start = reg.ip;

        int        opcode = read();
        function_t inst   = instructions[opcode];

        (this->*inst)();

        int end = reg.ip;
        if (inst != &Day17::jnz)
          assert(end - start == 2);

        if (reg.ip >= program.size())
          terminated = true;
      }
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
  execute();

  string output = getOutputString();
  EXPECT_EQ(output, "6,5,7,4,5,7,3,1,0");
}