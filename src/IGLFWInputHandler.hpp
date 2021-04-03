#ifndef RAYMARCHER_IGLFWINPUTHANDLER_HPP
#define RAYMARCHER_IGLFWINPUTHANDLER_HPP

#include <GLFW/glfw3.h>

namespace raymarcher
{
class IGLFWInputHandler
{
    public:
    virtual auto MouseCursorChanged(GLFWwindow* window, double relativeX, double relativeY) const -> void = 0;
    virtual auto ScrollChanged(GLFWwindow* window, double relativeX, double relativeY) const -> void = 0;
    virtual auto KeyPressed(GLFWwindow* window, int key) const -> void = 0;
};
}
#endif
