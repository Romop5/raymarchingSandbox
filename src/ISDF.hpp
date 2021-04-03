#ifndef RAYMARCHER_ISDF_HPP
#define RAYMARCHER_ISDF_HPP

#include "geGL/geGL.h"
namespace raymarcher 
{
/**
 * @brief Represents wrapper over SDF function
 */
class ISDF
{
    public:
    virtual auto GetGLSLCode() -> const std::string& = 0;
    virtual auto OnUpdateParametersCallback(ge::gl::Program& program) -> void = 0;
};

}

#endif
