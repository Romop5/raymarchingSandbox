#ifndef RAYMARCHER_RAYMARCHER_HELPER_HPP
#define RAYMARCHER_RAYMARCHER_HELPER_HPP

#include <string>

namespace raymarcher
{
std::string ConstructRenderedVertexShader();
std::string ConstructRenderedFragmentShader(std::string sdfGLSL);
}

#endif
