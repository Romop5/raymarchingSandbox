#ifndef RAYMARCHER_SANDBOXAPPLICATION_HPP
#define RAYMARCHER_SANDBOXAPPLICATION_HPP

#include "application/IApplication.hpp"
#include "bindings/ImguiAdapter.hpp"
#include "rendering/GLFWCamera.hpp"
#include "rendering/OrbitCamera.hpp"
#include "widgets/WidgetManager.hpp"

namespace raymarcher {

class GLFWCamera;
class WidgetManager;

class SandboxApplication : public IApplication
{
public:
  SandboxApplication();
  virtual ~SandboxApplication() = default;
  virtual auto Resize(size_t newWidth, size_t newHeight) -> void override;
  virtual auto Render() -> void override;

  virtual auto CharacterPressed(GLFWwindow* window, unsigned int character)
    -> void override;
  virtual auto MouseCursorChanged(GLFWwindow* window,
                                  double absoluteX,
                                  double absoluteY) -> void override;
  virtual auto MouseButtonPressed(GLFWwindow* window, int button, int action)
    -> void override;
  virtual auto ScrollChanged(GLFWwindow* window,
                             double relativeX,
                             double relativeY) -> void override;
  virtual auto KeyPressed(GLFWwindow* window, int key, int action, int mod)
    -> void override;

  virtual auto SetFocus(std::shared_ptr<IGLFWInputHandler> handler)
    -> void override;
  virtual auto GetFocus() -> std::shared_ptr<IGLFWInputHandler> override;

  virtual auto ShouldContinue() -> bool override;
  virtual auto RequestExit() -> void override;

private:
  raymarcher::ImguiAdapter adapter;
  raymarcher::WidgetManager widgetManager;
  std::shared_ptr<raymarcher::GLFWCamera> focusedCamera;
  std::shared_ptr<IGLFWInputHandler> inputHandler;

  size_t width;
  size_t height;

  bool should_continue;
};
}
#endif
