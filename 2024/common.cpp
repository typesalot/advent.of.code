#include "common.h"

config g_config;

vector<string> split(const string& s, char delimiter) {
  vector<string> tokens;
  string         token;
  istringstream  tokenStream(s);
  while (getline(tokenStream, token, delimiter)) {
    if (token == "")
      continue;
    tokens.push_back(token);
  }
  return tokens;
}

uint64_t concat_uints(uint64_t a, uint32_t b) {
  int _b = b;
  while (_b) {
    _b /= 10;
    a *= 10;
  }
  return a + b;
}

string getInputFile(int year, int day) {
  filesystem::path asset_base = g_config.input_path;
  string           y          = format("{}", year);
  string           d          = format("day{:02}.txt", day);
  filesystem::path asset_path = asset_base / y / d;
  if (!filesystem::exists(asset_path))
    throw runtime_error(format("File Not Found! {}", asset_path.string()));
  return asset_path;
}

timer::timer() {
  start();
}

timer::~timer() {
  stop();
}

void timer::start() {
  _start = chrono::high_resolution_clock::now();
  _stop  = chrono::high_resolution_clock::now();
}

void timer::stop() {
  if (!stopped) {
    _stop   = chrono::high_resolution_clock::now();
    stopped = true;
  }
}

int timer::ms() const {
  auto start = chrono::time_point_cast<chrono::milliseconds>(_start).time_since_epoch().count();
  auto end   = (stopped) ? chrono::time_point_cast<chrono::milliseconds>(_stop).time_since_epoch().count()
                         : chrono::time_point_cast<chrono::milliseconds>(chrono::high_resolution_clock::now())
                             .time_since_epoch()
                             .count();
  return end - start;
}

ostream& operator<<(ostream& os, const timer& _timer) {
  os << _timer.ms();
  return os;
}

decltype(timer_f::pump)    timer_f::pump;
decltype(timer_f::ref_cnt) timer_f::ref_cnt = 0;

timer_f::timer_f(const string& pmsg, const function<void()>& _f) : prefix(pmsg), f(_f) {
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
      max_s = max<int>(max_s, s.size());
      max_v = max<int>(max_v, format("{}", v).length());
    }
    for (const auto& [s, v] : pump)
      cout << format("{:>{}} = {:<{}} ms", s, max_s + indent, v, max_v) << endl;
  }
}
