#include "widgets/MenuWidget.hpp"

#include <imgui.h>
#include <sstream>
#include <string>
#include <vector>

#include "helpers/FileHelper.hpp"
#include "widgets/BVHCalculatorWidget.hpp"
#include "widgets/EditWidget.hpp"
#include <filesystem>

using namespace raymarcher;

namespace {
auto
SimpleSDFCode() -> std::string
{
  auto sdfLiteral = R"(
uniform float elevation = -0.1;
uniform float sphereRadius = 1.0;
uniform float colorX = 1.0;
uniform float colorY = 1.0;
uniform float colorZ = 1.0;
vec4 df(vec3 pos)
{
    vec4 gd = ground(pos, elevation*10.0);

    // Map <-1, 1> to <0,1> for each channel
    vec3 color = 0.5*(vec3(colorX, colorY, colorZ)+vec3(1.0));
    return unite(gd,vec4(sphere(pos, sphereRadius), color));
}
        )";
  return sdfLiteral;
}

auto
TestApplication() -> std::string
{
  auto sdfLiteral = R"(
uniform float elevation = -0.1;
uniform float radius = 1.0;
vec4 df(vec3 pos)
{
    vec4 gd = ground(pos, elevation*10.0);

    vec4 s1 = vec4(sphere(pos+vec3(0.5,0.1,0.0), radius), vec3(1.0));
    vec4 s2 = vec4(sphere(pos+vec3(-0.5,0.5,0.0), radius), vec3(1.0));
    vec4 s3 = vec4(sphere(pos+vec3(0.5,0.9,0.0), radius), vec3(1.0));
    return unite(gd, unite(s1, unite(s2, s3)));
}
        )";
  return sdfLiteral;
}
}

MenuWidget::MenuWidget(WidgetManager& manager)
  : windowManager{ manager }
  , showFPS{ false }
{}

auto
MenuWidget::RenderContent() -> void
{
  if (showFPS) {
    fpsMeter.Measure();
    fpsMeter.RenderOverlay();
  }

  if (ImGui::Button("New")) {
    auto widget = std::make_shared<raymarcher::EditWidget>("New SDF function",
                                                           SimpleSDFCode());
    windowManager.AddWidget(widget);
  }
  ImGui::SameLine();
  LoadSDFWidget();

  ImGui::Text("Primitives");
  static std::vector<std::pair<std::string, std::string>> primitives = {
    { "Sphere", "sphere.sdf" },
    { "Cube", "cube.sdf" },
    { "Plane", "plane.sdf" },
    { "Oriented plane", "orientedPlane.sdf" },
  };

  for (auto& [name, path] : primitives) {
    if (ImGui::Button(name.c_str())) {
      Load(path);
    }
  }

  ImGui::Text("Operations");

  static std::vector<std::pair<std::string, std::string>> operations = {
    { "Union", "union.sdf" },
    { "Intersection", "intersection.sdf" },
    { "Animated waves", "wave.sdf" },
    { "Repeating", "repeat.sdf" },
    { "Smooth union", "metaballs.sdf" },
  };

  for (auto& [name, path] : operations) {
    if (ImGui::Button(name.c_str())) {
      Load(path);
    }
  }
  ImGui::Separator();

  ImGui::Text("Interesting scenes");
  if (ImGui::Button("Test app")) {
    auto widget = std::make_shared<raymarcher::EditWidget>("New SDF function",
                                                           TestApplication());
    widget->Recompile();
    windowManager.AddWidget(widget);
  }

  if (ImGui::Button("Contractions")) {
    Load("contractions.sdf");
  }
  if (ImGui::Button("Contractions Animated")) {
    Load("contractionsAnimated2.sdf");
  }

  if (ImGui::Button("Dungeon")) {
    Load("dungeon.sdf");
  }

  if (ImGui::Button("Vanishing spheres")) {
    Load("periodicCubes.sdf");
  }

  if (ImGui::Button("Bending tube/cone")) {
    Load("cone.sdf");
  }

  ImGui::Separator();

  ImGui::Text("Extras");
  if (ImGui::Button("BHV Optimizator")) {
    auto widget = std::make_shared<raymarcher::BVHCalculatorWidget>();
    windowManager.AddWidget(widget);
  }
  if (ImGui::Button("Show FPS")) {
    showFPS = !showFPS;
  }
}

auto
MenuWidget::LoadSDFWidget() -> void
{
  static std::vector<std::string> fileNames;
  static std::filesystem::path selectedFile;

  if (ImGui::Button("Load file")) {
    ImGui::OpenPopup("LoadPopup");
  }

  if (ImGui::BeginPopup("LoadPopup")) {
    ImGui::Text("Load SDF function from disk");
    ImGui::Separator();

    if (ImGui::BeginListBox("")) {

      for (const auto& entry : std::filesystem::directory_iterator(
             std::filesystem::current_path())) {
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
      Load(selectedFile);
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
MenuWidget::Load(std::string path) -> void
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
  windowManager.AddWidget(widget);
}
