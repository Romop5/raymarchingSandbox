#ifndef RAYMARCHER_SDFLIBRARY_HPP
#define RAYMARCHER_SDFLIBRARY_HPP

#include <vector>
#include <map>
#include <string>

namespace raymarcher
{

class SDFLibrary
{
public:
    SDFLibrary();
    SDFLibrary(std::vector<std::string> primitive, std::vector<std::string> binaryOps);

    const std::vector<std::string> GetPrimitiveNames() const;
    const std::string GetPrimitive(std::string);
private:
    std::string ParseName(std::string GLSLcode);
    std::map<std::string,std::string> primitives;
    std::map<std::string,std::string> binaryOps;
};
}
#endif
