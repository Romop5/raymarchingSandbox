#ifndef RAYMARCHER_IAPPLICATION_HPP
#define RAYMARCHER_IAPPLICATION_HPP

#include <cstdlib>
#include <memory>

// Fwd
class GLFWwindow;

namespace raymarcher {

// Fwd
class IGLFWInputHandler;

/**
 * @brief Interface of a generic application with GLFW window
 */
class IApplication
{
public:
  virtual ~IApplication() = default;

  // On window's size change
  virtual auto Resize(size_t newWidth, size_t newHeight) -> void = 0;

  /// Do visual rendering
  virtual auto Render() -> void = 0;

  /// GLFW window input handler
  virtual auto CharacterPressed(GLFWwindow* window, unsigned int character)
    -> void = 0;
  /// GLFW window input handler
  virtual auto MouseCursorChanged(GLFWwindow* window,
                                  double absoluteX,
                                  double absoluteY) -> void = 0;
  /// GLFW window input handler
  virtual auto MouseButtonPressed(GLFWwindow* window, int button, int action)
    -> void = 0;
  /// GLFW window input handler
  virtual auto ScrollChanged(GLFWwindow* window,
                             double relativeX,
                             double relativeY) -> void = 0;
  /// GLFW window input handler
  virtual auto KeyPressed(GLFWwindow* window, int key, int action, int mod)
    -> void = 0;

  /// Set IGLFWInputHandler as focused input receiver
  virtual auto SetFocus(std::shared_ptr<IGLFWInputHandler> handler) -> void = 0;

  /// Returns current input receiver (or nullptr)
  virtual auto GetFocus() -> std::shared_ptr<IGLFWInputHandler> = 0;

  /// Should we continue or terminate process?
  virtual auto ShouldContinue() -> bool { return true; }

  /// Singleton: only a single application is active per process run
  static IApplication& GetApplication();

  /// Request the application's exit
  virtual auto RequestExit() -> void{};
};
}
#endif
