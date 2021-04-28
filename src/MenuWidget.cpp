#include "MenuWidget.hpp"

#include <string>
#include <vector>
#include <imgui.h>
#include <filesystem>
#include <fstream>
#include <sstream>

#include "EditWidget.hpp"
#include "BVHCalculator.hpp"

using namespace raymarcher;

namespace
{
    auto SimpleSDFCode() -> std::string
    {
        auto sdfLiteral = R"(
vec4 df(vec3 pos)
{
    vec4 gd = ground(pos, floorElevation);

    //return unite(gd,object(pos));
    return unite(gd,vec4(sphere(pos, 1.0), vec3(1.0,0.0,0.0)));
}
        )";
        return sdfLiteral;
    }

    auto TestApplication() -> std::string
    {
        auto sdfLiteral = R"(
vec4 df(vec3 pos)
{
    vec4 gd = ground(pos, floorElevation);

    vec4 s1 = vec4(sphere(pos+vec3(0.5,0.1,0.0), 1.0), vec3(1.0));
    vec4 s2 = vec4(sphere(pos+vec3(-0.5,0.5,0.0), 1.0), vec3(1.0));
    vec4 s3 = vec4(sphere(pos+vec3(0.5,0.9,0.0), 1.0), vec3(1.0));
    return unite(gd, unite(s1, unite(s2, s3)));
    //return unite(gd,object(pos));
    //return unite(gd,vec4(sphere(pos, 1.0), vec3(1.0,0.0,0.0)));
}
        )";
        return sdfLiteral;
    }
}

MenuWidget::MenuWidget(WidgetManager& manager) :
    windowManager { manager },
    showFPS { false }
{
}

auto MenuWidget::RenderContent() -> void
{
    if (showFPS)
    {
        fpsMeter.Measure();
        fpsMeter.RenderOverlay();
    }

    LoadSDFWidget();
    ImGui::SameLine();
    if(ImGui::Button("New"))
    {
        auto widget = std::make_shared<raymarcher::EditWidget>("New SDF function", SimpleSDFCode());
        windowManager.AddWidget(widget);
    }

    if(ImGui::Button("BHV Optimizator"))
    {
        auto widget = std::make_shared<raymarcher::BVHCalculator>();
        windowManager.AddWidget(widget);
    }

    if(ImGui::Button("Test app"))
    {
        auto widget = std::make_shared<raymarcher::EditWidget>("New SDF function", TestApplication());
        widget->Recompile();
        windowManager.AddWidget(widget);
    }

    ImGui::Text("Primitives");

    static std::vector<std::pair<std::string, std::string>> primitives =
    {
        { "Sphere", "vec4 df(vec3 pos) {\n    vec3 center = vec3(0.0);\n    float radius = 1.0;\n    float d = length(pos-center) - radius;\n    return vec4(d, vec3(1.0,0.0,0.0));\n}" },
        { "Cube", "vec4 df(vec3 pos) {\n    vec3 center = vec3(0.0);\n    float radius = 1.0;\n    vec3 np = abs(pos);\n    float d = max(np.x, max(np.y,np.z)) - radius;\n    return vec4(d, vec3(1.0,0.0,0.0));\n}" },
        { "Plane", "vec4 df(vec3 pos) {\n   vec3 color = vec3(sin(10.0*pos.x), 0.0, 0.0);\n   float offset = -3.0;\n   return vec4(pos.y - offset, color);\n}"},
        { "Oriented Plane", "vec4 df(vec3 pos) {\n    vec3 normal = normalize(vec3(1.0,1.0,1.0));\n   vec3 color = vec3(sin(10.0*pos.x), 0.0, 0.0);\n   float offset = -3.0;\n   return vec4(dot(normal, pos) - offset, color);\n}"},
    };

    for(auto& [name, code]: primitives)
    {
        if(ImGui::Button(name.c_str()))
        {
            auto widget = std::make_shared<raymarcher::EditWidget>(name, code);
            widget->Recompile();
            windowManager.AddWidget(widget);
        }
    }

    ImGui::Text("Operations");

    static std::vector<std::pair<std::string, std::string>> operations =
    {
        { "Union", "vec4 df(vec3 pos) {\n    float d = min(sphere(pos, 1.0), sphere(pos+vec3(0.5), 1.0));\n    return vec4(d, vec3(1.0));\n}" },
        { "Intersection", "vec4 df(vec3 pos) {\n    float d = max(sphere(pos, 1.0), sphere(pos+vec3(0.5), 1.0));\n    return vec4(d, vec3(1.0));\n}" },
    };

    for(auto& [name, code]: operations)
    {
        if(ImGui::Button(name.c_str()))
        {
            auto widget = std::make_shared<raymarcher::EditWidget>(name, code);
            widget->Recompile();
            windowManager.AddWidget(widget);
        }
    }
    if(ImGui::Button("Show FPS"))
    {
        showFPS = !showFPS;
    }
}

auto MenuWidget::LoadSDFWidget() -> void
{
    static std::vector<std::string> fileNames;
    static std::filesystem::path selectedFile;

    if(ImGui::Button("Load file"))
    {
        //auto widget = std::make_shared<raymarcher::EditWidget>("New SDF function", SimpleSDFCode());
        //windowManager.AddWidget(widget);
        ImGui::OpenPopup("LoadPopup");

    }

    if(ImGui::BeginPopup("LoadPopup"))
    {
        ImGui::Text("Load SDF function from disk");
        ImGui::Separator();

        if(ImGui::BeginListBox(""))
        {
        
            for (const auto & entry : std::filesystem::directory_iterator(std::filesystem::current_path()))
            {
                const auto path = entry.path();
                const auto filename = path.filename();
                if(filename.extension() != ".sdf")
                {
                    continue;
                }
                if(ImGui::Selectable(filename.c_str()))
                {
                    selectedFile = path;
                }
            }
            ImGui::EndListBox();
        }

        ImGui::Text(selectedFile.filename().c_str());
        ImGui::SameLine();
        if(ImGui::Button("Load"))
        {
            Load(selectedFile);
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

auto MenuWidget::Load(std::string path) -> void
{
    std::filesystem::path filePath = path;
    std::ifstream file;
    file.open(path, std::ifstream::in);
    if(!file.is_open())
    {
        return;
    }

    std::stringstream content;
    std::string line;
    while(file)
    {
        std::getline(file, line);
        if(!file)
            break;
        content << line << std::endl;
    }

    auto widget = std::make_shared<raymarcher::EditWidget>(filePath.filename(), content.str(), filePath.filename());
    windowManager.AddWidget(widget);
}

