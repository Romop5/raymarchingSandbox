#ifndef RAYMARCHER_BVH_GENERATOR_HPP
#define RAYMARCHER_BVH_GENERATOR_HPP

#include <vector>
#include <random>
#include <glm/glm.hpp>
#include "BVHLibrary.hpp"

namespace raymarcher
{
class BVHGenerator
{
    public:
    explicit BVHGenerator(size_t seed);

    struct BoundingBox
    {
        glm::vec3 pos;
        glm::vec3 size;
    };

    using Scene = SpherePrimitive::ChildrenType;

    auto GenerateCluster(glm::vec3 pos, float deviation, size_t count) -> Scene;
    auto GenerateClusteredScene(size_t clusters, float dev, BoundingBox area) -> Scene;

    private:
    std::mt19937 generator;
};

}

#endif
