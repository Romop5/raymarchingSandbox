#include "WidgetManager.hpp"

using namespace raymarcher;

auto WidgetManager::AddWidget(std::shared_ptr<raymarcher::IWidget> newWidget) -> void
{
    widgets.push_back(newWidget);
}

auto WidgetManager::Render() -> void
{
    for(auto& widget: widgets)
    {
        if(widget)
            widget->Render();
    }
}
