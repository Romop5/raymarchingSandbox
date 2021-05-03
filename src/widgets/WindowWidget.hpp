#ifndef RAYMARCHER_WINDOWWIDGET_HPP
#define RAYMARCHER_WINDOWWIDGET_HPP
#include <string>
#include "widgets/WidgetBase.hpp"
#include "WidgetManager.hpp"

namespace raymarcher
{
class WindowWidget: public WidgetBase, public WidgetManager
{
    public:
    WindowWidget();
    ~WindowWidget() = default;
    virtual auto Render() -> void override;
    virtual auto RenderContent() -> void override;

    auto SetSize(float width, float height) -> void;
    auto SetPosition(float posx, float posy) -> void;

    auto SetTitle(const std::string& title) -> void;
    auto GetTitle() const -> const std::string;

    private:
    std::string title;
    size_t width;
    size_t height;

    size_t posx;
    size_t posy;
};
}
#endif
