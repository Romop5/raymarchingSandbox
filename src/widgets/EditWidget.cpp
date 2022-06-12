
#include <imgui.h>

#include "helpers/FileHelper.hpp"
#include "raymarching/Raymarcher.hpp"
#include "raymarching/SDF.hpp"
#include "rendering/GLFWCamera.hpp"
#include "rendering/OrbitCamera.hpp"
#include "widgets/RendererWidget.hpp"
#include <bindings/imgui_stdlib.h>

#include "widgets/EditWidget.hpp"

using namespace raymarcher;

namespace {
auto
CreateAndSetSDF(std::string code, std::shared_ptr<raymarcher::Raymarcher> rm)
  -> void
{
  auto sdf = std::make_shared<raymarcher::SDF>(code);
  rm->SetSDF(sdf);
}
}

EditWidget::EditWidget(std::string name,
                       std::string startingCode,
                       std::string file)
  : code{ startingCode }
  , status{ false, "" }
  , isAutocompileEnabled{ true }
  , filename{ file }
{
  SetTitle(name);
  SetSize(400, 400);
}

auto
EditWidget::RenderContent() -> void
{
  SaveAsSubwidget();
  ImGui::SameLine();
  if (ImGui::Button("Compile")) {
    Recompile();
  }

  ImGui::SameLine();
  ImGui::Checkbox("Auto-Compile", &isAutocompileEnabled);

  if (!status.lastMessage.empty()) {
    ImGui::SameLine();
    std::string msg = status.lastMessage.substr(0, 30);
    if (status.isError) {
      msg = "Error: " + msg;
      ImGui::TextColored(ImVec4(1.0, 0.0, 0.0, 1.0), msg.c_str());
    } else {
      msg = "Status: " + msg;
      ImGui::Text(msg.c_str());
    }
    if (ImGui::IsItemHovered()) {
      ImGui::BeginTooltip();
      ImGui::Text(status.lastMessage.c_str());
      ImGui::EndTooltip();
    }
  }

  ImGui::Text("Code:");
  if (ImGui::InputTextMultiline("Code", &code, ImVec2(-1, -1)) &&
      isAutocompileEnabled) {
    Recompile();
  }

  /// Render subwidgets/subviews (e.g. preview window)
  WindowWidget::RenderContent();
}

auto
EditWidget::Recompile() -> void
{
  SetLastStatus("");
  try {
    rm = (!rm ? std::make_shared<raymarcher::Raymarcher>() : rm);
    CreateAndSetSDF(code, rm);

    // If preview window is not opened, yet
    if (!HasAnyWidget()) {
      AddWidget(CreateRenderWidget(rm));
    }
  } catch (std::exception& error) {
    SetLastError(error.what());
  }
}

auto
EditWidget::SaveAs(std::string filename) -> void
{
  if (filename.empty()) {
    SetLastError("No file entered for this SDF");
    return;
  }
  auto hasBeenSaved = FileHelper::SaveFile(filename, code);
  if (!hasBeenSaved) {
    SetLastError("Failed to save file " + filename);
    return;
  }
  SetLastStatus("Saved");
}

auto
EditWidget::SaveAsSubwidget() -> void
{
  if (ImGui::Button("Save")) {
    SaveAs(filename);
  }
  ImGui::SameLine();

  if (ImGui::Button("Save as")) {
    if (filename.empty()) {
      filename = "new.sdf";
    }
    ImGui::OpenPopup("SVPopup");
  }

  if (ImGui::BeginPopup("SVPopup")) {
    ImGui::Text("Save SDF function to disk");
    ImGui::Separator();
    ImGui::InputText("", &filename);
    if (ImGui::Button("Save")) {
      SaveAs(filename);
      ImGui::CloseCurrentPopup();
    }

    ImGui::SameLine();
    if (ImGui::Button("Exit")) {
      ImGui::CloseCurrentPopup();
    }
    ImGui::EndPopup();
  }
}

auto
EditWidget::SetLastError(std::string error) -> void
{
  status.lastMessage = error;
  status.isError = true;
}
auto
EditWidget::SetLastStatus(std::string msg) -> void
{
  status.lastMessage = msg;
  status.isError = false;
}

auto
EditWidget::CreateRenderWidget(std::shared_ptr<raymarcher::Raymarcher> rm)
  -> std::shared_ptr<WidgetBase>
{
  // Create orbiter as default camera
  auto orbiter = std::make_shared<raymarcher::OrbitCamera>();
  orbiter->SetCenter(glm::vec3(0.0, 3.0, 0.0));
  orbiter->SetDistance(5.0);
  auto focusedCamera = std::make_shared<raymarcher::GLFWCamera>(
    orbiter, raymarcher::GLFWCamera::CameraType::ORBITER_CAMERA);
  rm->SetCamera(focusedCamera);

  // Create preview window
  auto widget = std::make_shared<raymarcher::RendererWidget>(rm, focusedCamera);

  // Determine current size of window, and set preview to half of window
  ImGuiIO& io = ImGui::GetIO();
  auto size = io.DisplaySize;
  widget->SetViewportSize(size.x * 0.5, size.y * 0.5);
  widget->SetSize(size.x * 0.5, size.y * 0.5);

  auto currentPos = ImGui::GetWindowPos();
  auto currentSize = ImGui::GetWindowSize();
  auto newPosition = ImVec2(currentPos.x + currentSize.x, currentPos.y);
  widget->SetPosition(newPosition.x, newPosition.y);
  widget->SetTitle(GetTitle() + " - Render");
  return widget;
}
