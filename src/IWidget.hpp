#ifndef RAYMARCHER_IWIDGET_HPP
#define RAYMARCHER_IWIDGET_HPP
#include <glm/glm.hpp>

namespace raymarcher
{
class IWidget 
{
    public:
    ~IWidget() = default;
    virtual auto Render() -> void = 0;
};
}
#endif
