#include "test.h"
#include "util/point.h"
#include "util/term.h"

using namespace std;

class Day20 : public aoc_2024 {
  protected:
    using point = point_int;

    vector<term::string>              map;
    int32_t                           w;
    int32_t                           h;
    point                             start;
    point                             end;
    vector<point>                     path;
    uint32_t                          baseline;
    set<point>                        cheats;
    unordered_map<uint32_t, uint32_t> savings;

    void LoadInput(istringstream& input) override {
      string s;
      while (getline(input, s)) {
        auto s_loc = s.find('S');
        auto e_loc = s.find('E');
        if (s_loc != -1)
          start = {(int)s_loc, (int)map.size()};
        if (e_loc != -1)
          end = {(int)e_loc, (int)map.size()};

        map.push_back(s);
      }

      h = map.size();
      w = map[0].length();
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

      const array<point, 4> cheat_directions = {
          point{0, -2},  // north
          point{2, 0},   // east
          point{0, 2},   // south
          point{-2, 0}   // west
      };

      rect_int bounds = {point{0, 0}, point{w - 1, h - 1}};

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

          if (!bounds.in_bounds(next))
            continue;

          if (map[next.y][next.x] == '#') {
            point cheat_dir = p + cheat_directions[v_dir];
            if (bounds.in_bounds(cheat_dir) && map[cheat_dir.y][cheat_dir.x] != '#') {
              cheats.emplace(next);

              if (debug())
                map[next.y].background(next.x).blue();
            }

            continue;
          }

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

    uint32_t getCheatCount(uint32_t picoSavings) {
      auto b = dijkstra();
      assert(b.has_value());
      baseline = b.value();

      return savings[picoSavings];
    }
};

TEST_F(Day20, Part1Example) {
  input = "###############\n"
          "#...#...#.....#\n"
          "#.#.#.#.#.###.#\n"
          "#S#...#.#.#...#\n"
          "#######.#.#.###\n"
          "#######.#.#...#\n"
          "#######.#.###.#\n"
          "###..E#...#...#\n"
          "###.#######.###\n"
          "#...###...#...#\n"
          "#.#####.#.###.#\n"
          "#.#...#.#.#...#\n"
          "#.#.#.#.#.#.###\n"
          "#...#...#...###\n"
          "###############";
  SetUp();

  EXPECT_EQ(getCheatCount(2), 14);
  EXPECT_EQ(getCheatCount(4), 14);
  EXPECT_EQ(getCheatCount(6), 2);
  EXPECT_EQ(getCheatCount(8), 4);
  EXPECT_EQ(getCheatCount(10), 2);
  EXPECT_EQ(getCheatCount(12), 3);
  EXPECT_EQ(getCheatCount(20), 1);
  EXPECT_EQ(getCheatCount(36), 1);
  EXPECT_EQ(getCheatCount(38), 1);
  EXPECT_EQ(getCheatCount(40), 1);
  EXPECT_EQ(getCheatCount(64), 1);
}

TEST_F(Day20, Part1) {
}