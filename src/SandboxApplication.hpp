#ifndef RAYMARCHER_SANDBOXAPPLICATION_HPP
#define RAYMARCHER_SANDBOXAPPLICATION_HPP

#include "IApplication.hpp"

#include "Raymarcher.hpp"
#include "FlyingCamera.hpp"
#include "OrbitCamera.hpp"
#include "GLFWCamera.hpp"
#include "SDF.hpp"
#include "ImguiAdapter.hpp"
#include "RendererWidget.hpp"
#include "WidgetManager.hpp"
#include "IWidget.hpp"

namespace raymarcher
{
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
    std::shared_ptr<raymarcher::GLFWCamera> globalCamera;
    std::unique_ptr<raymarcher::WidgetManager> widgetManager;
};
}
#endif
