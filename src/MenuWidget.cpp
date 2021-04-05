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
float df(vec3 pos)
{
    float gd = ground(pos, floorElevation);
    //float a = sphere(pos, 1.0);
    //float ab = sphere(pos+vec3(0.5+sin(iTime),0.0,sin(iTime)*-0.0), 1.0);

    //float d = unite(a,b);
    return unite(gd,object(pos));
    //return uniteSmooth(ab,a);
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
        auto widget = std::make_shared<raymarcher::EditWidget>(SimpleSDFCode());
        auto window = std::make_shared<raymarcher::WindowWidget>();
        window->AddWidget(widget);
        window->SetTitle("New SDF function");
        windowManager.AddWidget(window);
    }
    
    static std::vector<std::pair<std::string, std::string>> primitives =
    {
        { "Sphere", "float df(vec3 pos) {\n    vec3 center = vec3(0.0);\n    float radius = 1.0;\n    return length(pos-center) - radius;\n}" }
    };

    for(auto& [name, code]: primitives)
    {
        if(ImGui::Button(name.c_str()))
        {
            auto widget = std::make_shared<raymarcher::EditWidget>(code);
            auto window = std::make_shared<raymarcher::WindowWidget>();
            window->AddWidget(widget);
            window->SetTitle(name);
            windowManager.AddWidget(window);
        }
    }
}

