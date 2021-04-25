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
    auto CreateAndSetSDF(std::string code, std::shared_ptr<raymarcher::Raymarcher> rm) -> void
    {
        auto sdf = std::make_shared<raymarcher::SDF>(code);
        rm->SetSDF(sdf);
    }

    auto CreateWidget(std::shared_ptr<raymarcher::Raymarcher> rm) -> std::shared_ptr<WidgetBase>
    {
        auto orbiter = std::make_shared<raymarcher::OrbitCamera>();
        orbiter->SetCenter(glm::vec3(0.0, 3.0,0.0));
        orbiter->SetDistance(5.0);
        auto focusedCamera = std::make_shared<raymarcher::GLFWCamera>(orbiter, raymarcher::GLFWCamera::CameraType::ORBITER_CAMERA);
        rm->SetCamera(focusedCamera);
        
        auto widget = std::make_shared<raymarcher::RendererWidget>(rm, focusedCamera);
        widget->SetViewportSize(400,400);
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
        Recompile();
    }
    if(!lastError.empty())
    {
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(1.0, 0.0,0.0,1.0),"Compilation Error");
    }
    ImGui::Text("Code:");
    if(ImGui::InputTextMultiline("", &code, ImVec2(-1,-1)))
    {
        Recompile();
    }

    if(!lastError.empty())
    {
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar;
        ImGui::BeginChild("ErrorChild", ImVec2(0, 60), false, window_flags);
        ImGui::TextWrapped("Error: %s", lastError.c_str());
        ImGui::EndChild();
    }


    WindowWidget::RenderContent();
}

auto EditWidget::Recompile() -> void
{
    //RemoveAllWidgets();
    lastError = "";
    try {
        if(!rm)
        {
            rm = std::make_shared<raymarcher::Raymarcher>();
        }
        CreateAndSetSDF(code, rm);
        if(!HasAnyWidget())
        {
            AddWidget(CreateWidget(rm));
        }
    } catch (std::exception& error)
    {
        lastError = error.what();
    }
}
