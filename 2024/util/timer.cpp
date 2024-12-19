#include "timer.h"
#include <algorithm>
#include <format>
#include <iostream>

timer::timer() {
  start();
}

timer::~timer() {
  stop();
}

void timer::start() {
  _start = std::chrono::high_resolution_clock::now();
  _stop  = std::chrono::high_resolution_clock::now();
}

void timer::stop() {
  if (!stopped) {
    _stop   = std::chrono::high_resolution_clock::now();
    stopped = true;
  }
}

int timer::ms() const {
  auto start = std::chrono::time_point_cast<std::chrono::milliseconds>(_start).time_since_epoch().count();
  auto end   = (stopped)
                   ? std::chrono::time_point_cast<std::chrono::milliseconds>(_stop).time_since_epoch().count()
                   : std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now())
                       .time_since_epoch()
                       .count();
  return end - start;
}

std::ostream& operator<<(std::ostream& os, const timer& _timer) {
  os << _timer.ms();
  return os;
}

decltype(timer_f::pump)    timer_f::pump;
decltype(timer_f::ref_cnt) timer_f::ref_cnt = 0;

timer_f::timer_f(const std::string& pmsg, const std::function<void()>& _f) : prefix(pmsg), f(_f) {
  timer_f::ref_cnt++;
  timer::start();
  f();
  timer::stop();
}

timer_f::~timer_f() {
  pump.emplace_back(this->prefix, this->ms());

  timer_f::ref_cnt--;

  int indent = 2;

  if (timer_f::ref_cnt == 0 && timer_f::pump.size()) {
    int max_s = 0;
    int max_v = 0;
    for (const auto& [s, v] : pump) {
      max_s = std::max<int>(max_s, s.size());
      max_v = std::max<int>(max_v, std::format("{}", v).length());
    }
    for (const auto& [s, v] : pump)
      std::cout << std::format("{:>{}} = {:<{}} ms", s, max_s + indent, v, max_v) << std::endl;
  }
}
