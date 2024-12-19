#pragma once
#include <string>

struct config {
    std::string input_path;
    bool        debug = false;
};

extern config g_config;

std::string getInputFile(uint32_t year, uint32_t day);