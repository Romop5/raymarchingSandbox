#include <filesystem>

#include "helpers/FileHelper.hpp"
#include "widgets/EditWidget.hpp"
#include "widgets/WindowWidget.hpp"
#include <imgui.h>

#include "LoadFileWidget.hpp"

using namespace raymarcher;

LoadFileWidget::LoadFileWidget(WidgetManager& windowManager)
  : windowManager_(windowManager)
{
  SetTitle("Load file");
}

auto
LoadFileWidget::RenderContent() -> void
{
  static std::vector<std::string> fileNames;
  static std::filesystem::path selectedFile;

  ImGui::Text("Load SDF function from disk");
  ImGui::Separator();

  const auto size = ImVec2{ 0, 300 };
  ImGui::SetNextWindowSize(size);
  if (ImGui::BeginListBox("")) {

    for (const auto& entry :
         std::filesystem::directory_iterator(std::filesystem::current_path())) {
      const auto path = entry.path();
      const auto filename = path.filename();
      if (filename.extension() != ".sdf") {
        continue;
      }
      if (ImGui::Selectable(filename.c_str())) {
        selectedFile = path;
      }
    }
    ImGui::EndListBox();
  }

  ImGui::Text(selectedFile.filename().c_str());
  ImGui::SameLine();
  if (ImGui::Button("Load")) {
    LoadFile(selectedFile);
    Delete();
  }

  ImGui::SameLine();
  if (ImGui::Button("Exit")) {
    Delete();
  }
}

auto
LoadFileWidget::LoadFile(std::string path) -> void
{
  auto content = FileHelper::LoadFile(path);
  if (!content.has_value()) {
    return;
  }
  std::filesystem::path filePath(path);
  auto widget = std::make_shared<raymarcher::EditWidget>(
    filePath.filename(), content.value(), filePath.filename());
  widget->SetTitle(filePath.filename());
  widget->Recompile();
  windowManager_.AddWidget(widget);
}