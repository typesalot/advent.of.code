#include "string.h"
#include <sstream>

std::vector<std::string> split(const std::string& s, char delimiter, bool _trim) {
  std::vector<std::string> tokens;
  std::string              token;
  std::istringstream       tokenStream(s);
  while (getline(tokenStream, token, delimiter)) {
    if (token == "")
      continue;
    if (_trim)
      tokens.emplace_back(trim(token));
    else
      tokens.emplace_back(token);
  }
  return tokens;
}

std::string trim(const std::string& str) {
  size_t start = str.find_first_not_of(" \t\n\r\f\v");
  size_t end   = str.find_last_not_of(" \t\n\r\f\v");

  return (start == std::string::npos) ? "" : str.substr(start, end - start + 1);
}