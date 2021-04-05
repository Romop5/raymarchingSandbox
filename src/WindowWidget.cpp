#include "WindowWidget.hpp"
#include <imgui.h>

using namespace raymarcher;

WindowWidget::WindowWidget() :
    title { "WindowWidget" }
{

}

auto WindowWidget::Render() -> void
{
    auto id = std::to_string(reinterpret_cast<size_t>(static_cast<void*>(this)));
    auto label = title + "###" + id;

    if(ImGui::Begin(label.c_str(), nullptr))
    {
        RenderContent();
    }
    ImGui::End();
}

auto WindowWidget::RenderContent() -> void
{
    WidgetManager::Render();
}

auto WindowWidget::SetTitle(const std::string& title) -> void
{
    this->title = title;
}
