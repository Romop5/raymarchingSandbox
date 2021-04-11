#include "MenuWidget.hpp"

#include <string>
#include <vector>
#include <imgui.h>

#include "EditWidget.hpp"

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
    windowManager { manager }
{
}

auto MenuWidget::RenderContent() -> void
{
    if(ImGui::Button("Add new"))
    {
        auto widget = std::make_shared<raymarcher::EditWidget>("New SDF function", SimpleSDFCode());
        windowManager.AddWidget(widget);
    }

    if(ImGui::Button("Test app"))
    {
        auto widget = std::make_shared<raymarcher::EditWidget>("New SDF function", TestApplication());
        windowManager.AddWidget(widget);
    }

    static std::vector<std::pair<std::string, std::string>> primitives =
    {
        { "Sphere", "vec4 df(vec3 pos) {\n    vec3 center = vec3(0.0);\n    float radius = 1.0;\n    float d = length(pos-center) - radius;\n    return vec4(d, vec3(1.0,0.0,0.0));\n}" }
    };

    for(auto& [name, code]: primitives)
    {
        if(ImGui::Button(name.c_str()))
        {
            auto widget = std::make_shared<raymarcher::EditWidget>(name, code);
            windowManager.AddWidget(widget);
        }
    }
}

