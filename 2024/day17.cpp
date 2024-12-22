#include "test.h"
#include "util/string.h"
#include <format>
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

        void print() {
          cout << format("{:>8} = {:10} {:032b}\n", "A", a, a);
          cout << format("{:>8} = {:10} {:032b}\n", "B", b, b);
          cout << format("{:>8} = {:10} {:032b}\n", "C", c, c);
        }
    } reg;

    vector<uint32_t> program;
    bool             terminated;

    bool find_copy = false;
    bool match     = false;

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

    string getProgramString() {
      stringstream s;
      for (int i = 0; i < program.size(); i++) {
        s << program[i];
        if (i < program.size() - 1)
          s << ',';
      }
      return s.str();
    }

    void reset() {
      output.clear();
      reg.a      = 0;
      reg.b      = 0;
      reg.c      = 0;
      reg.ip     = 0;
      match      = false;
      terminated = false;
    };

    // 0 - division; A = A / ( 1 << combo ) truncated
    void adv() {
      uint32_t value = combo();

      if (g_config.debug) {
        auto name = source_location::current().function_name();
        cout << name << format(" -> A = A / ( 1 << {} ) = {}\n", value, reg.a / (1 << value));
        cout << format("{:>8} = {:10} {:032b}\n", "A", reg.a, reg.a);
        cout << format("{:>8} = {:10} {:032b}\n", "Value", value, value);
        cout << format("{:>8} = {:10} {:032b}\n\n", "DIV", reg.a / (1 << value), reg.a / (1 << value));
      }

      reg.a = reg.a / (1 << value);

      if (g_config.debug)
        reg.print();
    }

    // 1 - bitwise XOR; B ^ literal operand
    void bxl() {
      uint32_t value = literal();

      if (g_config.debug) {
        auto name = source_location::current().function_name();
        cout << name << format(" -> B = B ^ {} = {}\n", value, reg.b ^ value);
        cout << format("{:>8} = {:10} {:032b}\n", "B", reg.b, reg.b);
        cout << format("{:>8} = {:10} {:032b}\n", "Value", value, value);
        cout << format("{:>8} = {:10} {:032b}\n\n", "XOR", reg.b ^ value, reg.b ^ value);
      }

      reg.b = reg.b ^ value;

      if (g_config.debug)
        reg.print();
    }

    // 2 - bitwise mod8; B = combo % 8
    void bst() {
      if (g_config.debug) {
        auto combo_str = combo_string();
        auto value     = combo();
        auto name      = source_location::current().function_name();
        cout << name << format(" -> B = {} % 8 -> get the lower 3 bits of {}\n", combo_str, combo_str);
        cout << format("{:>8} = {:10} {:032b}\n", combo_str, value, value);
        cout << format("{:>8} = {:10} {:032b}\n", "8", 8, 8);
        cout << format("{:>8} = {:10} {:032b}\n\n", "%", value % 8, value % 8);
        reg.ip--;
      }

      uint32_t value = combo();
      reg.b          = value % 8;

      if (g_config.debug)
        reg.print();
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
      if (g_config.debug) {
        auto name = source_location::current().function_name();
        cout << name << format(" -> B = B ^ C\n");
        cout << format("{:>8} = {:10} {:032b}\n", "B", reg.b, reg.b);
        cout << format("{:>8} = {:10} {:032b}\n", "C", reg.c, reg.c);
        cout << format("{:>8} = {:10} {:032b}\n\n", "XOR", reg.b ^ reg.c, reg.b ^ reg.c);
      }

      literal();  // reads, but ignores ("legacy" reason is probably because adv reads a combo)
      reg.b = reg.b ^ reg.c;

      if (g_config.debug)
        reg.print();
    }

    // 5 - saves output; output.push( combo % 8 )
    void out() {
      uint32_t value = combo();

      if (g_config.debug) {
        auto name = source_location::current().function_name();
        cout << name << format(" -> Value % 8\n");
        cout << format("{:>8} = {:10} {:032b}\n", "Value", value, value);
        cout << format("{:>8} = {:10} {:032b}\n", "% 8", value % 8, value % 8);
      }

      value %= 8;

      if (find_copy) {
        uint32_t i = output.size();
        match      = (i < program.size()) ? value == program[i] : false;
        if (!match)
          return;
      }

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
      uint32_t value = combo();

      if (g_config.debug) {
        auto name = source_location::current().function_name();
        cout << name << format(" -> C = A / ( 1 << {} ) = {}\n", value, reg.a / (1 << value));
        cout << format("{:>8} = {:10} {:032b}\n", "A", reg.a, reg.a);
        cout << format("{:>8} = {:10} {:032b}\n", "Value", value, value);
        cout << format("{:>8} = {:10} {:032b}\n\n", "DIV", reg.a / (1 << value), reg.a / (1 << value));
      }

      reg.c = reg.a / (1 << value);

      if (g_config.debug)
        reg.print();
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

    string combo_string() {
      uint32_t value = program[reg.ip];
      if (value == 0)
        return "0";
      if (value == 1)
        return "1";
      if (value == 2)
        return "2";
      if (value == 3)
        return "3";
      if (value == 4)
        return "A";
      if (value == 5)
        return "B";
      if (value == 6)
        return "C";
      return "";
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

        if (g_config.debug)
          cout << endl;
        (this->*inst)();

        int end = reg.ip;
        if (inst != &Day17::jnz)
          assert(end - start == 2);

        if (reg.ip >= program.size())
          terminated = true;
      }
    }

    uint32_t findCopy() {
      find_copy = true;

      uint32_t a = -1;
      while (!match) {
        reg.a = ++a;
        execute();

        match &= program.size() == output.size();
        if (!match)
          reset();
      }

      return a;
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

TEST_F(Day17, Part2Example) {
  input = "Register A: 2024\n"
          "Register B: 0\n"
          "Register C: 0\n"
          "\n"
          "Program: 0,3,5,4,3,0";
  SetUp();

  uint32_t a = findCopy();

  string output  = getOutputString();
  string program = getProgramString();
  EXPECT_EQ(output, program);

  EXPECT_EQ(a, 117440);
}

TEST_F(Day17, Part2) {
  uint32_t a = findCopy();

  string output  = getOutputString();
  string program = getProgramString();
  EXPECT_EQ(output, program);

  EXPECT_EQ(a, 117440);
}