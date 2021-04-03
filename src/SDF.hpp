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
    auto GetGLSLCode() -> const std::string& override;
    auto OnUpdateParametersCallback(ge::gl::Program& program) -> void override;
};

}

#endif
