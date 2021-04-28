#include "FileHelper.hpp"
#include <filesystem>
#include <fstream>

using namespace raymarcher;

auto FileHelper::LoadFile(std::string path) -> std::optional<std::string>
{
    std::filesystem::path filePath = path;
    std::ifstream file;
    file.open(path, std::ifstream::in);
    if(!file.is_open())
    {
        return {};
    }

    std::stringstream content;
    std::string line;
    while(file)
    {
        std::getline(file, line);
        if(!file)
            break;
        content << line << std::endl;
    }
    return std::make_optional(content.str());
}
