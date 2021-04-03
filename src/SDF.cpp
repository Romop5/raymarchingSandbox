#include "SDF.hpp"

using namespace raymarcher;

auto SDF::GetGLSLCode() -> const std::string&
{
    return "";
}

auto SDF::OnUpdateParametersCallback(ge::gl::Program& program) -> void
{
    // Pass
}
