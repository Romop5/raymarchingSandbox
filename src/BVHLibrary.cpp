#include "BVHLibrary.hpp"

#include <sstream>
#include <queue>
#include <algorithm>

#include <glm/gtc/constants.hpp>

using namespace raymarcher;

namespace 
{
    auto GetSphereVolume(float r) -> float
    {
        return 1.3333333*glm::pi<float>()*pow(r, 3.0f);
    }

    auto GetSphereToSphereVolumeRatio(float smallerRadius, float biggerRadius) -> float
    {
        return GetSphereVolume(smallerRadius)/GetSphereVolume(biggerRadius); 
    }
}

SpherePrimitive::SpherePrimitive(ChildrenType childrenInst) :
    children { std::move(childrenInst) },
    isBoundingSphere { true }
{
    ConstructBoudingBoxForChildren();
}

/// Construct sphere
SpherePrimitive::SpherePrimitive(glm::vec3 centerInst, float sizeInst) :
    center { centerInst },
    size { sizeInst },
    isBoundingSphere { false }
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

auto SpherePrimitive::IsBoudingSphere() const -> bool
{
    return isBoundingSphere;
}

double SpherePrimitive::DistanceTo(SpherePrimitive& other)
{
    // Note: we add radii to prevent a single, large sphere to be close to all spheres due to it's
    // large radius
    // This "distance function" prefers small and close spheres to large-to-small sphere distance
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

auto SpherePrimitive::InflateUsingSAH(float threshold) -> void
{
    // Skip optimization of leafs
    if(!IsBoudingSphere())
    {
        return;
    }

    // Vector iterator does not allow linear pass with appending comfortably
    // so we keep new children aside
    SpherePrimitive::ChildrenType toBeAdded;
    bool hasAddedChild = false;
    const auto size = GetSize();

    // Until convergence: while there is any subnode's child that has been added to its parent
    do {
        bool hasAddedChild = false;

        // For each subnode: determine SAH
        for(auto& child: children)
        {
            const auto childSize = child->GetSize();
            if(child->IsBoudingSphere() && GetSphereToSphereVolumeRatio(childSize, size) > threshold)
            {
                // If subnode is too big -> remove one level by inflating subnode
                // into parent
                for(auto& child: child->GetChildren())
                {
                    toBeAdded.push_back(child);
                    hasAddedChild = true;
                }
                child->GetChildren().clear();
            }
        }

        // Remove empty nodes
        std::remove_if(children.begin(), children.end(), [](auto& child)
        {
            return (child->IsBoudingSphere() && child->GetChildren().size() == 0);
        });
        // Append children of subnode to parent
        for(auto& child: toBeAdded)
        {
            children.push_back(child);
        }
    } while(hasAddedChild);

    // Call inflate recursively for each child
    for(auto& child: children)
    {
        child->InflateUsingSAH(threshold);
    }
}

auto SpherePrimitive::ConstructBoudingBoxForChildren() -> void
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

    if(node.GetChildren().size() == 0)
    {
        nodeString << "sphere(pos-" << position << ", " << node.GetSize() << "*time)";
        ss << padding <<  "d = smin(d, " << nodeString.str() << ", k);" << std::endl;
    } else {
        nodeString << "sphere(pos-" << position << ", " << node.GetSize() << ")";
        ss << padding <<  "if ( " << nodeString.str() << " < " << minDistance << ")" << std::endl;
        ss << padding <<  "{" << std::endl;
        for(auto& child: node.GetChildren())
        {
            ss <<  GenerateCodeForNode(*child, level+1);
        }
        ss << padding <<  "}" << std::endl;
        ss << padding <<  "else { d = smin(d, " << nodeString.str() << ", k); }" << std::endl;
    }
    return ss.str();
}

auto BVHLibrary::GenerateCodeForScene() -> std::string
{
    std::stringstream ss;
    ss << "// polynomial smooth min (k = 0.1);" << std::endl;
    ss << "float smin( float a, float b, float k ) " << std::endl;
    ss << "{" << std::endl;
    ss << "  float h = clamp( 0.5+0.5*(b-a)/k, 0.0, 1.0 );" << std::endl;
    ss << "  return mix( b, a, h ) - k*h*(1.0-h);" << std::endl;
    ss << "}" << std::endl;
    ss << "float time = fract(iTime);" << std::endl;

    ss << "uniform float k = 0.1;" << std::endl;

    ss << "uniform float minDistanceUniform = 0.1;" << std::endl;
    ss << "vec4 df(vec3 pos)" << std::endl;
    ss << "{" << std::endl;
    ss << "float minDistance = minDistanceUniform * 10.0;" << std::endl;
    ss << "float d = 1000.0;" << std::endl;
    for(auto& node: scene)
    {
        ss << GenerateCodeForNode(*node, 0);
    }
    ss << "return unite(ground(pos, -1.0), vec4(d, vec3(1.0)));" << std::endl;
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
        ss <<  "d = smin(d, " << nodeString.str() << ", d);" << std::endl;
    } else {
        for(auto& child: node.GetChildren())
        {
            ss << GenerateCodeForNodeNonOptimized(*child, level+1);
        }
    }
    return ss.str();
}

auto BVHLibrary::GenerateCodeForSceneNonOptimized() -> std::string
{
    std::stringstream ss;

    ss << "// polynomial smooth min (k = 0.1);" << std::endl;
    ss << "float smin( float a, float b, float k ) " << std::endl;
    ss << "{" << std::endl;
    ss << "  float h = clamp( 0.5+0.5*(b-a)/k, 0.0, 1.0 );" << std::endl;
    ss << "  return mix( b, a, h ) - k*h*(1.0-h);" << std::endl;
    ss << "}" << std::endl;

    ss << "float k = 0.1;" << std::endl;

    ss << "vec4 df(vec3 pos)" << std::endl;
    ss << "{" << std::endl;
    ss << "float d = 1000.0;" << std::endl;
    for(auto& node: scene)
    {
        ss << GenerateCodeForNodeNonOptimized(*node, 0);
    }
    ss << "return unite(ground(pos, -1.0), vec4(d, vec3(1.0)));" << std::endl;
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

auto BVHLibrary::SetScene(SpherePrimitive::ChildrenType scene) -> void
{
    this->scene = std::move(scene);
}


auto BVHLibrary::SetParams(OptimizationParameters& params) -> void
{
    this->params = params;
}

auto BVHLibrary::GetParams() const -> const OptimizationParameters&
{
    return this->params;
}

auto BVHLibrary::Optimize() -> void
{
    // Construct raw binary BVH tree, which may be suboptimal
    ConstructTree();
    
    // Inflate tree since maxLevel -> bounds the depth to maxLevel
    scene[0]->InflateUsingSAH(params.SAHthreshold);

    // Inflate tree since maxLevel -> bounds the depth to maxLevel
    scene[0]->InflateSinceLevel(params.maxLevel);
}

// Constructs binary BVH tree (thus each node has two subnodes or it's a leaf)
// The tree may not be balanced. 
//
// DistanceTo() methods used in each step of construction to join two closest 
// nodes at the level
auto BVHLibrary::ConstructTree() -> void
{
    while(scene.size() > 1)
    {
        const auto count = scene.size();
        double minSize = 0.0;
        size_t indices[2] = {0, 1};

        /*
         * Find the closest two nodes at current level. This has
         * terrible O(n^2) complexity.
         */
        for(size_t i = 0; i < count; i++)
        {
            for(size_t j = i+1; j < count; j++)
            {
                auto distance = scene[i]->DistanceTo(*scene[j]);
                //auto primitive = SpherePrimitive({scene[i], scene[j]});
                if(distance < minSize)
                {
                    minSize = distance;
                    indices[0] = i;
                    indices[1] = j;
                }
            }
        }

        /*
         * Remove found nodes from current level and insert a new, joined node with two subnodes 
         */
        auto firstIndex = std::max(indices[0], indices[1]);
        auto secondIndex = std::min(indices[0], indices[1]);

        auto firstElement = scene[indices[0]];
        auto secondElement = scene[indices[1]];

        scene.erase(scene.begin() + firstIndex);
        scene.erase(scene.begin() + secondIndex);
        scene.emplace_back(std::make_shared<SpherePrimitive>(SpherePrimitive::ChildrenType{firstElement,secondElement}));
    }
}

