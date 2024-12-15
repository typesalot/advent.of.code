#include "common.h"

class Day12 : public testing::Test {
  protected:
    vector<term::string> plot;
    int                  w;
    int                  h;

    struct region {
        char     crop;
        uint32_t area  = 0;
        uint32_t perim = 0;

        region(char _c) : crop(_c) {};
    };

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

    uint32_t getPrice() {
      using point = point_uint;

      uint32_t price = 0;

      h = plot.size();
      w = plot[0].length();

      vector<region>       regions;
      deque<point>         next;
      vector<vector<bool>> visited(h);

      for (auto& v : visited)
        v.resize(w, false);

      regions.emplace_back(plot[0][0]);
      next.emplace_front(0, 0);

      while (!next.empty()) {
        auto& region = regions.back();

        int x = next.front().x;
        int y = next.front().y;
        next.pop_front();

        // skip if visited and update
        if (visited[x][y])
          continue;
        visited[x][y] = true;

        auto current_crop = plot[y][x];
        if (region.crop != current_crop) {
          clear_colors();
          regions.emplace_back(current_crop);
          region = regions.back();
          region.area++;
          // auto& n = regions.back();
          // n.area++;
        }

        if (region.crop == current_crop) {
          region.area++;
          region.perim += calcPerim(region.crop, x, y);
        }

        plot[y].background(x).red();
        print_plot();

        // add unvisited neighbors
        if (x != 0 && !visited[x - 1][y]) {
          if (plot[y][x - 1] == current_crop)
            next.emplace_front(x - 1, y);
          else
            next.emplace_back(x - 1, y);
        }

        if (x + 1 < w && !visited[x + 1][y]) {
          if (plot[y][x + 1] == current_crop)
            next.emplace_front(x + 1, y);
          else
            next.emplace_back(x + 1, y);
        }

        if (y != 0 && !visited[x][y - 1]) {
          if (plot[y - 1][x] == current_crop)
            next.emplace_front(x, y - 1);
          else
            next.emplace_back(x, y - 1);
        }

        if (y + 1 < h && !visited[x][y + 1]) {
          if (plot[y + 1][x] == current_crop)
            next.emplace_front(x, y + 1);
          else
            next.emplace_back(x, y + 1);
        }
      }

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
}

TEST_F(Day12, Part1) {
}