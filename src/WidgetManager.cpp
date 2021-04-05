#include "WidgetManager.hpp"

#include <imgui.h>

using namespace raymarcher;

auto WidgetManager::AddWidget(std::shared_ptr<raymarcher::WidgetBase> newWidget) -> void
{
    widgets.push_back(newWidget);
}

auto WidgetManager::HasWidget(WidgetBase::WidgetID id) -> bool
{
    for(auto& widget: widgets)
    {
        if(widget && widget->GetID() == id)
        {
            return true;
        }
    }
    return false;
}

auto WidgetManager::RemoveWidget(WidgetBase::WidgetID id) -> void
{
    for(auto it = widgets.begin(); it != widgets.end(); it++)
    {
        auto& widget = *it;
        if(widget && widget->GetID() == id)
        {
            widgets.erase(it);
            return;
        }
    }

}

auto WidgetManager::Render() -> void
{
    for(auto& widget: widgets)
    {
        if(widget)
        {
            widget->Render();
            if(layout == HORIZONTAL_BOX)
            {
                ImGui::SameLine();
            }
        }
    }
}

auto WidgetManager::SetLayout(LayoutType type) -> void
{
    layout = type;
}
