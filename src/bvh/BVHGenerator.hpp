#ifndef RAYMARCHER_BVH_GENERATOR_HPP
#define RAYMARCHER_BVH_GENERATOR_HPP

#include "bvh/BVHLibrary.hpp"
#include <glm/glm.hpp>
#include <random>
#include <vector>

namespace raymarcher {
/**
 * @brief Generates deterministically datasets for testing optimization
 * parameters
 */
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

  /// Generate cluster of 'count' primitives at position 'pos' within 'dev'
  /// distance
  auto GenerateCluster(glm::vec3 pos, float deviation, size_t count) -> Scene;

  /// Generate clusters within 'area'. 'Dev' determines spread of a single
  /// cluster
  auto GenerateClusteredScene(size_t clusters, float dev, BoundingBox area)
    -> Scene;

private:
  std::mt19937 generator;
};

}

#endif
