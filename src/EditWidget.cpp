#include "EditWidget.hpp"

#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

#include "RendererWidget.hpp"
#include "Raymarcher.hpp"
#include "OrbitCamera.hpp"
#include "GLFWCamera.hpp"
#include "SDF.hpp"

using namespace raymarcher;

namespace
{
    auto CreateRayMarchingWidget(std::string code) -> std::shared_ptr<WidgetBase>
    {
        auto rm = std::make_shared<raymarcher::Raymarcher>();
        auto orbiter = std::make_shared<raymarcher::OrbitCamera>();
        orbiter->SetCenter(glm::vec3(0.0, 3.0,0.0));
        orbiter->SetDistance(5.0);
        //auto focusedCamera = std::make_shared<raymarcher::GLFWCamera>(orbiter, raymarcher::GLFWCamera::CameraType::ORBITER_CAMERA);
        //rm->SetCamera(focusedCamera);
        rm->SetCamera(orbiter);
        auto sdf = std::make_shared<raymarcher::SDF>(code);
        rm->SetSDF(sdf);

        auto widget = std::make_shared<raymarcher::RendererWidget>(rm);
        widget->SetViewportSize(300,300);
        return widget;
    }
}

EditWidget::EditWidget(std::string name, std::string startingCode) :
    code { startingCode }
{
    SetTitle(name);
}

auto EditWidget::Render() -> void
{
    ImGui::SetNextWindowSize(ImVec2(400,300), ImGuiCond_Once);
    WindowWidget::Render();
}

auto EditWidget::RenderContent() -> void
{
    if(ImGui::Button("Compile"))
    {
        AddWidget(CreateRayMarchingWidget(code));
    }
    ImGui::Text("Code:");
    ImGui::InputTextMultiline("", &code, ImVec2(-1,-1));

    WindowWidget::RenderContent();
}
