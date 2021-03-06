#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

#include "widgets/RendererAttributesWidget.hpp"

using namespace raymarcher;

RendererAttributesWidget::RendererAttributesWidget(
  std::shared_ptr<Raymarcher> rm)
  : raymarcher{ rm }
{
  SetTitle("Settings");
}

auto
RendererAttributesWidget::RenderContent() -> void
{
  if (ImGui::CollapsingHeader("General")) {
    if (ImGui::BeginCombo("Shading mode", "Shading mode")) {
      if (ImGui::Selectable("Phong"))
        raymarcher->SetShadingMode(ShadingMode::PHONG);
      if (ImGui::Selectable("Normal"))
        raymarcher->SetShadingMode(ShadingMode::NORMAL);
      if (ImGui::Selectable("Depth"))
        raymarcher->SetShadingMode(ShadingMode::DEPTH);
      if (ImGui::Selectable("Iterations"))
        raymarcher->SetShadingMode(ShadingMode::ITERATIONS);
      if (ImGui::Selectable("Bitmap"))
        raymarcher->SetShadingMode(ShadingMode::BITMAP);
      ImGui::EndCombo();
    }

    {
      auto val = static_cast<int>(raymarcher->GetMaximumIterations());
      if (ImGui::InputInt("Max. iterations", &val)) {
        raymarcher->SetMaximumIterations(val);
      }
    }
    {
      auto val = raymarcher->GetEps();
      if (ImGui::InputFloat("Precision (eps)", &val)) {
        raymarcher->SetEps(val);
      }
    }
    {
      auto val = static_cast<float>(raymarcher->GetStepRatio());
      if (ImGui::InputFloat("Step ratio", &val)) {
        raymarcher->SetStepRatio(val);
      }
    }
    {
      auto val = raymarcher->GetFarPlaneDistance();
      if (ImGui::InputFloat("Far plane", &val, 1.0, 1000.0)) {
        raymarcher->SetFarPlaneDistance(val);
      }
    }
    {
      auto val = raymarcher->GetFx();
      if (ImGui::InputFloat("Fx", &val, 0.1, 3.0)) {
        raymarcher->SetFx(val);
      }
    }
    {
      auto val = raymarcher->GetAspect();
      if (ImGui::InputFloat("Aspect ratio", &val, 0.1, 2.0)) {
        raymarcher->SetAspect(val);
      }
    }
  }

  if (ImGui::CollapsingHeader("Phong-related")) {
    {
      auto val = raymarcher->GetAmbientCoef();
      if (ImGui::SliderFloat("Ambient coef.", &val, 0.0f, 1.0f)) {
        raymarcher->SetAmbientCoef(val);
      }
    }
    {
      auto val = raymarcher->GetSpecularityCoef();
      if (ImGui::SliderFloat("Specularity", &val, 0.0f, 1.0f)) {
        raymarcher->SetSpecularityCoef(val);
      }
    }

    {
      auto val = static_cast<bool>(raymarcher->IsSunDirectional());
      if (ImGui::Checkbox("Is sun directional", &val)) {
        raymarcher->SetSunDirectional(val);
      }
    }

    {
      auto val = raymarcher->GetSun();
      if (ImGui::InputFloat3("Sun position", glm::value_ptr(val))) {
        raymarcher->SetSun(val);
      }
    }

    {
      auto val = raymarcher->GetSunIntensity();
      if (ImGui::InputFloat("Sun intensity", &val)) {
        raymarcher->SetSunIntensity(val);
      }
    }

    {
      auto val = raymarcher->GetSunColour();
      if (ImGui::ColorPicker3("Sun color", glm::value_ptr(val))) {
        raymarcher->SetSunColour(val);
      }
    }
    {
      auto val = raymarcher->GetFogColour();
      if (ImGui::ColorPicker3("Fog color", glm::value_ptr(val))) {
        raymarcher->SetFogColour(val);
      }
    }

    {
      auto val = static_cast<bool>(raymarcher->IsFogRendered());
      if (ImGui::Checkbox("Render fog", &val)) {
        raymarcher->SetRenderFog(val);
      }
    }

    {
      auto val = static_cast<bool>(raymarcher->IsShadowsRendered());
      if (ImGui::Checkbox("Render shadows", &val)) {
        raymarcher->SetRenderShadows(val);
      }
    }
  }

  auto& uniforms = raymarcher->GetUserUniforms();
  if (!uniforms.empty() && ImGui::CollapsingHeader("User-defined")) {
    for (const auto& uniform : uniforms) {
      if (uniform.type == "float") {
        auto value = lastUniformValue[uniform.name];
        if (ImGui::SliderFloat(uniform.name.c_str(), &value, -1.0, 1.0)) {
          raymarcher->SetUniform(uniform.name, value);
          lastUniformValue[uniform.name] = value;
        }
      }
    }
  }
}
