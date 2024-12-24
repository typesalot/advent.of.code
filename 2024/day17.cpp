#include "test.h"
#include "util/string.h"
#include "util/term.h"
#include <bitset>
#include <format>
#include <functional>
#include <queue>
#include <source_location>
#include <vector>

using namespace std;

bool execute_print_regs = false;

class Day17 : public aoc_2024 {
  public:
    vector<int> output;

    struct reg_t {
        uint64_t a;
        uint64_t b;
        uint64_t c;
        uint64_t ip = 0;

        unique_ptr<reg_t> last = nullptr;

        void print() {
          if (!execute_print_regs)
            return;

          term::string s;
          uint64_t     y = 10;
          uint64_t     x = 8 + 10 + 32 + 6;

          cout << term::cursor::save();

          s = format("{:>8} = {:10} {:064b}\n", "A", a, a);
          if (last && a != last->a)
            s.fg_red();
          cout << term::cursor::move(y, x);
          cout << s;

          s = format("{:>8} = {:10} {:064b}\n", "B", b, b);
          if (last && b != last->b)
            s.fg_red();
          cout << term::cursor::set_x(x);
          cout << s;

          s = format("{:>8} = {:10} {:064b}\n", "C", c, c);
          if (last && c != last->c)
            s.fg_red();

          cout << term::cursor::set_x(x);
          cout << s;

          cout << term::cursor::restore();

          if (last == nullptr)
            last = make_unique<reg_t>();
          last->a = a;
          last->b = b;
          last->c = c;
        }
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
      if (execute_print_regs) {
        string   cstr  = combo_string();
        uint64_t value = combo();
        reg.ip--;

        if (cstr == to_string(value))
          cout << format("[adv] A = A >> {}\n", cstr);
        else
          cout << format("[adv] A = A >> {} = A >> {}\n", cstr, value);
        cout << format("{:>8} = {:10} {:064b}\n", "A", reg.a, reg.a);
        cout << format("{:>8} = {:10} {:064b}\n", cstr, value, value);
        cout << format("{:>8} = {:10} {:064b}\n\n", ">>", reg.a / (1 << value), reg.a / (1 << value));
      }

      uint64_t value = combo();
      reg.a          = reg.a / (1 << value);
    }

    // 1 - bitwise XOR; B ^ literal operand
    void bxl() {
      if (execute_print_regs) {
        uint64_t value = literal();
        reg.ip--;

        cout << format("[bxl] B = B ^ {}\n", value);
        cout << format("{:>8} = {:10} {:064b}\n", "B", reg.b, reg.b);
        cout << format("{:>8} = {:10} {:064b}\n", value, value, value);
        cout << format("{:>8} = {:10} {:064b}\n\n", "^", reg.b ^ value, reg.b ^ value);
      }

      uint64_t value = literal();
      reg.b          = reg.b ^ value;
    }

    // 2 - bitwise mod8; B = combo % 8
    void bst() {
      if (execute_print_regs) {
        auto cstr  = combo_string();
        auto value = combo();
        reg.ip--;

        if (cstr == to_string(value))
          cout << format("[bst] B = {} % 8 = \n", cstr);
        else
          cout << format("[bst] B = {} % 8 = {} % 8 \n", cstr, value);
        cout << format("{:>8} = {:10} {:064b}\n", cstr, value, value);
        cout << format("{:>8} = {:10} {:064b}\n", "8", 8, 8);
        cout << format("{:>8} = {:10} {:064b}\n\n", "%", value % 8, value % 8);
      }

      uint64_t value = combo();
      reg.b          = value % 8;
    }

    // 3 - jump-not-zero; nop if A == 0 else ip = literal
    void jnz() {
      if (execute_print_regs) {
        uint64_t value = literal();
        reg.ip--;

        cout << format("[jnz] {}", value) << endl;
        cout << format("{:>8} = {:10} {:064b}\n\n", "A", reg.a, reg.a);
      }

      uint64_t value = literal();
      if (!reg.a)
        return;
      reg.ip = value;
    }

    // 4 - bitwise XOR (legacy); B = B ^ C
    void bxc() {
      if (execute_print_regs) {
        cout << format("[bxc] B = B ^ C\n");
        cout << format("{:>8} = {:10} {:064b}\n", "B", reg.b, reg.b);
        cout << format("{:>8} = {:10} {:064b}\n", "C", reg.c, reg.c);
        cout << format("{:>8} = {:10} {:064b}\n\n", "^", reg.b ^ reg.c, reg.b ^ reg.c);
      }

      literal();
      reg.b = reg.b ^ reg.c;
    }

    // 5 - saves output; output.push( combo % 8 )
    void out() {
      if (execute_print_regs) {
        string   cstr  = combo_string();
        uint64_t value = combo();
        reg.ip--;

        if (cstr == to_string(value))
          cout << format("[out] {} % 8\n", value);
        else
          cout << format("[out] {} % 8 = {} % 8 \n", cstr, value);
        cout << format("{:>8} = {:10} {:064b}\n", cstr, value, value);
        cout << format("{:>8} = {:10} {:064b}\n", "8", 8, 8);
        cout << format("{:>8} = {:10} {:064b}\n", "%", value % 8, value % 8);
      }

      uint64_t value = combo();
      value %= 8;

      output.push_back(value);
    }

    // 6 - division; B = A / ( 1 << combo ) truncated
    // B = A >> combo()
    void bdv() {
      if (execute_print_regs) {
        string   cstr  = combo_string();
        uint64_t value = combo();
        reg.ip--;

        if (cstr == to_string(value))
          cout << format("[bdv] B = A >> {}\n", cstr);
        else
          cout << format("[bdv] B = A >> {} = A >> {}\n", cstr, value);
        cout << format("{:>8} = {:10} {:064b}\n", "A", reg.a, reg.a);
        cout << format("{:>8} = {:10} {:064b}\n", cstr, value, value);
        cout << format("{:>8} = {:10} {:064b}\n\n", ">>", reg.a / (1 << value), reg.a / (1 << value));
      }

      uint64_t value = combo();
      reg.b          = reg.a / (1 << value);
    }

    // 7 - division; C = A / ( 1 << combo ) truncated
    // C = A >> combo()
    void cdv() {
      if (execute_print_regs) {
        string   cstr  = combo_string();
        uint64_t value = combo();
        reg.ip--;

        if (cstr == to_string(value))
          cout << format("[cdv] C = A >> {}\n", cstr);
        else
          cout << format("[cdv] C = A >> {} = A >> {}\n", cstr, value);
        cout << format("{:>8} = {:10} {:064b}\n", "A", reg.a, reg.a);
        cout << format("{:>8} = {:10} {:064b}\n", cstr, value, value);
        cout << format("{:>8} = {:10} {:064b}\n\n", ">>", reg.a / (1 << value), reg.a / (1 << value));
      }

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

    string combo_string() {
      uint64_t value = program[reg.ip];
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
    uint64_t literal() {
      return read();
    }

    void reset(uint64_t a) {
      reg.a      = a;
      reg.b      = 0;
      reg.c      = 0;
      reg.ip     = 0;
      reg.last   = nullptr;
      terminated = false;
      output.clear();
    }

    string execute(uint64_t a = 0) {
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
  reg.a = 5522;

  auto print_a = [](const string& header, uint64_t a_value, uint64_t a_mask) {
    // cout << format("{:>10} = {:10} {:064b}\n", "A mask", a_mask, a_mask);
    cout << format("{:>10} = ", header, "");
    uint64_t print_mask = 1l << 63;
    while (print_mask) {
      if (a_mask & print_mask) {
        if (print_mask & a_value)
          cout << "1";
        else
          cout << "0";
      } else {
        cout << ".";
      }
      print_mask >>= 1;
    }
    cout << endl;
  };

  queue<tuple<uint64_t, uint64_t, uint64_t>> q;

  int p = 0;

  uint64_t mask = (1 << 3) - 1;
  for (int i = 0; i < 8; i++)
    q.emplace(mask, i, 0);

  while (!q.empty() && p < program.size()) {
    uint64_t size      = q.size();
    uint64_t out_match = program[p];
    p++;

    cout << format("\nMatching {} in program...", out_match);
    cout.flush();

    while (size) {
      size--;

      uint64_t a_mask  = get<0>(q.front());
      uint64_t a_value = get<1>(q.front());
      uint64_t a_shift = get<2>(q.front());

      a_value = a_value >> a_shift;
      a_mask  = a_mask >> a_shift;

      uint64_t b = 0;
      uint64_t c = 0;

      b                = a_value % 8;
      b                = b ^ 5;
      c                = a_value >> b;
      uint64_t c_shift = b;
      b                = b ^ 6;

      uint64_t c_correct = b ^ out_match;  // correct answer
      uint64_t c_test    = (c_shift >= 3) ? 3 : 3 - c_shift;
      uint64_t c_mask    = a_mask >> c_shift;
      uint64_t c_check   = (c_correct & (~c_mask & 0x7)) | (c & 0x7);

      bool valid = (c_correct == c_check);

      if (valid) {
        a_value = a_value | (c_correct << c_shift);
        a_mask  = a_mask | (0x7 << c_shift);

        cout << endl;
        print_a("A", a_value, a_mask);
        print_a("A", a_value, -1);

        uint64_t space_count = 0;
        uint64_t space_shift = -1;
        for (int i = 3; i < c_shift; i++) {
          if (!(a_mask & (1 << i))) {
            space_count++;
            if (space_shift == -1)
              space_shift = i;
          }
        }
        uint64_t space_mask = (1 << space_count) - 1;
        // space_count = min<uint64_t>(space_count, 3);
        if (space_count) {
          for (int i = 0; i < (1 << space_count); i++) {
            uint64_t tmp_val  = a_value | (i << space_shift);
            uint64_t tmp_mask = a_mask | (space_mask << space_shift);

            uint64_t next      = (tmp_val << a_shift) | (get<1>(q.front()) & (1l << a_shift) - 1);
            uint64_t next_mask = (tmp_mask << a_shift) | (get<0>(q.front()) & (1l << a_shift) - 1);
            print_a("next", next, next_mask);
            auto o = execute(next);
            cout << format("{:>10} execute({}) = {}", "", next, o) << endl;
            if (o == getProgramString())
              assert(false);
            q.emplace(next_mask, next, a_shift + 3);
          }
        } else {
          uint64_t next      = (a_value << a_shift) | (get<1>(q.front()) & (1l << a_shift) - 1);
          uint64_t next_mask = (a_mask << a_shift) | (get<0>(q.front()) & (1l << a_shift) - 1);
          print_a("next", next, next_mask);
          auto o = execute(next);
          cout << format("{:>10} execute({}) = {}", "", next, o) << endl;
          if (o == getProgramString())
            assert(false);
          q.emplace(next_mask, next, a_shift + 3);
        }
      }

      q.pop();
    }
  }

  return;

  execute();

  string output  = getOutputString();
  string program = getProgramString();
  // EXPECT_EQ(output, program);
}