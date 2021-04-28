#include "BVHCalculator.hpp"

#include <vector>
#include <algorithm>
#include <sstream>

#include <glm/glm.hpp>
#include <imgui.h>

#include "EditWidget.hpp"

using namespace raymarcher;
namespace raymarcher
{
    class SpherePrimitive
    {
        public:
        using ChildrenType = std::vector<std::shared_ptr<SpherePrimitive>>;
        /// Construct bouding sphere for children
        SpherePrimitive(ChildrenType childrenInst) :
            children { std::move(childrenInst) }
        {
            center = glm::vec3(0.0);
            for(auto child: children)
            {
                center += child->GetCenter();
            }
            center = center / glm::vec3(children.size());

            size = 0;
            for(auto child: children)
            {
                size = std::max(size, glm::length(center - child->GetCenter())+child->GetSize());
            }
        }

        /// Construct sphere
        SpherePrimitive(glm::vec3 centerInst, float sizeInst) :
            center { centerInst },
            size { sizeInst }
        {
        }
        auto GetChildren() -> ChildrenType& { return children; }
        auto GetCenter() -> glm::vec3 { return center; }
        auto GetSize() -> float { return size; }

        double DistanceTo(SpherePrimitive& other)
        {
            auto d = glm::length(other.GetCenter()-GetCenter()) - (other.GetSize()+GetSize());
            return d;
        }
        protected:
        ChildrenType children;

        glm::vec3 center;
        float size;
    };

    class BVHCalculatorPimpl
    {
        public:
            SpherePrimitive::ChildrenType scene;
            auto GenerateCodeForNode(SpherePrimitive& node, size_t level) -> std::string
            {
                std::string padding(level*2, ' ');
                std::stringstream ss;
                const auto minDistance = "minDistance";
                std::stringstream nodeString;
                std::string position = "vec3("+std::to_string(node.GetCenter().x) + "," +
                                           std::to_string(node.GetCenter().y) + "," +
                                           std::to_string(node.GetCenter().z) + ")";
 
                nodeString << "sphere(pos-" << position << ", " << node.GetSize() << ")";
                ss << padding <<  "if ( " << nodeString.str() << " < " << minDistance << ")" << std::endl;
                ss << padding <<  "{" << std::endl;
                if(node.GetChildren().size() == 0)
                {
                    ss << padding <<  "d = min(d, " << nodeString.str() << ");" << std::endl;
                } else {
                    for(auto& child: node.GetChildren())
                    {
                        ss <<  GenerateCodeForNode(*child, level+1)  << std::endl;
                    }
                }
                ss << padding <<  "}" << std::endl;
                ss << padding <<  "else { d = min(d, " << nodeString.str() << "); }" << std::endl;
                return ss.str();
            }
            auto GenerateCodeForScene() -> std::string
            {
                std::stringstream ss;
                ss << "uniform float minDistanceUniform = 1.0;" << std::endl;
                ss << "vec4 df(vec3 pos)" << std::endl;
                ss << "{" << std::endl;
                ss << "float minDistance = minDistanceUniform * 10.0;" << std::endl;
                ss << "float d = 1000.0;" << std::endl;
                ss << GenerateCodeForNode(*scene[0], 0);
                ss << "return vec4(d, vec3(1.0));" << std::endl;
                ss << "}" << std::endl;
                return ss.str();
            }
            auto GenerateCodeForNodeNonOptimized(SpherePrimitive& node, size_t level) -> std::string
            {
                std::string padding(level*2, ' ');
                std::stringstream ss;
                std::stringstream nodeString;
                std::string position = "vec3("+std::to_string(node.GetCenter().x) + "," +
                                           std::to_string(node.GetCenter().y) + "," +
                                           std::to_string(node.GetCenter().z) + ")";
                if(node.GetChildren().size() == 0)
                {
                    ss <<  "d = min(d, " << nodeString.str() << ");" << std::endl;
                } else {
                    for(auto& child: node.GetChildren())
                    {
                        ss << GenerateCodeForNodeNonOptimized(*child, level+1)  << std::endl;
                    }
                }
                return ss.str();
            }
            auto GenerateCodeForSceneNonOptimized() -> std::string
            {
                std::stringstream ss;
                ss << "uniform float minDistance = 1.0;" << std::endl;
                ss << "vec4 df(vec3 pos)" << std::endl;
                ss << "{" << std::endl;
                ss << "float d = 1000.0;" << std::endl;
                ss << GenerateCodeForNode(*scene[0], 0);
                ss << "return vec4(d, vec3(1.0));" << std::endl;
                ss << "}" << std::endl;
                return ss.str();
            }
            auto GenerateCode(bool optimized) -> std::string
            {
                if(optimized)
                    return GenerateCodeForScene();
                return GenerateCodeForSceneNonOptimized();
            }
    };
}

BVHCalculator::BVHCalculator() :
    pimpl { std::make_unique<BVHCalculatorPimpl>() }
{
    pimpl->scene.push_back(std::make_shared<SpherePrimitive>( glm::vec3(0.0, 0.0, 0.0), 1.0));
    pimpl->scene.push_back(std::make_shared<SpherePrimitive>( glm::vec3(1.0, 0.0, 0.0), 1.0));

    pimpl->scene.push_back(std::make_shared<SpherePrimitive>( glm::vec3(3.0, 0.0, 0.0), 1.0));
    pimpl->scene.push_back(std::make_shared<SpherePrimitive>( glm::vec3(4.0, 0.0, 0.0), 1.0));

    pimpl->scene.push_back(std::make_shared<SpherePrimitive>( glm::vec3(10.0, 0.0, 0.0), 1.0));
    pimpl->scene.push_back(std::make_shared<SpherePrimitive>( glm::vec3(11.0, 0.0, 0.0), 1.0));

    pimpl->scene.push_back(std::make_shared<SpherePrimitive>( glm::vec3(13.0, 0.0, 0.0), 1.0));
    pimpl->scene.push_back(std::make_shared<SpherePrimitive>( glm::vec3(14.0, 0.0, 0.0), 1.0));
    pimpl->scene.push_back(std::make_shared<SpherePrimitive>( glm::vec3(0.0, 0.0, 0.0), 1.0));
    pimpl->scene.push_back(std::make_shared<SpherePrimitive>( glm::vec3(0.0, 1.0, 0.0), 1.0));
    pimpl->scene.push_back(std::make_shared<SpherePrimitive>( glm::vec3(0.0, 0.0, 0.0), 1.0));
    pimpl->scene.push_back(std::make_shared<SpherePrimitive>( glm::vec3(1.0, 0.0, 0.0), 1.0));
    pimpl->scene.push_back(std::make_shared<SpherePrimitive>( glm::vec3(1.0, 0.0, -1.0), 1.0));
    pimpl->scene.push_back(std::make_shared<SpherePrimitive>( glm::vec3(1.0, 0.0, -1.1), 1.0));
}

BVHCalculator::~BVHCalculator() = default;

auto BVHCalculator::RenderContent() -> void
{
    if(ImGui::Button("Generate optimized code"))
    {
        Generate(true);

        auto widget = std::make_shared<raymarcher::EditWidget>("Generated: optimized", pimpl->GenerateCode(true));
        AddWidget(widget);

        auto widget2 = std::make_shared<raymarcher::EditWidget>("Generated: non-optimized", pimpl->GenerateCode(false));
        AddWidget(widget2);
    }

    ImGui::Text("Elements");
    for(auto& element: pimpl->scene)
    {
        DisplayElement(*element, 0);
    }

    WindowWidget::RenderContent();
}

auto BVHCalculator::DisplayElement(SpherePrimitive& element, size_t level) -> void
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

auto BVHCalculator::Generate(bool isOptimized) -> void
{
        while(pimpl->scene.size() > 1)
        {
            const auto count = pimpl->scene.size();
            double minSize = 0.0;
            size_t indices[2] = {0, 1};

            for(size_t i = 0; i < count; i++)
            {
                for(size_t j = i+1; j < count; j++)
                {
                    auto distance = pimpl->scene[i]->DistanceTo(*pimpl->scene[j]);
                    //auto primitive = SpherePrimitive({pimpl->scene[i], pimpl->scene[j]});
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

            auto firstElement = pimpl->scene[indices[0]];
            auto secondElement = pimpl->scene[indices[1]];

            pimpl->scene.erase(pimpl->scene.begin() + firstIndex);
            pimpl->scene.erase(pimpl->scene.begin() + secondIndex);
            pimpl->scene.emplace_back(std::make_shared<SpherePrimitive>(SpherePrimitive::ChildrenType{firstElement,secondElement}));
        }

}
