#include <algorithm>
#include <queue>
#include <random>
#include <sstream>
#include <vector>

#include "IconsFontAwesome5.h"
#include <glm/glm.hpp>
#include <imgui.h>

#include "bvh/BVHGenerator.hpp"
#include "bvh/BVHLibrary.hpp"
#include "helpers/FileHelper.hpp"
#include "widgets/EditWidget.hpp"

#include "widgets/BVHCalculatorWidget.hpp"

using namespace raymarcher;

BVHCalculatorWidget::BVHCalculatorWidget()
  : pimpl{ std::make_unique<BVHLibrary>() }
{
  SetSize(400, 0);
  // Small (close) scene
  auto& scene = pimpl->GetScene();
}

BVHCalculatorWidget::~BVHCalculatorWidget() = default;

auto
BVHCalculatorWidget::RenderContent() -> void
{
  //--------------------------------------------------------------------------------------
  // Button controls
  //--------------------------------------------------------------------------------------
  if (ImGui::Button(ICON_FA_PLUS "Generate scene")) {
    GenerateGeometry();
  }
  ImGui::SameLine();
  if (ImGui::Button(ICON_FA_FILE_UPLOAD "Generate & save")) {
    Optimize();

    auto hasPassed = true;
    hasPassed &=
      FileHelper::SaveFile("optimized.sdf", pimpl->GenerateCode(true));
    hasPassed &=
      FileHelper::SaveFile("nonoptimized.sdf", pimpl->GenerateCode(false));
  }

  ImGui::SameLine();

  if (ImGui::Button(ICON_FA_PLUS "Generate optimized code")) {
    Optimize();

    auto widget = std::make_shared<raymarcher::EditWidget>(
      "Generated: optimized", pimpl->GenerateCode(true));
    AddWidget(widget);

    auto widget2 = std::make_shared<raymarcher::EditWidget>(
      "Generated: non-optimized", pimpl->GenerateCode(false));
    AddWidget(widget2);
  }

  ImGui::SameLine();
  if (ImGui::Button(ICON_FA_TV "Preview current scene")) {
    auto widget2 = std::make_shared<raymarcher::EditWidget>(
      "Scene Preview: non-optimized", pimpl->GenerateCode(false));
    widget2->Recompile();
    AddWidget(widget2);
  }

  ImGui::SameLine();
  if (ImGui::Button(ICON_FA_TIMES "Close subwindows")) {
    RemoveAllWidgets();
  }

  //--------------------------------------------------------------------------------------
  // Parameters
  //--------------------------------------------------------------------------------------
  auto params = pimpl->GetParams();
  int maxLevel = params.maxLevel;
  ImGui::PushItemWidth(30);
  ImGui::SliderInt("Maximum level of BVH", &maxLevel, 0, 16);
  params.maxLevel = maxLevel;
  pimpl->SetParams(params);

  //--------------------------------------------------------------------------------------
  // Scene
  //--------------------------------------------------------------------------------------

  ImGui::Text("Elements");
  for (auto& element : pimpl->GetScene()) {
    DisplayElement(*element, 0);
  }

  WindowWidget::RenderContent();
}

auto
BVHCalculatorWidget::DisplayElement(SpherePrimitive& element, size_t level)
  -> void
{
  ImGui::PushID(&element);
  auto pos = element.GetCenter();
  auto size = element.GetSize();
  std::string padding(level, ' ');
  ImGui::Text("%sSphere: (%f, %f, %f) Size: %f",
              padding.c_str(),
              pos.x,
              pos.y,
              pos.z,
              size);

  if (element.GetChildren().size() > 0) {
    ImGui::Separator();
    for (auto& child : element.GetChildren()) {
      DisplayElement(*child, level + 1);
    }
    ImGui::Separator();
  }
  ImGui::PopID();
}

auto
BVHCalculatorWidget::Optimize() -> void
{
  pimpl->Optimize();
}

auto
BVHCalculatorWidget::GenerateGeometry() -> void
{
  std::random_device seed;
  BVHGenerator generator(seed());
  auto scene =
    generator.GenerateClusteredScene(1.0, 0.5, { glm::vec3(0), glm::vec3(3) });
  for (auto& child : scene) {
    pimpl->GetScene().push_back(child);
  }
}
