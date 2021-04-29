#ifndef RAYMARCHER_FILEHELPER_HPP
#define RAYMARCHER_FILEHELPER_HPP

#include <string>
#include <optional>

namespace raymarcher
{
class FileHelper
{
    public:
    static auto LoadFile(const std::string& path) -> std::optional<std::string>;
    static auto SaveFile(const std::string& path, const std::string& content) -> bool;
};
}

#endif
