#ifndef RAYMARCHER_IAPPLICATION_HPP
#define RAYMARCHER_IAPPLICATION_HPP

#include <memory>
#include <cstdlib>

class GLFWwindow;

namespace raymarcher
{
class IGLFWInputHandler;

class IApplication
{
    public:
    virtual ~IApplication() = default;
    virtual auto Resize(size_t newWidth, size_t newHeight) -> void = 0;
    virtual auto Render() -> void = 0;

    virtual auto CharacterPressed(GLFWwindow* window, unsigned int character) -> void = 0;
    virtual auto MouseCursorChanged(GLFWwindow* window, double absoluteX, double absoluteY) -> void = 0;
    virtual auto MouseButtonPressed(GLFWwindow* window, int button, int action) -> void = 0; 
    virtual auto ScrollChanged(GLFWwindow* window, double relativeX, double relativeY) -> void = 0;
    virtual auto KeyPressed(GLFWwindow* window, int key, int action, int mod) -> void = 0;

    virtual auto SetFocus(std::shared_ptr<IGLFWInputHandler> handler) -> void  = 0;

    static IApplication& GetApplication();
};
}
#endif
