#ifndef RAYMARCHER_GLFWCAMERA_HPP
#define RAYMARCHER_GLFWCAMERA_HPP

#include <memory>
#include "rendering/ICamera.hpp"
#include "application/IGLFWInputHandler.hpp"

namespace raymarcher
{
/**
 * @brief Unified controller over various types of cameras
 *
 * Translates GLFW inputs to camera-native methods
 */
class GLFWCamera: public IGLFWInputHandler, public ICamera
{
    public:
    enum CameraType
    {
        ORBITER_CAMERA,         // OrbitCamera
        FLYING_CAMERA,          // FlyingCamera
        INTERPOLATED_CAMERA     // InterpolatedCamera
    };

    GLFWCamera(std::shared_ptr<ICamera> camera, CameraType type);
    auto GetType() const -> CameraType;

    /// Update camera once per frame
    auto UpdateFrame() -> void;

    /* ICamera override */
    auto GetTransformation() const -> const glm::mat4& override;

    /* IGLFWInputHandler override */
    auto MouseCursorChanged(GLFWwindow* window, double relativeX, double relativeY) const -> void override;
    auto ScrollChanged(GLFWwindow* window, double relativeX, double relativeY) const -> void override;
    auto KeyPressed(GLFWwindow* window, int key) const -> void override;

    private:
    CameraType type;
    std::shared_ptr<ICamera> camera;
};
}
#endif
