#include "common.h"

class Day12 : public testing::Test {
  protected:
    using point = point_int;

    struct region {
        char     crop;
        uint32_t area  = 0;
        uint32_t perim = 0;

        region(char _c) : crop(_c) {};
    };

    vector<term::string> plot;
    int                  w;
    int                  h;
    vector<vector<bool>> visited;
    vector<region>       regions;

    void SetUp() override {
      auto   fname = getInputFile(2024, 12);
      auto   f     = ifstream(fname);
      string s;
      while (getline(f, s))
        plot.push_back(s);
    };

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

    int calcPerim(char crop, int x, int y) {
      int perim = 0;

      // left
      if (x == 0)
        perim++;
      else if (x != 0 && plot[y][x - 1] != crop)
        perim++;

      // right
      if (x + 1 == w)
        perim++;
      else if (x + 1 < w && plot[y][x + 1] != crop)
        perim++;

      // up
      if (y == 0)
        perim++;
      else if (y != 0 && plot[y - 1][x] != crop)
        perim++;

      // bottom
      if (y + 1 == h)
        perim++;
      else if (y + 1 < h && plot[y + 1][x] != crop)
        perim++;

      return perim;
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
        region.perim += calcPerim(region.crop, x, y);

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

    uint32_t getPrice() {
      regions.clear();
      visited.clear();

      h = plot.size();
      w = plot[0].length();

      visited.resize(h);
      for (auto& v : visited)
        v.resize(w, false);

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
};

TEST_F(Day12, Part1Example) {
  plot = {"RRRRIICCFF",
          "RRRRIICCCF",
          "VVRRRCCFFF",
          "VVRCCCJFFF",
          "VVVVCJJCFE",
          "VVIVCCJJEE",
          "VVIIICJJEE",
          "MIIIIIJJEE",
          "MIIISIJEEE",
          "MMMISSJEEE"};

  uint32_t price = getPrice();
  EXPECT_EQ(price, 1930);
  EXPECT_EQ(regions.size(), 11);

  plot = {
      "OOOOO",
      "OXOXO",
      "OOOOO",
      "OXOXO",
      "OOOOO",
  };

  price = getPrice();
  EXPECT_EQ(price, 1 * 4 * 4 + 21 * 36);
  EXPECT_EQ(regions.size(), 5);
}

TEST_F(Day12, Part1) {
  uint32_t price = getPrice();
  EXPECT_EQ(price, 1450816);
}