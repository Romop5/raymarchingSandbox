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
        auto IsBoudingSphere() const -> bool;

        auto DistanceTo(SpherePrimitive& other) -> double;

        auto InflateChildren() -> void;
        auto InflateSinceLevel(size_t level) -> void;
        auto InflateUsingSAH(float threshold) -> void;

        protected:
        bool isBoundingSphere;
        ChildrenType children;

        glm::vec3 center;
        float size;
    };

    class BVHLibrary
    {
        public:
        struct OptimizationParameters
        {
            float   SAHthreshold    = 1.0;
            size_t  maxLevel        = 32;
        };

        auto GenerateCodeForNode(SpherePrimitive& node, size_t level) -> std::string;
        auto GenerateCodeForScene() -> std::string;
        auto GenerateCodeForNodeNonOptimized(SpherePrimitive& node, size_t level) -> std::string;
        auto GenerateCodeForSceneNonOptimized() -> std::string;
        auto GenerateCode(bool optimized) -> std::string;

        auto GetScene() -> SpherePrimitive::ChildrenType&;
        auto SetScene(SpherePrimitive::ChildrenType scene) -> void;

        auto SetParams(OptimizationParameters& params) -> void;
        auto GetParams() const -> const OptimizationParameters&;

        auto Optimize() -> void;
        protected:
        auto ConstructTree() -> void;
        SpherePrimitive::ChildrenType scene;
        OptimizationParameters params;
    };
}
#endif
