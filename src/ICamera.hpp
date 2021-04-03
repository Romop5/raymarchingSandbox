#ifndef RAYMARCHER_ICAMERA_HPP
#define RAYMARCHER_ICAMERA_HPP
#include <glm/glm.hpp>

class ICamera
{
    public:
    virtual auto GetTransformation() const -> const glm::mat4& = 0;
};
#endif
