#ifndef RAYMARCHER_TESTAPPLICATION_HPP
#define RAYMARCHER_TESTAPPLICATION_HPP

#include "application/IApplication.hpp"

#include "ImguiAdapter.hpp"
#include "raymarching/Raymarcher.hpp"
#include "rendering/GLFWCamera.hpp"
#include "rendering/OrbitCamera.hpp"
#include "widgets/FPSMeter.hpp"
#include "widgets/RendererAttributesWidget.hpp"

namespace raymarcher {
class GLFWCamera;
class WidgetManager;

class TestApplication : public IApplication
{
public:
  struct StartParameters
  {
    std::string filename = "sphere.sdf";
    bool shouldRunWithFreeMovement = false;
    size_t cameraSequenceID = 1;
  };
  explicit TestApplication(StartParameters params);
  virtual ~TestApplication() = default;
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

private:
  raymarcher::ImguiAdapter adapter;
  std::unique_ptr<raymarcher::GLFWCamera> focusedCamera;
  std::shared_ptr<IGLFWInputHandler> inputHandler;

  std::shared_ptr<Raymarcher> raymarcher;
  std::shared_ptr<GLFWCamera> camera;
  std::unique_ptr<RendererAttributesWidget> attributes;
  FPSMeter fpsMeter;

  StartParameters parameters;

  bool shouldContinue = true;
  bool isGUIControlActive = false;
};
}
#endif
