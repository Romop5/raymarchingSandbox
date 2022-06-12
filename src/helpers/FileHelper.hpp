#ifndef RAYMARCHER_FILEHELPER_HPP
#define RAYMARCHER_FILEHELPER_HPP

#include <optional>
#include <string>

namespace raymarcher {
/**
 * @brief Helper for storing/loading files
 */
class FileHelper
{
public:
  static auto LoadFile(const std::string& path) -> std::optional<std::string>;
  static auto SaveFile(const std::string& path, const std::string& content)
    -> bool;
};
}

#endif
