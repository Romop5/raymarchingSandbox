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
    struct Uniform
    {
        std::string name;
        std::string type;
    };

    virtual auto GetGLSLCode() -> const std::string& = 0;
    virtual auto OnUpdateParametersCallback(ge::gl::Program& program) -> void = 0;
    virtual auto GetUniforms() const -> const std::vector<Uniform>& = 0;
};

}

#endif
