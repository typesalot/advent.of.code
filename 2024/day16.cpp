#include "util/config.h"
#include "util/point.h"
#include "util/term.h"
#include <array>
#include <cmath>
#include <gtest/gtest.h>
#include <queue>
#include <sstream>
#include <vector>
using namespace std;

class Day16 : public ::testing::Test {
  protected:
    using point = point_int;

    vector<term::string> map;
    point                start;
    point                end;
    uint32_t             w;
    uint32_t             h;

    void SetUp() override {
    }

    template <typename src_t>
    void readInput(src_t& src) {
      string s;
      while (getline(src, s)) {
        auto s_location = s.find('S');
        if (s_location != -1) {
          start.x = s_location;
          start.y = map.size();
        }

        auto e_location = s.find('E');
        if (e_location != -1) {
          end.x = e_location;
          end.y = map.size();
        }

        map.push_back(s);
      }
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

    void readInput(string& s) {
      auto input = istringstream(s);
      readInput(input);
    }

    uint32_t dijkstra() {
      uint32_t cost = 0;

      h         = map.size();
      w         = map[0].length();
      int count = h * w;

      const array<point, 4> directions = {
          point{0, -1},  // north
          point{1, 0},   // east
          point{0, 1},   // south
          point{-1, 0}   // west
      };

      vector<int> distances(count, numeric_limits<int>::max());
      vector<int> predecessors(count, -1);

      distances[start.flatten(w)] = 0;

      using node_t      = pair<int, int>;  // distance,node
      using container_t = vector<node_t>;
      using comp_t      = greater<node_t>;
      priority_queue<node_t, container_t, comp_t> pq;

      pq.push({0, start.flatten(w)});

      while (!pq.empty()) {
        int dist = pq.top().first;   // distance
        int u    = pq.top().second;  // node
        pq.pop();

        if (dist > distances[u])
          continue;

        point up = point::expand(u, w);
        map[up.y].background(up.x).red();
        print_map();

        point p = point::expand(u, w);
        for (const auto& dir : directions) {
          point next = p + dir;

          if (map[next.y][next.x] == '#')
            continue;

          uint32_t v      = next.flatten(w);
          int      weight = 1;

          if (distances[u] + weight < distances[v]) {
            distances[v]    = distances[u] + weight;
            predecessors[v] = u;
            pq.push({distances[v], v});
          }
        }
      }

      vector<point> path;
      int           at = end.flatten(w);
      while (at != -1) {
        path.emplace_back(point::expand(at, w));
        at = predecessors[at];
      }
      reverse(path.begin(), path.end());

      for (auto& p : path) {
        map[p.y].background(p.x).green();
        print_map();
      }

      return cost;
    }

    uint32_t getMinCostPath() {
      return dijkstra();
    }
};

TEST_F(Day16, Part1Example) {
  string s;

  s = "###############\n"
      "#.......#....E#\n"
      "#.#.###.#.###.#\n"
      "#.....#.#...#.#\n"
      "#.###.#####.#.#\n"
      "#.#.#.......#.#\n"
      "#.#.#####.###.#\n"
      "#...........#.#\n"
      "###.#.#####.#.#\n"
      "#...#.....#.#.#\n"
      "#.#.#.###.#.#.#\n"
      "#.....#...#.#.#\n"
      "#.###.#.#.#.#.#\n"
      "#S..#.....#...#\n"
      "###############\n";
  readInput(s);

  uint32_t cost = getMinCostPath();
  EXPECT_EQ(cost, 7036);
}

TEST_F(Day16, Part1) {
}