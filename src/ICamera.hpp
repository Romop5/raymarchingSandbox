#ifndef RAYMARCHER_ICAMERA_HPP
#define RAYMARCHER_ICAMERA_HPP
#include <glm/glm.hpp>

namespace raymarcher
{
class ICamera
{
    public:
    virtual auto GetTransformation() const -> const glm::mat4& = 0;
    virtual ~ICamera() = default;
};
}
#endif
