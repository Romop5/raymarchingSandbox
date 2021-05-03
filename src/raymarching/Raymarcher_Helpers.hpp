#ifndef RAYMARCHER_RAYMARCHER_HELPER_HPP
#define RAYMARCHER_RAYMARCHER_HELPER_HPP

#include <string>

namespace raymarcher
{
/// Construct Fragment Shader using user SDF and predefined functions
std::string ConstructRenderedFragmentShader(std::string sdfGLSL);

/// Create a simple VS for quad rendering
std::string ConstructRenderedVertexShader();
}

#endif
