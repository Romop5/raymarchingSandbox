#include "RendererAttributesWidget.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

using namespace raymarcher;

RendererAttributesWidget::RendererAttributesWidget(std::shared_ptr<Raymarcher> rm) :
    raymarcher { rm }
{
    SetTitle("Settings");
}

auto RendererAttributesWidget::RenderContent() -> void
{
    if(ImGui::BeginCombo("Shading mode", "Shading mode"))
    {
        if(ImGui::Selectable("Phong"))
            raymarcher->SetShadingMode(ShadingMode::PHONG);
        if(ImGui::Selectable("Normal"))
            raymarcher->SetShadingMode(ShadingMode::NORMAL);
        if(ImGui::Selectable("Depth"))
            raymarcher->SetShadingMode(ShadingMode::DEPTH);
        if(ImGui::Selectable("Iterations"))
            raymarcher->SetShadingMode(ShadingMode::ITERATIONS);
        ImGui::EndCombo();
    }

    {
        auto val = static_cast<int>(raymarcher->GetMaximumIterations());
        if(ImGui::InputInt("Max. iterations", &val))
        {
           raymarcher->SetMaximumIterations(val);
        }
    }
    {
        auto val = raymarcher->GetFarPlaneDistance();
        if(ImGui::InputFloat("Far plane", &val, 1.0, 1000.0))
        {
           raymarcher->SetFarPlaneDistance(val);
        }
    }

    {
        auto val = raymarcher->GetEps();
        if(ImGui::InputFloat("Precision (eps)", &val))
        {
           raymarcher->SetEps(val);
        }
    }
    {
        auto val = raymarcher->GetAmbientCoef();
        if(ImGui::SliderFloat("Ambient coef.", &val, 0.0f, 1.0f))
        {
           raymarcher->SetAmbientCoef(val);
        }
    }
    {
        auto val = raymarcher->GetSpecularityCoef();
        if(ImGui::SliderFloat("Specularity", &val, 0.0f, 1.0f))
        {
           raymarcher->SetSpecularityCoef(val);
        }
    }
    {
        auto val = raymarcher->GetSunColour();
        if(ImGui::ColorPicker3("Sun color", glm::value_ptr(val)))
        {
           raymarcher->SetSunColour(val);
        }
    }

    {
        auto val = static_cast<bool>(raymarcher->IsFogRendered());
        if(ImGui::Checkbox("Render fog", &val))
        {
           raymarcher->SetRenderFog(val);
        }
    }

    {
        auto val = static_cast<bool>(raymarcher->IsShadowsRendered());
        if(ImGui::Checkbox("Render shadows", &val))
        {
           raymarcher->SetRenderShadows(val);
        }
    }

    ImGui::Text("User-defined uniforms");

    for(const auto& uniform: raymarcher->GetUserUniforms())
    {
        if(uniform.type == "float")
        {
            auto value = lastUniformValue[uniform.name];
            if(ImGui::SliderFloat(uniform.name.c_str(), &value, -1.0, 1.0))
            {
                raymarcher->SetUniform(uniform.name, value);
                lastUniformValue[uniform.name] = value;
            }
        }
    }

}
