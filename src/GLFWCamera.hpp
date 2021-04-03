#ifndef RAYMARCHER_GLFWCAMERA_HPP
#define RAYMARCHER_GLFWCAMERA_HPP

#include <memory>
#include "ICamera.hpp"
#include "IGLFWInputHandler.hpp"

namespace raymarcher
{
class GLFWCamera: public IGLFWInputHandler, public ICamera
{
    public:
    enum CameraType
    {
        ORBITER_CAMERA,
        FLYING_CAMERA
    };

    GLFWCamera(std::shared_ptr<ICamera> camera, CameraType type); 
    auto GetTransformation() const -> const glm::mat4& override;
    auto MouseCursorChanged(GLFWwindow* window, double relativeX, double relativeY) const -> void override;
    auto KeyPressed(GLFWwindow* window, int key) const -> void override;

    private:
    CameraType type;
    std::shared_ptr<ICamera> camera;
};
}
#endif
