#include <algorithm>
#include <cctype>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <memory>
#include <numeric>
#include <optional>
#include <set>
#include <sstream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

using namespace std;

#include <gtest/gtest.h>

string         getInputFile(int year, int day);
vector<string> split(const string& s, char delimiter);

struct config {
    string input_path;
};

extern config g_config;

class timer {
  public:
    timer();
    ~timer();

    void start();
    void stop();
    int  ms() const;

  private:
    chrono::time_point<chrono::high_resolution_clock> _start;
    chrono::time_point<chrono::high_resolution_clock> _stop;
    bool                                              stopped = false;
};

std::ostream& operator<<(std::ostream& os, const timer& timer);