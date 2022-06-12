#include "widgets/RendererWidget.hpp"
#include "application/IApplication.hpp"
#include "rendering/FlyingCamera.hpp"
#include "rendering/OrbitCamera.hpp"
#include "widgets/RendererAttributesWidget.hpp"
#include <GL/gl.h>
#include <imgui.h>
#include <iostream>

using namespace raymarcher;

RendererWidget::RendererWidget(std::shared_ptr<Raymarcher> rm,
                               std::shared_ptr<GLFWCamera> cam)
  : raymarcher{ std::move(rm) }
  , camera{ std::move(cam) }
  , viewportWidth{ 100 }
  , viewportHeight{ 100 }
  , isPaused{ false }
{
  raymarcher->SetCamera(camera);
  SetTitle("Preview");
  Reinitialize();
}

auto
RendererWidget::RenderContent() -> void
{
  if (!targetFBO || !colorImage) {
    Reinitialize();
    return;
  }

  if (!isPaused) {
    targetFBO->bind();
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glClear(GL_COLOR_BUFFER_BIT);
    glViewport(0, 0, viewportWidth, viewportHeight);
    raymarcher->Render();
    targetFBO->unbind();
    glPopAttrib();
  }

  std::string pauseButtonString = (isPaused ? "Play" : "Pause");
  if (ImGui::Button(pauseButtonString.c_str())) {
    isPaused = !isPaused;
  }
  ImGui::SameLine();

  if (ImGui::Button("Settings")) {
    RemoveAllWidgets();
    auto widget = std::make_shared<RendererAttributesWidget>(raymarcher);
    widget->SetTitle(GetTitle() + "  - Settings");
    widget->SetSize(200, 0);
    AddWidget(widget);
  }
  ImGui::SameLine();
  if (ImGui::Button("Fit render plane to window")) {
    auto size = ImGui::GetWindowSize();
    SetViewportSize(size.x, size.y);
  }
  ImGui::SameLine();
  if (ImGui::Button("Toggle orbiter/fly")) {
    using CameraType = GLFWCamera::CameraType;
    if (camera) {
      if (camera->GetType() == CameraType::ORBITER_CAMERA) {
        camera = std::make_shared<GLFWCamera>(std::make_shared<FlyingCamera>(),
                                              CameraType::FLYING_CAMERA);
      } else {
        camera = std::make_shared<GLFWCamera>(std::make_shared<OrbitCamera>(),
                                              CameraType::ORBITER_CAMERA);
      }
      raymarcher->SetCamera(camera);
    }
    auto size = ImGui::GetWindowSize();
    SetViewportSize(size.x, size.y);
  }

  ImGui::SameLine();
  if (ImGui::Button("Help")) {
    ImGui::OpenPopup("HelpPopup");
  }

  if (ImGui::BeginPopup("HelpPopup")) {
    ImGui::Text("Help");
    ImGui::Separator();
    ImGui::Text("Fly camera:");
    ImGui::Text("A S W D - movement (left, back, front, right)");
    ImGui::Text("mouse - rotation");
    ImGui::Text("Arrow Up/Down - rotation (vertical)");
    ImGui::Text("Arrow Left/Right - rotation (horizontal)");
    ImGui::Text("Page Up/Down - movement up/down");

    ImGui::Separator();
    ImGui::Text("Orbit camera:");
    ImGui::Text("S W  - move close/further");
    ImGui::Text("A D  - rotate left/right");
    ImGui::EndPopup();
  }

  if (ImGui::BeginChild("Render")) {
    auto& app = IApplication::GetApplication();
    ImGui::Image(reinterpret_cast<void*>(colorImage->getId()),
                 ImGui::GetWindowSize(),
                 ImVec2(0, 1),
                 ImVec2(1, 0));
    if (app.GetFocus() == camera) {
      auto drawList = ImGui::GetWindowDrawList();
      auto pos = ImGui::GetWindowPos();
      auto size = ImGui::GetWindowSize();
      pos.x += 20;
      pos.y += 20;
      drawList->AddText(pos, -1u, "Press ESC to exit");
      // auto io = ImGui::GetIO();
      // drawList->AddText(io.FontDefault, 10.0f, pos, -1u, "Press ESC to
      // exit");
    }

    if (ImGui::IsItemClicked() && !isPaused) {
      app.SetFocus(camera);
    }
  }
  ImGui::EndChild();

  WindowWidget::RenderContent();
}

auto
RendererWidget::Render() -> void
{
  ImGui::SetNextWindowSize(ImVec2(200, 200), ImGuiCond_Once);
  WindowWidget::Render();
}

auto
RendererWidget::SetViewportSize(size_t width, size_t height) -> void
{
  viewportWidth = width;
  viewportHeight = height;
  Reinitialize();
}

auto
RendererWidget::Reinitialize() -> void
{
  colorImage = std::make_shared<ge::gl::Texture>(
    GL_TEXTURE_2D, GL_RGB16, 1, viewportWidth, viewportHeight);
  targetFBO = std::make_shared<ge::gl::Framebuffer>();
  targetFBO->attachTexture(GL_COLOR_ATTACHMENT0, colorImage.get());
  auto status = targetFBO->check();
  if (!status) {
    std::cout << "Texture: " << colorImage->getInfo() << std::endl;
    std::cout << "Info: " << targetFBO->getInfo() << std::endl;
    assert(false);
  }
  std::cout << "Reinitialized " << std::endl;
}
