
#include <filesystem>
#include <imgui.h>
#include <sstream>
#include <string>
#include <vector>

#include "IconsFontAwesome5.h"
#include "helpers/FileHelper.hpp"
#include "widgets/BVHCalculatorWidget.hpp"
#include "widgets/EditWidget.hpp"
#include "widgets/WelcomeWidget.hpp"

#include "widgets/MenuWidget.hpp"

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

MenuWidget::MenuWidget(IApplication& app, WidgetManager& manager)
  : application{ app }
  , windowManager{ manager }
  , showFPS{ false }
{
}

auto
MenuWidget::RenderContent() -> void
{
  if (showFPS) {
    fpsMeter.Measure();
    fpsMeter.RenderOverlay();
  }

  if (ImGui::BeginMainMenuBar()) {
    // RenderContentMenu();
    RenderContentWidget(true);
    ImGui::EndMainMenuBar();
  }
}

auto
MenuWidget::RenderContentWidget(bool is_menu) -> void
{
  if (ImGui::BeginMenu("File")) {
    if (ImGui::MenuItem("New")) {
      auto widget = std::make_shared<raymarcher::EditWidget>("New SDF function",
                                                             SimpleSDFCode());
      windowManager.AddWidget(widget);
    }

    LoadSDFWidget();

    if (ImGui::MenuItem("Exit (no save)")) {
      application.RequestExit();
    }
    ImGui::EndMenu();
  }

  if (ImGui::BeginMenu("Examples")) {
    if (ImGui::BeginMenu("Primitives")) {
      static std::vector<std::pair<std::string, std::string>> primitives = {
        { "Sphere", "sphere.sdf" },
        { "Cube", "cube.sdf" },
        { "Plane", "plane.sdf" },
        { "Oriented plane", "orientedPlane.sdf" },
      };

      for (auto& [name, path] : primitives) {
        if (ImGui::MenuItem(name.c_str())) {
          Load(path);
        }
      }
      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Operations")) {
      static std::vector<std::pair<std::string, std::string>> operations = {
        { "Union", "union.sdf" },
        { "Intersection", "intersection.sdf" },
        { "Animated waves", "wave.sdf" },
        { "Repeating", "repeat.sdf" },
        { "Smooth union", "metaballs.sdf" },
      };

      for (auto& [name, path] : operations) {
        if (ImGui::MenuItem(name.c_str())) {
          Load(path);
        }
      }
      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Interesting scenes")) {
      if (ImGui::MenuItem("Test app")) {
        auto widget = std::make_shared<raymarcher::EditWidget>(
          "New SDF function", TestApplication());
        widget->Recompile();
        windowManager.AddWidget(widget);
      }

      static std::vector<std::pair<std::string, std::string>> operations = {
        { "Contractions", "contractions.sdf" },
        { "Contractions Animated", "contractionsAnimated2.sdf" },
        { "Dungeon", "dungeon.sdf" },
        { "Vanishing spheres", "periodicCubes.sdf" },
        { "Bending tube/cone", "cone.sdf" },
      };

      for (auto& [name, path] : operations) {
        if (ImGui::MenuItem(name.c_str())) {
          Load(path);
        }
      }
      ImGui::EndMenu();
    }

    ImGui::EndMenu();
  }
  if (ImGui::BeginMenu("Extras")) {
    if (ImGui::MenuItem("BHV Optimizator")) {
      auto widget = std::make_shared<raymarcher::BVHCalculatorWidget>();
      windowManager.AddWidget(widget);
    }
    ImGui::EndMenu();
  }

  if (ImGui::BeginMenu("Windows")) {
    if (ImGui::MenuItem("Close all windows")) {
      windowManager.RemoveAllWidgets();
    }
    if (ImGui::MenuItem("Show FPS")) {
      showFPS = !showFPS;
    }
    ImGui::EndMenu();
  }

  if (ImGui::MenuItem("Welcome")) {
    auto welcome = std::make_shared<WelcomeWidget>(windowManager);
    welcome->SetSize(500, 0);
    windowManager.AddWidget(welcome);
  }
}

auto
MenuWidget::LoadSDFWidget() -> void
{
  if (ImGui::MenuItem("Load file")) {
    auto widget = std::make_shared<raymarcher::LoadFileWidget>(windowManager);
    windowManager.AddWidget(widget);
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
