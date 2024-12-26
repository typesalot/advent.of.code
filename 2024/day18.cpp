#include "test.h"
#include "util/point.h"
#include "util/term.h"
#include <queue>

using namespace std;

class Day18 : public aoc_2024 {
  protected:
    using point = point_int;

    int32_t              w;
    int32_t              h;
    uint32_t             fall_count;
    point                start;
    point                end;
    queue<point>         spawns;
    vector<term::string> map;

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
        point& p = spawns.emplace();
        p.x      = stoi(s.substr(0, f));
        p.y      = stoi(s.substr(f + 1, s.length() - f));
        if (fall_count) {
          map[p.y][p.x] = '#';
          fall_count--;
        }
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

    uint32_t dijkstra() {
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

      using node_t      = tuple<int, int, int>;  // distance,node,direction
      using container_t = vector<node_t>;
      using comp_t      = greater<node_t>;
      priority_queue<node_t, container_t, comp_t> pq;

      pq.emplace(0, start.flatten(w), east);

      while (!pq.empty()) {
        int dist  = get<0>(pq.top());  // distance
        int u     = get<1>(pq.top());  // node
        int u_dir = get<2>(pq.top());  // direction
        pq.pop();

        if (u == end.flatten(w))
          break;

        if (dist > distances[u])
          continue;

        point up = point::expand(u, w);
        map[up.y].background(up.x).red();
        print_map();

        point p = point::expand(u, w);
        for (int v_dir = 0; v_dir < 4; v_dir++) {
          // opposite directions are always mod2 because of
          // how the cardinal directions are defined above
          if (v_dir != u_dir && (v_dir + u_dir % 2 == 0))
            continue;

          point next = p + directions[v_dir];

          if (!bounds.in_bounds(next) || map[next.y][next.x] == '#')
            continue;

          uint32_t v = next.flatten(w);

          // costs 1 to move to next node
          int weight = 1;

          if (distances[u] + weight < distances[v]) {
            distances[v]    = distances[u] + weight;
            predecessors[v] = u;
            pq.emplace(distances[v], v, v_dir);
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

      cost = distances[end.flatten(w)];

      return cost;
    }

    uint32_t countShortestPath() {
      uint32_t answer = dijkstra();

      return answer;
    }
};

TEST_F(Day18, Part1Example) {
  input      = "5,4\n"
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
  w          = 7;
  h          = 7;
  fall_count = 12;
  SetUp();

  uint32_t answer = countShortestPath();
  EXPECT_EQ(answer, 22);
}