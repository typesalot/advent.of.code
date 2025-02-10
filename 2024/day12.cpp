#include "test.h"
#include "util/point.h"
#include "util/term.h"
#include <bitset>
#include <queue>
#include <stack>

using namespace std;

class Day12 : public aoc_2024 {
  protected:
    using point = point_int;

    struct region {
        char     crop;
        uint32_t area  = 0;
        uint32_t perim = 0;
        uint32_t sides = 0;

        region(char _c) : crop(_c) {};
    };

    enum edge {
      left   = 0,
      top    = 1,
      right  = 2,
      bottom = 3
    };

    using perim = bitset<4>;

    vector<term::string> plot;
    int                  w;
    int                  h;
    vector<vector<bool>> visited;
    vector<region>       regions;

    void LoadInput(istringstream& input) override {
      plot.clear();
      
      string s;
      while (getline(input, s))
        plot.push_back(s);

      regions.clear();
      visited.clear();

      h = plot.size();
      w = plot[0].length();

      visited.resize(h);
      for (auto& v : visited)
        v.resize(w, false);
    }

    void print_plot() {
      if (!g_config.debug)
        return;
      static bool move = false;
      if (move)
        cout << term::cursor::move_up(plot.size());
      move = true;

      for (const auto& r : plot)
        cout << r << endl;
    }

    void clear_colors() {
      if (!g_config.debug)
        return;
      for (auto& r : plot)
        r.reset();
    }

    perim calcPerim(char crop, int x, int y) {
      perim p;

      // left
      if (x == 0)
        p.set(left);
      else if (x != 0 && plot[y][x - 1] != crop)
        p.set(left);

      // right
      if (x + 1 == w)
        p.set(right);
      else if (x + 1 < w && plot[y][x + 1] != crop)
        p.set(right);

      // up
      if (y == 0)
        p.set(top);
      else if (y != 0 && plot[y - 1][x] != crop)
        p.set(top);

      // bottom
      if (y + 1 == h)
        p.set(bottom);
      else if (y + 1 < h && plot[y + 1][x] != crop)
        p.set(bottom);

      return p;
    }

    void map_region(int startx, int starty) {
      char current_crop = plot[starty][startx];
      clear_colors();
      regions.emplace_back(current_crop);
      auto& region = regions.back();

      queue<point> next;
      next.emplace(startx, starty);

      while (!next.empty()) {
        int x = next.front().x;
        int y = next.front().y;
        next.pop();

        if (visited[x][y])
          continue;
        visited[x][y] = true;

        region.area++;
        region.perim += calcPerim(region.crop, x, y).count();

        if (g_config.debug) {
          plot[y].background(x).red();
          print_plot();
        }

        // add unvisited neighbors
        if (x != 0 && !visited[x - 1][y]) {
          if (plot[y][x - 1] == current_crop)
            next.emplace(x - 1, y);
        }

        if (x + 1 < w && !visited[x + 1][y]) {
          if (plot[y][x + 1] == current_crop)
            next.emplace(x + 1, y);
        }

        if (y != 0 && !visited[x][y - 1]) {
          if (plot[y - 1][x] == current_crop)
            next.emplace(x, y - 1);
        }

        if (y + 1 < h && !visited[x][y + 1]) {
          if (plot[y + 1][x] == current_crop)
            next.emplace(x, y + 1);
        }
      }
    }

    void map_region_sides(int startx, int starty) {
      char current_crop = plot[starty][startx];
      clear_colors();
      regions.emplace_back(current_crop);
      auto& region = regions.back();

      stack<point> outer;
      stack<point> inner;
      outer.emplace(startx, starty);

      perim last_perim = calcPerim(region.crop, startx, starty);

      while (!outer.empty() || !inner.empty()) {
        int x = (!outer.empty()) ? outer.top().x : inner.top().x;
        int y = (!outer.empty()) ? outer.top().y : inner.top().x;

        if (!outer.empty())
          outer.pop();
        else {
          last_perim.reset();
          inner.pop();
        }

        if (visited[x][y])
          continue;
        visited[x][y] = true;

        if (g_config.debug) {
          plot[y].background(x).red();
          print_plot();
        }

        perim p     = calcPerim(region.crop, x, y);
        perim sides = p ^ last_perim;
        last_perim  = p;

        region.area++;
        region.perim += p.count();
        region.sides += sides.count();

        // add unvisited neighbors
        if (x != 0 && !visited[x - 1][y]) {
          if (!p.test(edge::left))
            outer.emplace(x - 1, y);
        }

        if (x + 1 < w && !visited[x + 1][y]) {
          if (!p.test(edge::right))
            outer.emplace(x + 1, y);
        }

        if (y != 0 && !visited[x][y - 1]) {
          if (!p.test(edge::top))
            outer.emplace(x, y - 1);
        }

        if (y + 1 < h && !visited[x][y + 1]) {
          if (!p.test(edge::bottom))
            outer.emplace(x, y + 1);
        }
      }

      // region.sides += remaining.count();
    }

    uint32_t getPrice() {
      for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
          // skip visited nodes
          if (visited[x][y])
            continue;

          map_region(x, y);
        }
      }

      uint32_t price = 0;
      for (const auto& region : regions)
        price += region.area * region.perim;

      return price;
    }

    uint32_t getDiscountedPrice() {
      for (int y = 1; y < h; y++) {
        for (int x = 0; x < w; x++) {
          if (visited[x][y])
            continue;
          map_region_sides(x, y);
        }
      }

      uint32_t price = 0;
      for (const auto& region : regions)
        price += region.area * region.sides;

      return price;
    }
};

TEST_F(Day12, Part1Example) {
  input = "RRRRIICCFF\n"
          "RRRRIICCCF\n"
          "VVRRRCCFFF\n"
          "VVRCCCJFFF\n"
          "VVVVCJJCFE\n"
          "VVIVCCJJEE\n"
          "VVIIICJJEE\n"
          "MIIIIIJJEE\n"
          "MIIISIJEEE\n"
          "MMMISSJEEE";
  SetUp();

  uint32_t price = getPrice();
  EXPECT_EQ(price, 1930);
  EXPECT_EQ(regions.size(), 11);

  input = "OOOOO\n"
          "OXOXO\n"
          "OOOOO\n"
          "OXOXO\n"
          "OOOOO";
  SetUp();

  price = getPrice();
  EXPECT_EQ(price, 1 * 4 * 4 + 21 * 36);
  EXPECT_EQ(regions.size(), 5);
}

TEST_F(Day12, Part1) {
  uint32_t price = getPrice();
  EXPECT_EQ(price, 1450816);
}

TEST_F(Day12, Part2Example) {
  return;
  uint32_t price = 0;

  input = "AAAA\n"
          "BBCD\n"
          "BBCC\n"
          "EEEC";
  SetUp();

  price = getDiscountedPrice();
  EXPECT_EQ(price, 80);

  input = "OOOOO\n"
          "OXOXO\n"
          "OOOOO\n"
          "OXOXO\n"
          "OOOOO";
  SetUp();

  price = getDiscountedPrice();
  EXPECT_EQ(price, 436);

  input = "EEEEE\n"
          "EXXXX\n"
          "EEEEE\n"
          "EXXXX\n"
          "EEEEE";
  SetUp();

  price = getDiscountedPrice();
  EXPECT_EQ(price, 236);

  input = "AAAAAA"
          "AAABBA"
          "AAABBA"
          "ABBAAA"
          "ABBAAA"
          "AAAAAA";
  SetUp();

  price = getDiscountedPrice();
  EXPECT_EQ(price, 368);
}