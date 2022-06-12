#include "helpers/FileHelper.hpp"
#include <filesystem>
#include <fstream>

using namespace raymarcher;

auto
FileHelper::LoadFile(const std::string& path) -> std::optional<std::string>
{
  std::filesystem::path filePath = path;
  std::ifstream file;
  file.open(path, std::ifstream::in);
  if (!file.is_open()) {
    return {};
  }

  std::stringstream content;
  std::string line;
  while (file) {
    std::getline(file, line);
    if (!file)
      break;
    content << line << std::endl;
  }
  return std::make_optional(content.str());
}

auto
FileHelper::SaveFile(const std::string& path, const std::string& content)
  -> bool
{
  std::fstream outputFile;
  outputFile.open(path, std::fstream::out | std::fstream::trunc);
  if (!outputFile.is_open()) {
    return false;
  }
  outputFile << content;
  if (!outputFile.good()) {
    return false;
  }
  return true;
}
