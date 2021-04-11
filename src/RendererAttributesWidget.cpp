#include "RendererAttributesWidget.hpp"

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
        auto val = raymarcher->GetEps();
        if(ImGui::InputFloat("Precision (eps)", &val))
        {
           raymarcher->SetEps(val);
        }
    }
    {
        auto val = raymarcher->GetAmbientCoef();
        if(ImGui::InputFloat("Ambient coef.", &val))
        {
           raymarcher->SetAmbientCoef(val);
        }
    }
    {
        auto val = raymarcher->GetSpecularityCoef();
        if(ImGui::InputFloat("Specularity", &val))
        {
           raymarcher->SetSpecularityCoef(val);
        }
    }
}
