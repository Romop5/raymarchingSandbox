#ifndef RAYMARCHER_WIDGETBASE_HPP
#define RAYMARCHER_WIDGETBASE_HPP
#include <glm/glm.hpp>

namespace raymarcher
{
class WidgetBase
{
    public:
    WidgetBase();
    ~WidgetBase() = default;

    using WidgetID = unsigned long;
    auto GetID() -> WidgetID;

    virtual auto Render() -> void;
    protected:
    virtual auto RenderContent() -> void;

    private:
    WidgetID id;
};
}
#endif
