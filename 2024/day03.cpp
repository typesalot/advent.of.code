#include "common.h"

string readInput(const string& input) {
  auto         f = ifstream(input);
  stringstream stream;
  string       s;
  while (getline(f, s))
    stream << s;
  return stream.str();
}

class graph;

class match_node {
  public:
    virtual ~match_node() = default;

    virtual bool match(char v) {
      return find_edge(v) == std::nullopt;
    }

    virtual void consume(char v) {};
    virtual void reset() {};

    virtual bool is_enabled() {
      return true;
    }

    optional<shared_ptr<match_node>> find_edge(char v) {
      for (auto& edge : edges)
        if (edge->is_enabled() && edge->match(v))
          return edge;
      return std::nullopt;
    }

  private:
    friend class graph;
    vector<shared_ptr<match_node>> edges;
};

class match_keyword : public match_node {
  private:
    bool                      enabled = true;
    string                    keyword;
    string::iterator          itr;
    std::function<void(void)> oncomplete;

  public:
    match_keyword(const string& kw) : keyword(kw), oncomplete([]() {}), itr(keyword.begin()) {};
    match_keyword(const string& kw, decltype(oncomplete) cb) : keyword(kw), oncomplete(cb), itr(keyword.begin()) {};

    bool match(char v) override {
      return *itr == v;
    }

    void consume(char v) override {
      itr++;
      if (itr == keyword.end())
        oncomplete();
    }

    virtual void reset() override {
      itr = keyword.begin();
    }

    bool is_enabled() override {
      return this->enabled;
    }

    void enable() {
      this->enabled = true;
    }

    void disable() {
      this->enabled = false;
    }
};

class capture_num : public match_node {
  private:
    int num = 0;

  public:
    bool match(char v) override {
      return isdigit(v);
    }

    void consume(char v) override {
      num *= 10;
      num += v - '0';
    }

    int get_capture() {
      return num;
    }

    virtual void reset() override {
      num = 0;
    }
};

class graph {
  private:
    shared_ptr<match_node>              _head = nullptr;
    std::vector<shared_ptr<match_node>> nodes;

  public:
    graph() {
      _head = add_node<match_node>();
    }

    template <typename T, typename... ArgsT>
    auto add_node(ArgsT&&... args) -> shared_ptr<T> {
      auto new_node = make_shared<T>(std::forward<ArgsT>(args)...);
      nodes.emplace_back(new_node);
      return new_node;
    }

    template <typename T1, typename T2>
    void add_edge(shared_ptr<T1>& from, shared_ptr<T2>& to) {
      from->edges.emplace_back(to);
    }

    auto head() -> decltype(_head)& {
      return _head;
    }

    void reset() {
      for (auto& node : nodes)
        node->reset();
    }
};

int calcMulls(const string& corrupted, bool use_enables) {
  int total = 0;

  // create graph
  graph g;

  auto kw_mul = g.add_node<match_keyword>("mul(");
  auto num1   = g.add_node<capture_num>();
  auto num2   = g.add_node<capture_num>();
  auto comma  = g.add_node<match_keyword>(",");
  auto cparen = g.add_node<match_keyword>(")", [&num1, &num2, &total]() {
    int a = num1->get_capture();
    int b = num2->get_capture();
    total += a * b;
  });

  // create graph edges
  g.add_edge(g.head(), kw_mul);  // mul(
  g.add_edge(kw_mul, num1);      // mul(25
  g.add_edge(num1, comma);       // mul(25,
  g.add_edge(comma, num2);       // mul(25,48
  g.add_edge(num2, cparen);      // mul(25,48)

  // if supporting do/don't, add couple more nodes/edges
  if (use_enables) {
    auto _do = g.add_node<match_keyword>("do()", [&kw_mul]() { kw_mul->enable(); });

    auto nt = g.add_node<match_keyword>("n't()", [&kw_mul]() { kw_mul->disable(); });

    g.add_edge(g.head(), _do);  // do()
    g.add_edge(_do, nt);        // don't()
  }

  // visit the graph with each character
  auto current = g.head();
  for (auto c : corrupted) {
    // find next match
    while (!current->match(c)) {
      auto edge = current->find_edge(c);
      if (edge)
        current = *edge;
      else {
        current = g.head();
        g.reset();
      }
    }

    // consume it
    current->consume(c);
  }

  return total;
}

TEST(Day3, Part1Examples) {
  EXPECT_EQ(calcMulls("mul(10,5)", false), 50);
  EXPECT_EQ(calcMulls("xmul(2,4)%&mul[3,7]!@^do_not_mul(5,5)+mul(32,64]then(mul(11,8)mul(8,5))", false), 161);
}

TEST(Day3, Part2Examples) {
  //                     ---------xxxxxxx.........
  EXPECT_EQ(calcMulls("mul(10,5)don't()mull(2,4)", true), 50);
  //                      --------           xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx++++ -------- --> 2*4 + 8*5 = 48
  EXPECT_EQ(calcMulls("xmul(2,4)&mul[3,7]!^don't()_mul(5,5)+mul(32,64](mul(11,8)undo()?mul(8,5))", true), 48);
}

TEST(Day3, Part1) {
  string s   = readInput("input/day03.txt");
  int    ans = calcMulls(s, false);
  EXPECT_EQ(ans, 192767529);
  cout << "Answer = " << ans << endl;
}

TEST(Day3, Part2) {
  string s   = readInput("input/day03.txt");
  int    ans = calcMulls(s, true);
  EXPECT_EQ(ans, 104083373);
  cout << "Answer = " << ans << endl;
}