#include "test.h"
#include "util/bits.h"
#include "util/string.h"
#include "util/term.h"
#include <bitset>
#include <format>
#include <functional>
#include <queue>
#include <source_location>
#include <vector>

using namespace std;

class Day17 : public aoc_2024 {
  public:
    vector<int> output;

    struct reg_t {
        uint64_t a;
        uint64_t b;
        uint64_t c;
        uint64_t ip = 0;
    } reg;

    vector<uint64_t> program;
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

    string getProgramString() {
      stringstream s;
      for (int i = 0; i < program.size(); i++) {
        s << program[i];
        if (i < program.size() - 1)
          s << ',';
      }
      return s.str();
    }

    // 0 - division; A = A / ( 1 << combo ) truncated
    // A = A >> combo()
    void adv() {
      uint64_t value = combo();
      reg.a          = reg.a / (1 << value);
    }

    // 1 - bitwise XOR; B ^ literal operand
    void bxl() {
      uint64_t value = literal();
      reg.b          = reg.b ^ value;
    }

    // 2 - bitwise mod8; B = combo % 8
    void bst() {
      uint64_t value = combo();
      reg.b          = value % 8;
    }

    // 3 - jump-not-zero; nop if A == 0 else ip = literal
    void jnz() {
      uint64_t value = literal();
      if (!reg.a)
        return;
      reg.ip = value;
    }

    // 4 - bitwise XOR (legacy); B = B ^ C
    void bxc() {
      literal();
      reg.b = reg.b ^ reg.c;
    }

    // 5 - saves output; output.push( combo % 8 )
    void out() {
      uint64_t value = combo();
      value %= 8;

      output.push_back(value);
    }

    // 6 - division; B = A / ( 1 << combo ) truncated
    // B = A >> combo()
    void bdv() {
      uint64_t value = combo();
      reg.b          = reg.a / (1 << value);
    }

    // 7 - division; C = A / ( 1 << combo ) truncated
    // C = A >> combo()
    void cdv() {
      uint64_t value = combo();
      reg.c          = reg.a / (1 << value);
    }

    // read and increment program value
    uint64_t read() {
      // shouldn't happen in a well-formed binary
      if (reg.ip >= program.size()) {
        assert(false);
        terminated = true;
        return 0;
      }
      return program[reg.ip++];
    }

    // combo operand; [0-3] literal + [4-7] indirect
    uint64_t combo() {
      uint64_t value = read();
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
    uint64_t literal() {
      return read();
    }

    void reset(uint64_t a) {
      reg.a      = a;
      reg.b      = 0;
      reg.c      = 0;
      reg.ip     = 0;
      terminated = false;
      output.clear();
    }

    string execute() {
      return execute(reg.a);
    }

    string execute(uint64_t a) {
      // if (g_config.debug)
      //   // cout << term::cursor::clear();
      reset(a);

      while (!terminated) {
        int start = reg.ip;

        int        opcode = read();
        function_t inst   = instructions[opcode];

        (this->*inst)();

        int end = reg.ip;
        if (inst != &Day17::jnz)
          assert(end - start == 2);
        // if (inst == &Day17::out && g_config.debug)
        //   cout << term::cursor::clear();
        // if (g_config.debug)
        //   reg.print();

        if (reg.ip >= program.size())
          terminated = true;
      }

      return getOutputString();
    }

    uint64_t getMirrorInput() {
      uint64_t answer = numeric_limits<uint64_t>::max();

      queue<tuple<uint64_t, uint64_t, uint64_t>> q;

      int p = 0;

      uint64_t mask = bitmask(3);
      for (int i = 0; i < 8; i++)
        q.emplace(mask, i, 0);

      while (!q.empty() && p < program.size()) {
        uint64_t size = q.size();

        while (size) {
          size--;

          uint64_t a_mask  = get<0>(q.front());
          uint64_t a_value = get<1>(q.front());

          a_value = a_value >> get<2>(q.front());
          a_mask  = a_mask >> get<2>(q.front());

          uint64_t b = 0;
          uint64_t c = 0;

          b                = a_value % 8;
          b                = b ^ 5;
          c                = a_value >> b;
          uint64_t c_shift = b;
          b                = b ^ 6;

          uint64_t c_correct = b ^ program[p];  // correct answer
          uint64_t c_test    = (c_shift >= 3) ? 3 : 3 - c_shift;
          uint64_t c_mask    = a_mask >> c_shift;
          uint64_t c_check   = (c_correct & (~c_mask & 0x7)) | (c & 0x7);

          if (c_correct == c_check) {
            a_value = a_value | (c_correct << c_shift);
            a_mask  = a_mask | (0x7 << c_shift);

            uint64_t space_count = 0;
            uint64_t space_shift = -1;
            uint64_t space_mask  = 0;

            // space after C
            for (int i = 3; i < c_shift; i++) {
              if (!(a_mask & (1 << i))) {
                space_count++;
                if (space_shift == -1)
                  space_shift = i;
              }
            }

            // space before C
            if (!space_count) {
              space_shift = 64 - __builtin_clzll(a_mask);
              space_count = (space_shift < 6) ? 6 - space_shift : 0;
            }

            if (space_count)
              space_mask = bitmask(space_count);

            // generate options
            for (int i = 0; i < (1 << space_count); i++) {
              uint64_t tmp_val   = a_value | (i << space_shift);
              uint64_t tmp_mask  = a_mask | (space_mask << space_shift);
              uint64_t next      = (tmp_val << get<2>(q.front())) | get<1>(q.front());
              uint64_t next_mask = (tmp_mask << get<2>(q.front())) | get<0>(q.front());

              if (p == program.size() - 1)
                answer = min(answer, next);
              else
                q.emplace(next_mask, next, get<2>(q.front()) + 3);
            }
          }

          q.pop();
        }

        p++;
      }

      return answer;
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
          "Program: 0,3,5,4,3,0\n";
  SetUp();

  reg.a = 117440;

  execute();

  string output  = getOutputString();
  string program = getProgramString();
  EXPECT_EQ(output, program);
}

TEST_F(Day17, Part2) {
  uint64_t answer = getMirrorInput();
  execute(answer);

  string output  = getOutputString();
  string program = getProgramString();

  EXPECT_EQ(output, program);
  EXPECT_EQ(answer, 105875099912602);
}