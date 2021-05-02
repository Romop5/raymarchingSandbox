#ifndef RAYMARCHER_ICAMERA_HPP
#define RAYMARCHER_ICAMERA_HPP
#include <glm/glm.hpp>

namespace raymarcher
{

/**
 * @brief Generic camera interface
 *
 * A generic camere only returns its view transformation 
 */
class ICamera
{
    public:
    virtual ~ICamera() = default;
    virtual auto GetTransformation() const -> const glm::mat4& = 0;
};
}
#endif
