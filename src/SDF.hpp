#ifndef RAYMARCHER_SDF_HPP
#define RAYMARCHER_SDF_HPP

#include "ISDF.hpp"
namespace raymarcher 
{
/**
 * @brief Represents wrapper over SDF function
 */
class SDF: public ISDF
{
    public:
    explicit SDF(std::string sourceCode);
    auto GetGLSLCode() -> const std::string& override;
    auto OnUpdateParametersCallback(ge::gl::Program& program) -> void override;

    private:
    std::string sourceCode;
};

}

#endif
