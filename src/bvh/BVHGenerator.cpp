#include <iostream>

#include <spdlog/spdlog.h>

#include "bvh/BVHGenerator.hpp"

using namespace raymarcher;

BVHGenerator::BVHGenerator(size_t seed)
{
  generator.seed(seed);
}

auto
BVHGenerator::GenerateCluster(glm::vec3 pos, float deviation, size_t count)
  -> Scene
{
  Scene scene;
  std::uniform_real_distribution<> dis(-deviation, deviation);

  for (size_t i = 0; i < count; i++) {
    auto posDelta = glm::vec3(dis(generator), dis(generator), dis(generator));
    auto newPos = pos + posDelta;
    float radius = abs(dis(generator)) * 0.1;
    scene.push_back(std::make_shared<SpherePrimitive>(newPos, radius));
  }
  return scene;
}

auto
BVHGenerator::GenerateClusteredScene(size_t clusters,
                                     float dev,
                                     BoundingBox area) -> Scene
{
  auto maxSide = std::max(area.size.x, std::max(area.size.y, area.size.z));
  std::uniform_real_distribution<> dis(dev * 0.5, dev);
  std::uniform_real_distribution<> disNumber(10, 150);
  std::uniform_real_distribution<> disX(-area.size.x, area.size.x);
  std::uniform_real_distribution<> disY(-area.size.y, area.size.y);
  std::uniform_real_distribution<> disZ(-area.size.z, area.size.z);

  Scene scene;
  for (size_t i = 0; i < clusters; i++) {
    auto maxDeviation = dis(generator);
    auto maxElements = disNumber(generator);
    auto clusterPosition =
      area.pos + glm::vec3(disX(generator), disY(generator), disZ(generator));

    spdlog::trace("Cluster position: {}, {}, {}",
                  clusterPosition.x,
                  clusterPosition.y,
                  clusterPosition.z);

    auto elements = GenerateCluster(clusterPosition, maxDeviation, maxElements);
    for (auto& element : elements) {
      scene.push_back(element);
    }
  }
  return scene;
}
