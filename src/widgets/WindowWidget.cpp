#include <imgui.h>

#include "widgets/WindowWidget.hpp"

using namespace raymarcher;

WindowWidget::WindowWidget()
  : title{ "WindowWidget" }
  , width{ 100 }
  , height{ 100 }
  , posx{ 100 }
  , posy{ 100 }
{}

auto
WindowWidget::Render() -> void
{
  auto id = std::to_string(reinterpret_cast<size_t>(static_cast<void*>(this)));
  auto label = title + "###" + id;

  bool shouldContinue = true;
  ImGui::SetNextWindowSize(ImVec2(width, height), ImGuiCond_Once);
  ImGui::SetNextWindowPos(ImVec2(posx, posy), ImGuiCond_Once);
  if (ImGui::Begin(label.c_str(), &shouldContinue)) {
    RenderContent();
  }
  ImGui::End();

  if (!shouldContinue) {
    Delete();
  }
}

auto
WindowWidget::RenderContent() -> void
{
  WidgetManager::Render();
}

auto
WindowWidget::SetSize(float width, float height) -> void
{
  this->width = width;
  this->height = height;
}

auto
WindowWidget::SetPosition(float posx, float posy) -> void
{
  this->posx = posx;
  this->posy = posy;
}

auto
WindowWidget::SetTitle(const std::string& title) -> void
{
  this->title = title;
}

auto
WindowWidget::GetTitle() const -> const std::string
{
  return title;
}
