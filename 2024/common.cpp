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