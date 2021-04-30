#include "BVHCalculatorWidget.hpp"

#include <vector>
#include <algorithm>
#include <sstream>
#include <queue>
#include <random>

#include <glm/glm.hpp>
#include <imgui.h>

#include "EditWidget.hpp"
#include "FileHelper.hpp"
#include "BVHLibrary.hpp"

using namespace raymarcher;

BVHCalculatorWidget::BVHCalculatorWidget() :
    pimpl { std::make_unique<BVHLibrary>() }
{
    SetSize(400, 0);
    // Small (close) scene
    auto& scene = pimpl->GetScene();

    /* scene.push_back(std::make_shared<SpherePrimitive>( glm::vec3(0.0, 0.0, 0.0), 0.1)); */
    /* scene.push_back(std::make_shared<SpherePrimitive>( glm::vec3(0.0, 0.0, 0.1), 0.09)); */
    /* scene.push_back(std::make_shared<SpherePrimitive>( glm::vec3(0.0, 0.1, 0.1), 0.13)); */
    /* scene.push_back(std::make_shared<SpherePrimitive>( glm::vec3(0.1, 0.0, 0.1), 0.09)); */
    /* scene.push_back(std::make_shared<SpherePrimitive>( glm::vec3(0.2, 0.1, 0.1), 0.13)); */

    /* scene.push_back(std::make_shared<SpherePrimitive>( glm::vec3(0.1, 0.0, 0.0), 0.1)); */
    /* scene.push_back(std::make_shared<SpherePrimitive>( glm::vec3(0.1, 0.0, 0.1), 0.09)); */
    /* scene.push_back(std::make_shared<SpherePrimitive>( glm::vec3(0.2, 0.1, 0.1), 0.13)); */
    /* scene.push_back(std::make_shared<SpherePrimitive>( glm::vec3(0.2, 0.0, 0.1), 0.09)); */

    /* scene.push_back(std::make_shared<SpherePrimitive>( glm::vec3(0.1, 0.1, 0.0), 0.1)); */
    /* scene.push_back(std::make_shared<SpherePrimitive>( glm::vec3(0.1, 0.1, 0.1), 0.09)); */
    /* scene.push_back(std::make_shared<SpherePrimitive>( glm::vec3(0.2, 0.2, 0.1), 0.13)); */
    /* scene.push_back(std::make_shared<SpherePrimitive>( glm::vec3(0.2, 0.1, 0.1), 0.09)); */


    /* scene.push_back(std::make_shared<SpherePrimitive>( glm::vec3(0.0, 0.0, 3.0), 0.1)); */
    /* scene.push_back(std::make_shared<SpherePrimitive>( glm::vec3(0.0, 0.0, 3.1), 0.09)); */
    /* scene.push_back(std::make_shared<SpherePrimitive>( glm::vec3(0.0, 0.1, 3.1), 0.13)); */
    /* scene.push_back(std::make_shared<SpherePrimitive>( glm::vec3(0.1, 0.0, 3.1), 0.09)); */
    /* scene.push_back(std::make_shared<SpherePrimitive>( glm::vec3(0.2, 0.1, 3.1), 0.13)); */

    /* scene.push_back(std::make_shared<SpherePrimitive>( glm::vec3(0.1, 0.0, 3.0), 0.1)); */
    /* scene.push_back(std::make_shared<SpherePrimitive>( glm::vec3(0.1, 0.0, 3.1), 0.09)); */
    /* scene.push_back(std::make_shared<SpherePrimitive>( glm::vec3(0.2, 0.1, 3.1), 0.13)); */
    /* scene.push_back(std::make_shared<SpherePrimitive>( glm::vec3(0.2, 0.0, 3.1), 0.09)); */

    /* scene.push_back(std::make_shared<SpherePrimitive>( glm::vec3(0.1, 0.1, 3.0), 0.1)); */
    /* scene.push_back(std::make_shared<SpherePrimitive>( glm::vec3(0.1, 0.1, 3.1), 0.09)); */
    /* scene.push_back(std::make_shared<SpherePrimitive>( glm::vec3(0.2, 0.2, 3.1), 0.13)); */

    // Larger scene
    /* scene.push_back(std::make_shared<SpherePrimitive>( glm::vec3(0.0, 0.0, 0.0), 1.0)); */
    /* scene.push_back(std::make_shared<SpherePrimitive>( glm::vec3(1.0, 0.0, 0.0), 1.0)); */

    /* scene.push_back(std::make_shared<SpherePrimitive>( glm::vec3(3.0, 0.0, 0.0), 1.0)); */
    /* scene.push_back(std::make_shared<SpherePrimitive>( glm::vec3(4.0, 0.0, 0.0), 1.0)); */

    /* scene.push_back(std::make_shared<SpherePrimitive>( glm::vec3(10.0, 0.0, 0.0), 1.0)); */
    /* scene.push_back(std::make_shared<SpherePrimitive>( glm::vec3(11.0, 0.0, 0.0), 1.0)); */

    /* scene.push_back(std::make_shared<SpherePrimitive>( glm::vec3(13.0, 0.0, 0.0), 1.0)); */
    /* scene.push_back(std::make_shared<SpherePrimitive>( glm::vec3(14.0, 0.0, 0.0), 1.0)); */
    /* scene.push_back(std::make_shared<SpherePrimitive>( glm::vec3(0.0, 0.0, 0.0), 1.0)); */
    /* scene.push_back(std::make_shared<SpherePrimitive>( glm::vec3(0.0, 1.0, 0.0), 1.5)); */
    /* scene.push_back(std::make_shared<SpherePrimitive>( glm::vec3(0.0, 0.0, 0.0), 1.2)); */
    /* scene.push_back(std::make_shared<SpherePrimitive>( glm::vec3(1.0, 0.0, 0.0), 1.0)); */
    /* scene.push_back(std::make_shared<SpherePrimitive>( glm::vec3(1.0, 0.0, -1.0), 1.1)); */
    /* scene.push_back(std::make_shared<SpherePrimitive>( glm::vec3(1.0, 0.0, -1.1), 1.0)); */

    /* scene.push_back(std::make_shared<SpherePrimitive>( glm::vec3(13.0, 2.0, 0.0), 1.1)); */
    /* scene.push_back(std::make_shared<SpherePrimitive>( glm::vec3(14.0, 1.0, 0.0), 1.0)); */
    /* scene.push_back(std::make_shared<SpherePrimitive>( glm::vec3(0.0, 0.0, 0.0), 3.0)); */
    /* scene.push_back(std::make_shared<SpherePrimitive>( glm::vec3(1.0, 1.0, 0.0), 0.1)); */
    /* scene.push_back(std::make_shared<SpherePrimitive>( glm::vec3(5.0, 3.0, 0.0), 1.0)); */
    /* scene.push_back(std::make_shared<SpherePrimitive>( glm::vec3(1.0, 5.0, 0.0), 1.3)); */
    /* scene.push_back(std::make_shared<SpherePrimitive>( glm::vec3(2.0, 3.0, -1.0), 1.5)); */
    /* scene.push_back(std::make_shared<SpherePrimitive>( glm::vec3(1.0, 0.5, -1.1), 1.0)); */

    /* scene.push_back(std::make_shared<SpherePrimitive>( glm::vec3(13.0, 2.0, -4.0), 1.1)); */
    /* scene.push_back(std::make_shared<SpherePrimitive>( glm::vec3(14.0, 1.0, -6.0), 1.0)); */
    /* scene.push_back(std::make_shared<SpherePrimitive>( glm::vec3(0.0, 0.0, -10.0), 3.0)); */
    /* scene.push_back(std::make_shared<SpherePrimitive>( glm::vec3(1.0, 1.0, -20.0), 0.1)); */
    /* scene.push_back(std::make_shared<SpherePrimitive>( glm::vec3(5.0, 3.0, -30.0), 1.0)); */
    /* scene.push_back(std::make_shared<SpherePrimitive>( glm::vec3(1.0, 5.0, -5.0), 1.3)); */
    /* scene.push_back(std::make_shared<SpherePrimitive>( glm::vec3(2.0, 3.0, -1.0), 1.5)); */
    /* scene.push_back(std::make_shared<SpherePrimitive>( glm::vec3(1.0, 0.5, -1.1), 1.0)); */

}

BVHCalculatorWidget::~BVHCalculatorWidget() = default;

auto BVHCalculatorWidget::RenderContent() -> void
{
    //--------------------------------------------------------------------------------------
    // Button controls
    //--------------------------------------------------------------------------------------
    if(ImGui::Button("Generate scene"))
    {
        GenerateGeometry();
    }
    ImGui::SameLine();
    if(ImGui::Button("Generate & save"))
    {
        Optimize(params);

        auto hasPassed = true;
        hasPassed &= FileHelper::SaveFile("optimized.sdf", pimpl->GenerateCode(true));
        hasPassed &= FileHelper::SaveFile("nonoptimized.sdf", pimpl->GenerateCode(false));
    }

    ImGui::SameLine();

    if(ImGui::Button("Generate optimized code"))
    {
        Optimize(params);

        auto widget = std::make_shared<raymarcher::EditWidget>("Generated: optimized", pimpl->GenerateCode(true));
        AddWidget(widget);

        auto widget2 = std::make_shared<raymarcher::EditWidget>("Generated: non-optimized", pimpl->GenerateCode(false));
        AddWidget(widget2);
    }

    ImGui::SameLine();
    if(ImGui::Button("Preview current scene"))
    {
        auto widget2 = std::make_shared<raymarcher::EditWidget>("Scene Preview: non-optimized", pimpl->GenerateCode(false));
        widget2->Recompile();
        AddWidget(widget2);
    }

    ImGui::SameLine();
    if(ImGui::Button("Close subwindows"))
    {
        RemoveAllWidgets();
    }

    //--------------------------------------------------------------------------------------
    // Parameters
    //--------------------------------------------------------------------------------------
    int maxLevel = params.maxLevel;
    ImGui::PushItemWidth(30);
    ImGui::SliderInt("Maximum level of BVH", &maxLevel, 0, 16);
    params.maxLevel = maxLevel;

    //--------------------------------------------------------------------------------------
    // Scene
    //--------------------------------------------------------------------------------------

    ImGui::Text("Elements");
    for(auto& element: pimpl->GetScene())
    {
        DisplayElement(*element, 0);
    }

    WindowWidget::RenderContent();
}

auto BVHCalculatorWidget::DisplayElement(SpherePrimitive& element, size_t level) -> void
{
    ImGui::PushID(&element);
    auto pos = element.GetCenter();
    auto size = element.GetSize();
    std::string padding(level, ' ');
    ImGui::Text("%sSphere: (%f, %f, %f) Size: %f", padding.c_str(), pos.x, pos.y, pos.z, size);

    if(element.GetChildren().size() > 0)
    {
        ImGui::Separator();
        for(auto& child: element.GetChildren())
        {
            DisplayElement(*child, level + 1);
        }
        ImGui::Separator();
    }
    ImGui::PopID();
}

auto BVHCalculatorWidget::Optimize(const OptimizationParameters params) -> void
{
        while(pimpl->GetScene().size() > 1)
        {
            const auto count = pimpl->GetScene().size();
            double minSize = 0.0;
            size_t indices[2] = {0, 1};

            for(size_t i = 0; i < count; i++)
            {
                for(size_t j = i+1; j < count; j++)
                {
                    auto distance = pimpl->GetScene()[i]->DistanceTo(*pimpl->GetScene()[j]);
                    //auto primitive = SpherePrimitive({pimpl->GetScene()[i], pimpl->GetScene()[j]});
                    if(distance < minSize)
                    {
                        minSize = distance;
                        indices[0] = i;
                        indices[1] = j;
                    }
                }
            }

            auto firstIndex = std::max(indices[0], indices[1]);
            auto secondIndex = std::min(indices[0], indices[1]);

            auto firstElement = pimpl->GetScene()[indices[0]];
            auto secondElement = pimpl->GetScene()[indices[1]];

            pimpl->GetScene().erase(pimpl->GetScene().begin() + firstIndex);
            pimpl->GetScene().erase(pimpl->GetScene().begin() + secondIndex);
            pimpl->GetScene().emplace_back(std::make_shared<SpherePrimitive>(SpherePrimitive::ChildrenType{firstElement,secondElement}));
        }

        // Inflate
        pimpl->GetScene()[0]->InflateSinceLevel(params.maxLevel);
}

auto BVHCalculatorWidget::GenerateGeometry() -> void
{
    std::random_device rd;  
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(1.0, 2.0);
    
    size_t maxNumberOfPrimitives = 30;
    for(size_t i = 0; i < maxNumberOfPrimitives; i++)
    {
        glm::vec3 pos = glm::vec3(dis(gen),dis(gen), dis(gen));
        float radius = dis(gen)*0.1;
        pimpl->GetScene().push_back(std::make_shared<SpherePrimitive>(pos, radius));
    }
}
