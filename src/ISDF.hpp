#ifndef RAYMARCHER_ISDF_HPP
#define RAYMARCHER_ISDF_HPP

#include "geGL/geGL.h"
namespace raymarcher 
{
/**
 * @brief Represents wrapper over SDF function source code
 *
 * Provides GetUniforms() which returns user-defined uniforms.
 */
class ISDF
{
    public:
    /// Helper: represents uniform with type and name
    struct Uniform
    {
        std::string name;
        std::string type;
    };

    /// Return stored code
    virtual auto GetGLSLCode() -> const std::string& = 0;
    virtual auto OnUpdateParametersCallback(ge::gl::Program& program) -> void = 0;

    /// Return list of uniforms, defined in this code
    virtual auto GetUniforms() const -> const std::vector<Uniform>& = 0;
};

}

#endif
