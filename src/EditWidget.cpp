#include "EditWidget.hpp"

#include <fstream>
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
}

EditWidget::EditWidget(std::string name, std::string startingCode, std::string file) :
    code { startingCode },
    isStatusError { false },
    filename { file }
{
    SetTitle(name);
    SetSize(400, 400);
}

auto EditWidget::Render() -> void
{
    WindowWidget::Render();
}

auto EditWidget::RenderContent() -> void
{
    SaveAsSubwidget();
    ImGui::SameLine();
    if(ImGui::Button("Compile"))
    {
        Recompile();
    }

    ImGui::SameLine();
    ImGui::Checkbox("Auto-Compile",&isAutocompileEnabled);

    if(!lastStatus.empty())
    {
        ImGui::SameLine();
        std::string msg = lastStatus.substr(0, 30);
        if(isStatusError)
        {
            msg = "Error: " + msg;
            ImGui::TextColored(ImVec4(1.0, 0.0,0.0,1.0),msg.c_str());
        } else {
            msg = "Status: " + msg;
            ImGui::Text(msg.c_str());
        }
        if(ImGui::IsItemHovered())
        {
            ImGui::BeginTooltip();
            ImGui::Text(lastStatus.c_str());
            ImGui::EndTooltip();
        }
    }

    ImGui::Text("Code:");
    if(ImGui::InputTextMultiline("", &code, ImVec2(-1,-1)) && isAutocompileEnabled)
    {
        Recompile();
    }

    /* if(!lastStatus.empty()) */
    /* { */
    /*     ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar; */
    /*     ImGui::BeginChild("ErrorChild", ImVec2(0, 60), false, window_flags); */
        
    /*     ImGui::TextWrapped("Error: %s", lastStatus.c_str()); */
    /*     ImGui::EndChild(); */
    /* } */


    WindowWidget::RenderContent();
}

auto EditWidget::Recompile() -> void
{
    //RemoveAllWidgets();
    SetLastStatus("");
    try {
        if(!rm)
        {
            rm = std::make_shared<raymarcher::Raymarcher>();
        }
        CreateAndSetSDF(code, rm);
        if(!HasAnyWidget())
        {
            AddWidget(CreateRenderWidget(rm));
        }
    } catch (std::exception& error)
    {
        SetLastError(error.what());
    }
}

auto EditWidget::SaveAs(std::string filename) -> void
{
    if(filename.empty())
    {
        SetLastError("No file entered for this SDF");
        return;
    }
    std::fstream outputFile;
    outputFile.open(filename, std::fstream::out | std::fstream::trunc);
    if(!outputFile.is_open())
    {
        SetLastError("Failed to open file " + filename);
        return;
    }
    outputFile << code;
    if(!outputFile.good())
    {
        SetLastError("Failed to save to " + filename);
        return;
    }
    SetLastStatus("Saved");
}

auto EditWidget::SaveAsSubwidget() -> void
{
    if(ImGui::Button("Save"))
    {
        SaveAs(filename);
    }
    ImGui::SameLine();

    if(ImGui::Button("Save as"))
    {
        if(filename.empty())
        {
            filename = "new.sdf";
        }
        ImGui::OpenPopup("SVPopup");
    }

    if(ImGui::BeginPopup("SVPopup"))
    {
        ImGui::Text("Save SDF function to disk");
        ImGui::Separator();
        ImGui::InputText("", &filename);
        if(ImGui::Button("Save"))
        {
            SaveAs(filename);
            ImGui::CloseCurrentPopup();
        }

        ImGui::SameLine();
        if(ImGui::Button("Exit"))
        {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
}

auto EditWidget::SetLastError(std::string error) -> void
{
    lastStatus = error;
    isStatusError = true;
}
auto EditWidget::SetLastStatus(std::string msg) -> void
{
    lastStatus = msg;
    isStatusError = false;
}

auto EditWidget::CreateRenderWidget(std::shared_ptr<raymarcher::Raymarcher> rm) -> std::shared_ptr<WidgetBase>
{
    auto orbiter = std::make_shared<raymarcher::OrbitCamera>();
    orbiter->SetCenter(glm::vec3(0.0, 3.0,0.0));
    orbiter->SetDistance(5.0);
    auto focusedCamera = std::make_shared<raymarcher::GLFWCamera>(orbiter, raymarcher::GLFWCamera::CameraType::ORBITER_CAMERA);
    rm->SetCamera(focusedCamera);
    
    auto widget = std::make_shared<raymarcher::RendererWidget>(rm, focusedCamera);

    ImGuiIO& io = ImGui::GetIO();
    auto size = io.DisplaySize;
    widget->SetViewportSize(size.x*0.5, size.y*0.5);
    widget->SetSize(size.x*0.5, size.y*0.5);
    widget->SetTitle(GetTitle() + " - Render");
    return widget;
}
