#include <imgui.h>

#include "raymarching/SDF.hpp"
#include "widgets/MenuWidget.hpp"
#include "widgets/WelcomeWidget.hpp"
#include "widgets/WindowWidget.hpp"

#include "application/SandboxApplication.hpp"

using namespace raymarcher;

SandboxApplication::SandboxApplication()
{
  ge::gl::init();

  adapter.Initialize(500, 500);
  width = height = 500;
  adapter.SetVisibility(true);

  auto menuWindow = std::make_shared<WindowWidget>();
  menuWindow->AddWidget(std::make_shared<MenuWidget>(widgetManager));
  menuWindow->SetTitle("Menu");
  menuWindow->SetSize(150, 0);
  widgetManager.AddWidget(menuWindow);

  auto welcome = std::make_shared<WelcomeWidget>(widgetManager);
  welcome->SetSize(500, 0);
  widgetManager.AddWidget(welcome);
}

auto
SandboxApplication::Resize(size_t newWidth, size_t newHeight) -> void
{
  adapter.Resize(newWidth, newHeight);
  width = newWidth;
  height = newHeight;
}

auto
SandboxApplication::Render() -> void
{
  if (adapter.IsVisible()) {
    adapter.BeginFrame();
    widgetManager.Render();
    adapter.EndFrame();
    adapter.RenderCurrentFrame();
  }
}

auto
SandboxApplication::CharacterPressed(GLFWwindow* window, unsigned int character)
  -> void
{
  adapter.OnCharacter(character);
}

auto
SandboxApplication::MouseCursorChanged(GLFWwindow* window,
                                       double absoluteX,
                                       double absoluteY) -> void
{
  static double lastXpos = 0.0, lastYpos = 0.0;
  double relativeX = absoluteX - lastXpos;
  double relativeY = absoluteY - lastYpos;
  lastXpos = absoluteX;
  lastYpos = absoluteY;

  if (inputHandler) {
    inputHandler->MouseCursorChanged(window, relativeX, relativeY);
    return;
  }

  if (absoluteX < 0.0 || absoluteY < 0.0) {
    glfwSetCursorPos(
      window, std::max(0.0, absoluteX), std::max(0.0, absoluteY));
  }
  if (absoluteX > width || absoluteY > height) {
    auto w = static_cast<double>(width);
    auto h = static_cast<double>(height);
    glfwSetCursorPos(window, std::min(w, absoluteX), std::min(h, absoluteY));
  }

  adapter.OnMousePosition(absoluteX, absoluteY);
  if (adapter.IsVisible()) {
    return;
  }
}

auto
SandboxApplication::MouseButtonPressed(GLFWwindow* window,
                                       int button,
                                       int action) -> void
{
  if (adapter.WantCaptureMouse()) {
    adapter.OnButton(button, action == GLFW_PRESS);
  }
}

auto
SandboxApplication::ScrollChanged(GLFWwindow* window,
                                  double relativeX,
                                  double relativeY) -> void
{
  if (inputHandler) {
    inputHandler->ScrollChanged(window, relativeX, relativeY);
    return;
  }

  if (adapter.WantCaptureMouse()) {
    adapter.OnScroll(relativeX, relativeY);
  }
}

auto
SandboxApplication::KeyPressed(GLFWwindow* window, int key, int action, int mod)
  -> void
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    SetFocus(nullptr);
  }
  if (key == GLFW_KEY_F11 && action == GLFW_PRESS) {
    adapter.SetVisibility(!adapter.IsVisible());
    // glfwSetInputMode(window, GLFW_CURSOR,
    // adapter.IsVisible()?GLFW_CURSOR_NORMAL:GLFW_CURSOR_DISABLED);
  }

  if (inputHandler) {
    inputHandler->KeyPressed(window, key);
    return;
  }

  if (adapter.WantCaptureKeyboard()) {
    int imguiMod = ModifierBitmap::NORMAL;
    if (mod & GLFW_MOD_CONTROL)
      imguiMod = imguiMod | ModifierBitmap::CTRL;
    if (mod & GLFW_MOD_SHIFT)
      imguiMod = imguiMod | ModifierBitmap::SHIFT;

    adapter.OnKey(
      key, action == GLFW_PRESS, static_cast<ModifierBitmap>(imguiMod));
    return;
  }
}

auto
SandboxApplication::SetFocus(std::shared_ptr<IGLFWInputHandler> handler) -> void
{
  inputHandler = std::move(handler);
}

auto
SandboxApplication::GetFocus() -> std::shared_ptr<IGLFWInputHandler>
{
  return inputHandler;
}
