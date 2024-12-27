#pragma once
#include <string>
#include <vector>

std::vector<std::string> split(const std::string& s, char delimiter, bool _trim = false);
std::string              trim(const std::string& str);