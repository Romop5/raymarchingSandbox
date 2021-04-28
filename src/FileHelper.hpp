#ifndef RAYMARCHER_FILEHELPER_HPP
#define RAYMARCHER_FILEHELPER_HPP

#include <string>
#include <optional>

namespace raymarcher
{
class FileHelper
{
    public:
    static auto LoadFile(std::string path) -> std::optional<std::string>;
};
}

#endif
