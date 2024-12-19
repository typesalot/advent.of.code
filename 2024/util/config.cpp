#include "config.h"
#include <filesystem>
#include <format>
#include <stdexcept>

config g_config;

std::string getInputFile(uint32_t year, uint32_t day) {
  std::filesystem::path asset_base = g_config.input_path;
  std::string           y          = std::format("{}", year);
  std::string           d          = std::format("day{:02}.txt", day);
  std::filesystem::path asset_path = asset_base / y / d;
  if (!std::filesystem::exists(asset_path))
    throw std::runtime_error(std::format("File Not Found! {}", asset_path.string()));
  return asset_path;
}