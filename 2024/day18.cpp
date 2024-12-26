#include "test.h"
#include "util/point.h"
#include "util/term.h"
#include <optional>
#include <queue>
#include <vector>

using namespace std;

class Day18 : public aoc_2024 {
  protected:
    using point = point_int;

    int32_t              w = 71;
    int32_t              h = 71;
    point                start;
    point                end;
    vector<point>        spawns;
    vector<term::string> map;
    vector<point>        path;

    static constexpr int north = 0;
    static constexpr int east  = 1;
    static constexpr int south = 2;
    static constexpr int west  = 3;

    void LoadInput(istringstream& input) override {
      map.resize(h);
      for (auto& m : map)
        m = string(w, '.');

      string s;
      while (getline(input, s)) {
        size_t f = s.find(",");
        point& p = spawns.emplace_back();
        p.x      = stoi(s.substr(0, f));
        p.y      = stoi(s.substr(f + 1, s.length() - f));
      }

      start = {0, 0};
      end   = {w - 1, h - 1};
    }

    void print_map() {
      if (!g_config.debug)
        return;
      static bool move = false;
      if (move)
        cout << term::cursor::move_up(h);
      move = true;
      for (const auto& m : map)
        cout << m << endl;
    }

    void reset_map() {
      if (!g_config.debug)
        return;
      for (auto& m : map)
        m.reset();
    }

    optional<uint32_t> dijkstra() {
      reset_map();

      uint32_t cost = 0;

      int count = h * w;

      const array<point, 4> directions = {
          point{0, -1},  // north
          point{1, 0},   // east
          point{0, 1},   // south
          point{-1, 0}   // west
      };

      rect_int bounds = {start, end};

      vector<int> distances(count, numeric_limits<int>::max());
      vector<int> predecessors(count, -1);

      distances[start.flatten(w)] = 0;

      using node_t      = tuple<int, int>;  // distance,node
      using container_t = vector<node_t>;
      using comp_t      = greater<node_t>;
      priority_queue<node_t, container_t, comp_t> pq;

      pq.emplace(0, start.flatten(w));

      while (!pq.empty()) {
        int dist = get<0>(pq.top());  // distance
        int u    = get<1>(pq.top());  // node
        pq.pop();

        if (u == end.flatten(w))
          break;

        if (dist > distances[u])
          continue;

        point up = point::expand(u, w);
        if (g_config.debug) {
          map[up.y].background(up.x).red();
          print_map();
        }

        point p = point::expand(u, w);
        for (int v_dir = 0; v_dir < 4; v_dir++) {
          point next = p + directions[v_dir];

          if (!bounds.in_bounds(next) || map[next.y][next.x] == '#')
            continue;

          uint32_t v = next.flatten(w);

          // costs 1 to move to next node
          int weight = 1;

          if (distances[u] + weight < distances[v]) {
            distances[v]    = distances[u] + weight;
            predecessors[v] = u;
            pq.emplace(distances[v], v);
          }
        }
      }

      path.clear();

      int  at     = end.flatten(w);
      bool exists = false;
      while (at != -1) {
        path.emplace_back(point::expand(at, w));

        if (start.flatten(w) == at)
          exists = true;

        at = predecessors[at];
      }

      if (exists) {
        reverse(path.begin(), path.end());

        for (auto& p : path) {
          map[p.y].background(p.x).green();
          print_map();
        }

        cost = distances[end.flatten(w)];

        return cost;
      }

      return nullopt;
    }

    uint32_t countShortestPath(uint32_t fall_count) {
      for (int i = 0; i < fall_count; i++) {
        const auto& s = spawns[i];
        map[s.y][s.x] = '#';
      }

      auto answer = dijkstra();

      return answer.value();
    }

    point getBlockingByte() {
      point answer;

      auto shortest = dijkstra();

      for (int i = 0; i < spawns.size(); i++) {
        const auto& s = spawns[i];
        map[s.y][s.x] = '#';

        auto f = find(path.begin(), path.end(), s);

        if (f != path.end()) {
          shortest = dijkstra();
          if (!shortest) {
            answer = s;
            break;
          }
        }
      }

      return answer;
    }
};

TEST_F(Day18, Part1Example) {
  input = "5,4\n"
          "4,2\n"
          "4,5\n"
          "3,0\n"
          "2,1\n"
          "6,3\n"
          "2,4\n"
          "1,5\n"
          "0,6\n"
          "3,3\n"
          "2,6\n"
          "5,1\n"
          "1,2\n"
          "5,5\n"
          "2,5\n"
          "6,5\n"
          "1,4\n"
          "0,4\n"
          "6,4\n"
          "1,1\n"
          "6,1\n"
          "1,0\n"
          "0,5\n"
          "1,6\n"
          "2,0\n";
  w     = 7;
  h     = 7;
  SetUp();

  uint32_t answer = countShortestPath(12);
  EXPECT_EQ(answer, 22);
}

TEST_F(Day18, Part1) {
  uint32_t answer = countShortestPath(1024);
  EXPECT_EQ(answer, 252);
}

TEST_F(Day18, Part2Example) {
  input = "5,4\n"
          "4,2\n"
          "4,5\n"
          "3,0\n"
          "2,1\n"
          "6,3\n"
          "2,4\n"
          "1,5\n"
          "0,6\n"
          "3,3\n"
          "2,6\n"
          "5,1\n"
          "1,2\n"
          "5,5\n"
          "2,5\n"
          "6,5\n"
          "1,4\n"
          "0,4\n"
          "6,4\n"
          "1,1\n"
          "6,1\n"
          "1,0\n"
          "0,5\n"
          "1,6\n"
          "2,0\n";
  w     = 7;
  h     = 7;
  SetUp();

  point answer = getBlockingByte();
  EXPECT_EQ(answer.x, 6);
  EXPECT_EQ(answer.y, 1);
}

TEST_F(Day18, Part2) {
  point answer = getBlockingByte();
  EXPECT_EQ(answer.x, 5);
  EXPECT_EQ(answer.y, 60);
}