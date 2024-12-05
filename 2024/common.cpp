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

std::ostream& operator<<(std::ostream& os, const timer& _timer) {
  os << _timer.ms();
  return os;
}