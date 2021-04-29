#ifndef RAYMARCHER_BVHLIBRARY_HPP
#define RAYMARCHER_BVHLIBRARY_HPP

#include <vector>
#include <string>
#include <memory>
#include <glm/glm.hpp>

namespace raymarcher 
{
    class SpherePrimitive
    {
        public:
        using ChildrenType = std::vector<std::shared_ptr<SpherePrimitive>>;
        /// Construct bouding sphere for children
        SpherePrimitive(ChildrenType childrenInst);
        /// Construct sphere
        SpherePrimitive(glm::vec3 centerInst, float sizeInst);

        auto GetChildren() -> ChildrenType&;
        auto GetCenter() -> glm::vec3;
        auto GetSize() -> float;

        auto DistanceTo(SpherePrimitive& other) -> double;

        auto InflateChildren() -> void;
        auto InflateSinceLevel(size_t level) -> void;
        protected:
        ChildrenType children;

        glm::vec3 center;
        float size;
    };

    class BVHLibrary
    {
        public:
        auto GenerateCodeForNode(SpherePrimitive& node, size_t level) -> std::string;
        auto GenerateCodeForScene() -> std::string;
        auto GenerateCodeForNodeNonOptimized(SpherePrimitive& node, size_t level) -> std::string;
        auto GenerateCodeForSceneNonOptimized() -> std::string;
        auto GenerateCode(bool optimized) -> std::string;

        auto GetScene() -> SpherePrimitive::ChildrenType&;
        protected:
        SpherePrimitive::ChildrenType scene;
    };
}
#endif
