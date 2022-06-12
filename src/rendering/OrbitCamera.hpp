#ifndef RAYMARCHER_ORBITCAMERA_HPP
#define RAYMARCHER_ORBITCAMERA_HPP

#include "rendering/ICamera.hpp"
namespace raymarcher {
/**
 * @brief Simple orbiter around point 'center' with radius 'radius'
 */
class OrbitCamera : public ICamera
{
public:
  OrbitCamera();
  auto GetTransformation() const -> const glm::mat4& override;

  auto SetCenter(glm::vec3 position) -> void;
  auto SetDistance(float newRadius) -> void;
  auto GetDistance() const -> float;

  auto RotateLeft() -> void;
  auto RotateRight() -> void;
  auto RotateUp() -> void;
  auto RotateDown() -> void;

  auto SetAngularSpeed(float degreesPerMovement) -> void;
  auto GetAngularSpeed() const -> float;

private:
  /// Center of orbit
  glm::vec3 center;
  /// Radius of orbiting
  float radius;

  /// Current angle along horizontal plane of sphere
  float horizontalAngle;

  /// Current angle along vertical plane of sphere
  float verticalAngle;

  /// Speed of rotation
  float angularSpeed;
};

}

#endif
