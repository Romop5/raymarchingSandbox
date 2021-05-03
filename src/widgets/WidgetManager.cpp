#include "WidgetManager.hpp"
#include <imgui.h>

#include "helpers/observer.hpp"

using namespace raymarcher;


WidgetManager::WidgetManager() = default;

WidgetManager::~WidgetManager() = default;

auto WidgetManager::AddWidget(std::shared_ptr<raymarcher::WidgetBase> newWidget) -> void
{
    auto observer = std::make_unique<Observer<bool>>();
    *observer = newWidget->GetOnExitEventRegister().Register([&, newWidget](bool okay)
    {
        auto id = newWidget->GetID();
        this->RemoveWidget(id);
    });
    widgets.emplace_back(std::move(WidgetEntry { newWidget, std::move(observer)}));
}

auto WidgetManager::HasWidget(WidgetBase::WidgetID id) -> bool
{
    for(auto& entry: widgets)
    {
        auto& widget = entry.widget;
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
        auto& widget = (*it).widget;
        if(widget && widget->GetID() == id)
        {
            widgets.erase(it);
            return;
        }
    }

}

auto WidgetManager::RemoveAllWidgets() -> void
{
    widgets.clear();
}


auto WidgetManager::Render() -> void
{
    for(auto& entry: widgets)
    {
        auto& widget = entry.widget;
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

auto WidgetManager::HasAnyWidget() const -> bool
{
    return !widgets.empty();
}
