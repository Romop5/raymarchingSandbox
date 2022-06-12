#ifndef RAYMARCHER_BVHLIBRARY_HPP
#define RAYMARCHER_BVHLIBRARY_HPP

#include <memory>
#include <string>
#include <vector>

#include <glm/glm.hpp>

namespace raymarcher {
/**
 * @brief A base node type in BVH hierarchy
 *
 * This class is used for both primitives (spheres) and bouding volumes
 * (spheres) IsBoudingSphere() method is used to differentiate between each
 * other
 */
class SpherePrimitive
{
public:
  using ChildrenType = std::vector<std::shared_ptr<SpherePrimitive>>;
  /// Construct bouding sphere for children
  SpherePrimitive(ChildrenType childrenInst);
  /// Construct sphere
  SpherePrimitive(glm::vec3 centerInst, float sizeInst);

  /* ------Structure methods--------------------------------------------*/
  auto GetChildren() -> ChildrenType&;
  auto GetCenter() -> glm::vec3;
  auto GetSize() -> float;
  auto IsBoudingSphere() const -> bool;

  /// Calculate distance of two spheres, taking radii into account
  auto DistanceTo(SpherePrimitive& other) -> double;

  /* ------Optimization methods-----------------------------------------*/

  /// Collapse subtree to an array of leafs in current node
  auto InflateChildren() -> void;

  /// Colapse subtree if level == 0
  auto InflateSinceLevel(size_t level) -> void;

  /// Steal subnode's children if current vs node's volume ratio is bigger than
  /// threshold
  auto InflateUsingSAH(float threshold) -> void;

protected:
  auto ConstructBoudingBoxForChildren() -> void;
  bool isBoundingSphere;
  ChildrenType children;

  glm::vec3 center;
  float size;
};

/**
 * @brief Implements manipulation, optimization and code generation of
 * sphere-based scene
 */
class BVHLibrary
{
public:
  /// Parameters for optimization of BVH structure
  struct OptimizationParameters
  {
    /// Remove one level of tree when ratio of parent-child volume is bigger
    /// than
    float SAHthreshold = 1.0;

    /// Inflate all nodes from maxLevel and lower
    size_t maxLevel = 32;
  };

  /*
   * @brief Generate SDF in GLSL for given scene
   * @param optimized determines if resulting code should take BVH into account,
   * or print out just the leaves (without any acceleration)
   */
  auto GenerateCode(bool optimized) -> std::string;

  auto GetScene() -> SpherePrimitive::ChildrenType&;
  auto SetScene(SpherePrimitive::ChildrenType scene) -> void;

  auto SetParams(OptimizationParameters& params) -> void;
  auto GetParams() const -> const OptimizationParameters&;

  /**
   * @brief Constructs and optimize scene into BVH structure
   *
   * This function takes a flat array of nodes in the scene, and constructs a
   * full binary BVH tree. Subsequently, the tree is optimized using parameters
   * 'params'.
   *
   * Two levels of optimization are employed (in the following order):
   * - pseudo Surface Area Heuristic
   *   By comparing volumes of parent and child node, it can be detected what's
   * the probability that the SDF will be evaluated for the child provided it
   * was evaluated for parent.
   *
   *   The idea is that if child has similar radius to parent, then it's useless
   * in BVH hierarchy, and it can be removed by moving its children to parent.
   *
   * - inflating since fixed level: e.g. if maxLevel is 5 and the BVH tree has
   * 12 levels, all subtrees are flattened since level 5
   */
  auto Optimize() -> void;

protected:
  auto GenerateCodeForNode(SpherePrimitive& node, size_t level) -> std::string;
  auto GenerateCodeForScene() -> std::string;
  auto GenerateCodeForNodeNonOptimized(SpherePrimitive& node, size_t level)
    -> std::string;
  auto GenerateCodeForSceneNonOptimized() -> std::string;

  auto ConstructTree() -> void;
  SpherePrimitive::ChildrenType scene;
  OptimizationParameters params;
};
}
#endif
