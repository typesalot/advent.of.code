#pragma once
#include "util/config.h"
#include <fstream>
#include <gtest/gtest.h>
#include <sstream>
#include <string>

template <int year>
class aoc_test : public ::testing::Test {
  protected:
    std::string input;

    virtual void LoadInput(std::istringstream& input) = 0;

    void SetUp() override {
      const ::testing::TestInfo* test_info = ::testing::UnitTest::GetInstance()->current_test_info();

      if (test_info->name() == std::string("Part1") || test_info->name() == std::string("Part2")) {
        auto fname = getInputFile(year, getDay());
        auto f     = std::ifstream(fname);

        std::ostringstream oss;
        oss << f.rdbuf();
        input = oss.str();
        f.close();
      }

      if (input != "") {
        std::istringstream iss(input);
        this->LoadInput(iss);
      }
    }

  private:
    uint32_t getDay() {
      const ::testing::TestInfo* test_info = ::testing::UnitTest::GetInstance()->current_test_info();
      const std::string&         suite     = std::string(test_info->test_suite_name());

      std::string day   = suite.substr(3, suite.length() - 3);
      uint32_t    value = stoi(day);
      return value;
    }
};

using aoc_2024 = aoc_test<2024>;