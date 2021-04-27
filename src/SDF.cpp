#include "SDF.hpp"
#include <regex>

using namespace raymarcher;

SDF::SDF(std::string code) :
    sourceCode { code} 

{
    ParseCode();
}

auto SDF::GetGLSLCode() -> const std::string&
{
    return sourceCode;
}

auto SDF::OnUpdateParametersCallback(ge::gl::Program& program) -> void
{
    // Pass
}

auto SDF::GetUniforms() const -> const std::vector<Uniform>&
{
    return userUniforms;
}

auto SDF::ParseCode() -> void
{
    // Parse uniforms
    userUniforms.clear();
    auto str = sourceCode;
    auto uniformDefinition = std::regex("uniform *([^ ]*) *([a-zA-Z0-9]*)( *=.*)?;");
    std::smatch match;
    while(regex_search(str, match, uniformDefinition))
    {
        const Uniform uniform {match[2], match[1]};
        userUniforms.push_back(uniform);
        str = match.suffix();
    }
}
