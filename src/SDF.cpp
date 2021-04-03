#include "SDF.hpp"

using namespace raymarcher;

SDF::SDF(std::string code) :
    sourceCode { code} 

{
}

auto SDF::GetGLSLCode() -> const std::string&
{
    return sourceCode;
}

auto SDF::OnUpdateParametersCallback(ge::gl::Program& program) -> void
{
    // Pass
}
