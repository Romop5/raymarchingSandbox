#ifndef RAYMARCHER_SANDBOXAPPLICATION_HPP
#define RAYMARCHER_SANDBOXAPPLICATION_HPP

#include "IApplication.hpp"

#include "OrbitCamera.hpp"
#include "GLFWCamera.hpp"
#include "ImguiAdapter.hpp"
#include "WidgetManager.hpp"

namespace raymarcher
{
class GLFWCamera;
class WidgetManager;

class SandboxApplication: public IApplication
{
    public:
    SandboxApplication();
    virtual ~SandboxApplication() = default;
    virtual auto Resize(size_t newWidth, size_t newHeight) -> void override;
    virtual auto Render() -> void override;

    virtual auto MouseCursorChanged(GLFWwindow* window, double absoluteX, double absoluteY) -> void override;
    virtual auto MouseButtonPressed(GLFWwindow* window, int button, int action) -> void override; 
    virtual auto ScrollChanged(GLFWwindow* window, double relativeX, double relativeY) -> void override;
    virtual auto KeyPressed(GLFWwindow* window, int key, int action) -> void override;
    private:
    raymarcher::ImguiAdapter adapter;
    raymarcher::WidgetManager widgetManager;
    std::shared_ptr<raymarcher::GLFWCamera> focusedCamera;
};
}
#endif