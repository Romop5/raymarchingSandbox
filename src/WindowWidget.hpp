#ifndef RAYMARCHER_WINDOWWIDGET_HPP
#define RAYMARCHER_WINDOWWIDGET_HPP
#include <string>
#include "IWidget.hpp"
#include "WidgetManager.hpp"

namespace raymarcher
{
class WindowWidget: public IWidget, public WidgetManager
{
    public:
    WindowWidget();
    ~WindowWidget() = default;
    virtual auto Render() -> void override;

    auto SetTitle(const std::string& title) -> void;

    private:
    std::string title;
};
}
#endif
