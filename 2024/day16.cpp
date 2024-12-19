#include "util/point.h"
#include <cmath>
#include <gtest/gtest.h>
#include <queue>
#include <sstream>
#include <unordered_map>
#include <vector>
using namespace std;

class Day16 : public ::testing::Test {
  protected:
    using point = point_int;

    struct vertex;

    struct edge {
        edge(vertex* dst, uint32_t _cost) : v(dst), cost(_cost) {};

        vertex*  v;
        uint32_t cost;
    };

    struct vertex {
        vertex(const point& p) : pos(p) {};

        point        pos;
        vector<edge> edges;
    };

    vertex* start;

    struct graph_t {
        vertex* start = nullptr;

        vector<vertex*>                  vertices;
        unordered_map<uint32_t, vertex*> lookup;

        graph_t() = default;

        ~graph_t() {
          clear();
        }

        void clear() {
          for (auto v : vertices)
            delete v;
          vertices.clear();
          lookup.clear();
          start = nullptr;
        }

        vertex* add_vertex(const point& p) {
          uint32_t key = (p.y << 16) + p.x;
          vertex*& v   = lookup[key];
          if (v == nullptr) {
            v = new vertex(p);
            vertices.push_back(v);
            if (start == nullptr)
              start = v;
          }
          return v;
        }
    } graph;

    static constexpr uint32_t o_north = 0;
    static constexpr uint32_t o_east  = 1;
    static constexpr uint32_t o_south = 2;
    static constexpr uint32_t o_west  = 3;

    void SetUp() override {
    }

    template <typename src_t>
    void readInput(src_t& src) {
      graph.clear();

      vector<string> map;
      string         s;
      while (getline(src, s))
        map.push_back(s);

      createGraph(map);
    }

    void readInput(string& s) {
      auto input = istringstream(s);
      readInput(input);
    }

    uint32_t getTurnCost(uint32_t from, uint32_t to) {
      uint32_t d = (from != to) ? 2 - (to + from) % 2 : 0;
      return d * 1000;
    };

    point castRay(const vector<string>& map, const point& p, const point& d) {
      point result = p + d;
      char  c      = map[result.y][result.x];
      while (c != '#') {
        result += d;
        c = map[result.y][result.x];
      }
      return result - d;
    }

    uint32_t getDistanceCost(const point& p) {
      return (abs(p.y)) ? abs(p.y) : abs(p.x);
    }

    uint32_t getRayOrientation(const point& ray) {
      if (ray.x == 1)
        return o_east;
      else if (ray.x == -1)
        return o_west;
      else if (ray.y == 1)
        return o_south;
      return o_north;
    }

    void createGraph(const vector<string>& map) {
      start = graph.add_vertex({0, 0});

      // find start
      for (int32_t j = 0; j < map.size(); j++) {
        auto x = map[j].find('S');
        if (x != -1)
          start->pos = {(int)x, j};
      }

      struct choice {
          vertex*  vertex;
          uint32_t cost;
          uint32_t from;
      };

      queue<choice> choices;
      choices.emplace(start, 0, o_east);

      // cast rays
      vertex*  current             = choices.front().vertex;
      uint32_t existing_cost       = choices.front().cost;
      uint32_t current_orientation = choices.front().from;
      choices.pop();

      point ray     = {1, 0};
      auto  ray_hit = castRay(map, current->pos, ray);
      if (current->pos != ray_hit) {
        uint32_t ray_orientation = getRayOrientation(ray);
        uint32_t turn_cost       = getTurnCost(current_orientation, ray_orientation);
        uint32_t travel_cost     = getDistanceCost(current->pos - ray_hit);
        uint32_t cost            = existing_cost + turn_cost + travel_cost;

        vertex* dst = graph.add_vertex(ray_hit);

        current->edges.emplace_back(dst, cost);
      }
    }

    uint32_t getMinCostPath() {
      uint32_t cost = 0;

      return cost;
    }
};

TEST_F(Day16, turn_cost) {
  // north
  EXPECT_EQ(getTurnCost(o_north, o_north), 0);
  EXPECT_EQ(getTurnCost(o_north, o_east), 1000);
  EXPECT_EQ(getTurnCost(o_north, o_west), 1000);
  EXPECT_EQ(getTurnCost(o_north, o_south), 2000);

  // east
  EXPECT_EQ(getTurnCost(o_east, o_east), 0);
  EXPECT_EQ(getTurnCost(o_east, o_south), 1000);
  EXPECT_EQ(getTurnCost(o_east, o_west), 2000);
  EXPECT_EQ(getTurnCost(o_east, o_north), 1000);

  // south
  EXPECT_EQ(getTurnCost(o_south, o_south), 0);
  EXPECT_EQ(getTurnCost(o_south, o_west), 1000);
  EXPECT_EQ(getTurnCost(o_south, o_east), 1000);
  EXPECT_EQ(getTurnCost(o_south, o_north), 2000);

  // west
  EXPECT_EQ(getTurnCost(o_west, o_west), 0);
  EXPECT_EQ(getTurnCost(o_west, o_south), 1000);
  EXPECT_EQ(getTurnCost(o_west, o_east), 2000);
  EXPECT_EQ(getTurnCost(o_west, o_north), 1000);
}

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