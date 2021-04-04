#ifndef RAYMARCHER_IAPPLICATION_HPP
#define RAYMARCHER_IAPPLICATION_HPP

#include <cstdlib>

class GLFWwindow;

namespace raymarcher
{
class IApplication
{
    public:
    virtual ~IApplication() = default;
    virtual auto Resize(size_t newWidth, size_t newHeight) -> void = 0;
    virtual auto Render() -> void = 0;

    virtual auto MouseCursorChanged(GLFWwindow* window, double absoluteX, double absoluteY) -> void = 0;
    virtual auto MouseButtonPressed(GLFWwindow* window, int button, int action) -> void = 0; 
    virtual auto ScrollChanged(GLFWwindow* window, double relativeX, double relativeY) -> void = 0;
    virtual auto KeyPressed(GLFWwindow* window, int key, int action) -> void = 0;
};
}
#endif
