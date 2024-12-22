#include "test.h"
#include <filesystem>
#include <format>
#include <stdexcept>

using namespace std;

bool parse_commandline(int argc, char** argv);

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);

  bool run_tests = parse_commandline(argc, argv);

  int ret_val = 0;
  if (run_tests)
    ret_val = RUN_ALL_TESTS();

  return ret_val;
}

bool parse_commandline(int argc, char** argv) {
  try {
    for (int i = 1; i < argc; ++i) {
      string arg(argv[i]);
      if (arg == "--debug")
        g_config.debug = true;
      else if (arg == "--inputs") {
        if (i + 1 >= argc)
          throw runtime_error("Missing path for --inputs");

        g_config.input_path = string(argv[++i]);
      }
    }

    if (g_config.input_path == "")
      throw runtime_error("Use --inputs <path> to define the base directory for inputs.");
    if (!filesystem::exists(g_config.input_path))
      throw runtime_error(format("Unable to find input path '{}'", g_config.input_path));

  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << '\n';
    return false;
  }

  return true;
}