#include "BVHLibrary.hpp"

#include <sstream>
#include <queue>

using namespace raymarcher;
SpherePrimitive::SpherePrimitive(ChildrenType childrenInst) :
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
SpherePrimitive::SpherePrimitive(glm::vec3 centerInst, float sizeInst) :
    center { centerInst },
    size { sizeInst }
{
}

auto SpherePrimitive::GetChildren() -> ChildrenType& 
{ 
    return children; 
}

auto SpherePrimitive::GetCenter() -> glm::vec3 
{ 
    return center; 
}

auto SpherePrimitive::GetSize() -> float 
{ 
    return size; 
}

double SpherePrimitive::DistanceTo(SpherePrimitive& other)
{
    auto d = glm::length(other.GetCenter()-GetCenter()) + (other.GetSize()+GetSize());
    return d;
}

auto SpherePrimitive::InflateChildren() -> void
{
    ChildrenType newChildren;

    std::queue<std::shared_ptr<SpherePrimitive>> remainingNodes;
    for(auto& child: children)
    {
        remainingNodes.push(child);
    }

    while(!remainingNodes.empty())
    {
        auto current = remainingNodes.front();
        remainingNodes.pop();

        if(current->children.empty())
        {
            newChildren.push_back(current);
            continue;
        }
        for(auto& subnode: current->children)
        {
            remainingNodes.push(subnode);
        }
    }

    children = std::move(newChildren);
}

auto SpherePrimitive::InflateSinceLevel(size_t level) -> void
{
    if(level == 0)
    {
        InflateChildren();
        return;
    }
    for(auto& child: children)
    {
        child->InflateSinceLevel(level - 1);
    }
}

//------------------------------------------------------------------------
// BVHLibrary
//------------------------------------------------------------------------
auto BVHLibrary::GenerateCodeForNode(SpherePrimitive& node, size_t level) -> std::string
{
    std::string padding(level*2, ' ');
    std::stringstream ss;
    const auto minDistance = "minDistance";
    std::stringstream nodeString;
    std::string position = "vec3("+std::to_string(node.GetCenter().x) + "," +
                               std::to_string(node.GetCenter().y) + "," +
                               std::to_string(node.GetCenter().z) + ")";

    nodeString << "sphere(pos-" << position << ", " << node.GetSize() << ")";
    if(node.GetChildren().size() == 0)
    {
        ss << padding <<  "d = min(d, " << nodeString.str() << ");" << std::endl;
    } else {
        ss << padding <<  "if ( " << nodeString.str() << " < " << minDistance << ")" << std::endl;
        ss << padding <<  "{" << std::endl;
        for(auto& child: node.GetChildren())
        {
            ss <<  GenerateCodeForNode(*child, level+1)  << std::endl;
        }
        ss << padding <<  "}" << std::endl;
        ss << padding <<  "else { d = min(d, " << nodeString.str() << "); }" << std::endl;
    }
    return ss.str();
}

auto BVHLibrary::GenerateCodeForScene() -> std::string
{
    std::stringstream ss;
    ss << "uniform float minDistanceUniform = 0.1;" << std::endl;
    ss << "vec4 df(vec3 pos)" << std::endl;
    ss << "{" << std::endl;
    ss << "float minDistance = minDistanceUniform * 10.0;" << std::endl;
    ss << "float d = 1000.0;" << std::endl;
    for(auto& node: scene)
    {
        ss << GenerateCodeForNode(*node, 0);
    }
    ss << "return min(ground(pos, -1.0), vec4(d, vec3(1.0)));" << std::endl;
    ss << "}" << std::endl;
    return ss.str();
}

auto BVHLibrary::GenerateCodeForNodeNonOptimized(SpherePrimitive& node, size_t level) -> std::string
{
    std::string padding(level*2, ' ');
    std::stringstream ss;
    std::stringstream nodeString;
    std::string position = "vec3("+std::to_string(node.GetCenter().x) + "," +
                               std::to_string(node.GetCenter().y) + "," +
                               std::to_string(node.GetCenter().z) + ")";
    nodeString << "sphere(pos-" << position << ", " << node.GetSize() << ")";
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

auto BVHLibrary::GenerateCodeForSceneNonOptimized() -> std::string
{
    std::stringstream ss;
    ss << "vec4 df(vec3 pos)" << std::endl;
    ss << "{" << std::endl;
    ss << "float d = 1000.0;" << std::endl;
    for(auto& node: scene)
    {
        ss << GenerateCodeForNodeNonOptimized(*node, 0);
    }
    ss << "return min(ground(pos, -1.0), vec4(d, vec3(1.0)));" << std::endl;
    ss << "}" << std::endl;
    return ss.str();
}

auto BVHLibrary::GenerateCode(bool optimized) -> std::string
{
    if(optimized)
        return GenerateCodeForScene();
    return GenerateCodeForSceneNonOptimized();
}

auto BVHLibrary::GetScene() -> SpherePrimitive::ChildrenType&
{
    return scene;
}
