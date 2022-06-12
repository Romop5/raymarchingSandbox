#include "raymarching/SDFLibrary.hpp"

using namespace raymarcher;

SDFLibrary::SDFLibrary() {}

SDFLibrary::SDFLibrary(std::vector<std::string> newPrimitive,
                       std::vector<std::string> newBinaryOps)
{
  for (auto& primitive : newPrimitive) {
    auto name = ParseName(primitive);
    primitives[name] = primitive;
  }
}

const std::vector<std::string>
SDFLibrary::GetPrimitiveNames() const
{
  std::vector<std::string> names;
  for (auto& name : primitives) {
    names.push_back(name.first);
  }
  return names;
}

const std::string
SDFLibrary::GetPrimitive(std::string key)
{
  if (primitives.count(key)) {
    return primitives.at(key);
  }
  return "";
}

std::string
SDFLibrary::ParseName(std::string GLSLcode)
{
  auto posStart = GLSLcode.find("float");
  auto posEnd = GLSLcode.find("(");
  if (posStart && posEnd) {
    return GLSLcode.substr(posStart, posEnd - posStart);
  }
  static size_t id = 0;
  return "Unknown " + std::to_string(id++);
}
