#pragma once

#include <algorithm>
#include <cctype>
#include <chrono>
#include <cstdlib>
#include <deque>
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
#include <queue>
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

#include "util/point.h"
#include "util/term.h"

string         getInputFile(int year, int day);
vector<string> split(const string& s, char delimiter);
uint64_t       concat_uints(uint64_t a, uint32_t b);

struct config {
    string input_path;
    bool   debug = false;
};

extern config g_config;

class timer {
  public:
    timer();
    ~timer();

    virtual void start();
    void         stop();
    int          ms() const;

    friend ostream& operator<<(ostream& os, const timer& timer);

  private:
    chrono::time_point<chrono::high_resolution_clock> _start;
    chrono::time_point<chrono::high_resolution_clock> _stop;
    bool                                              stopped = false;
};

class timer_f : public timer {
  public:
    timer_f(const string& prefix, const function<void()>& f);
    ~timer_f();

  private:
    function<void()> f;
    string           prefix;

    static vector<pair<string, int>> pump;
    static int                       ref_cnt;
};