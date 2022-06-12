#include <iostream>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/transform.hpp>

#include "rendering/FlyingCamera.hpp"

using namespace raymarcher;

const auto maximumVerticalAngle = 0.7;

FlyingCamera::FlyingCamera()
  : position{ glm::vec3(0.0, 5.0, 0.0) }
  , normalizedLookDirection{ glm::vec3(0, 0, 1.0) }
  , normalizedUpVector{ glm::vec3(0, 1.0, 0) }
  , movementSpeed{ 1.0 }
  , angularSpeed{ 0.01 }
{}

auto
FlyingCamera::SetPosition(const glm::vec3& pos) -> void
{
  this->position = pos;
}

auto
FlyingCamera::GetTransformation() const -> const glm::mat4&
{
  // Calculate transformation using look vector and look-up vector.
  // maximumVerticalAngle is used in code below to ensure that this
  // two vectors are not collinear
  auto orthoSide =
    glm::normalize(glm::cross(normalizedUpVector, normalizedLookDirection));
  auto orthoUp =
    glm::normalize(glm::cross(-orthoSide, normalizedLookDirection));
  auto rotation =
    glm::mat4(glm::mat3(orthoSide, orthoUp, normalizedLookDirection));

  transform = glm::translate(-position) * rotation;
  return transform;
}

auto
FlyingCamera::MoveForward() -> void
{
  position += normalizedLookDirection * movementSpeed;
}

auto
FlyingCamera::MoveBackward() -> void
{
  position -= normalizedLookDirection * movementSpeed;
}

auto
FlyingCamera::MoveLeft() -> void
{
  auto leftDirection = glm::cross(normalizedLookDirection, normalizedUpVector);
  position += leftDirection * movementSpeed;
}

auto
FlyingCamera::MoveRight() -> void
{
  auto leftDirection = glm::cross(normalizedLookDirection, normalizedUpVector);
  position -= leftDirection * movementSpeed;
}

auto
FlyingCamera::MoveUp() -> void
{
  position += normalizedUpVector * movementSpeed;
}

auto
FlyingCamera::MoveDown() -> void
{
  position -= normalizedUpVector * movementSpeed;
}

auto
FlyingCamera::RotateLeft(float amount) -> void
{
  auto orthogonalLookUp =
    glm::cross(glm::cross(normalizedUpVector, normalizedLookDirection),
               normalizedLookDirection);

  normalizedLookDirection = glm::normalize(
    glm::vec3(glm::vec4(normalizedLookDirection, 0.0) *
              glm::rotate(angularSpeed * amount, orthogonalLookUp)));
}

auto
FlyingCamera::RotateRight(float amount) -> void
{
  auto orthogonalLookUp =
    glm::cross(glm::cross(normalizedUpVector, normalizedLookDirection),
               normalizedLookDirection);
  normalizedLookDirection = glm::normalize(
    glm::vec3(glm::vec4(normalizedLookDirection, 0.0) *
              glm::rotate(-angularSpeed * amount, orthogonalLookUp)));
}

auto
FlyingCamera::RotateUp(float amount) -> void
{
  if (glm::dot(normalizedLookDirection, normalizedUpVector) >
      maximumVerticalAngle)
    return;
  auto orthoDirection =
    glm::normalize(glm::cross(normalizedUpVector, normalizedLookDirection));
  normalizedLookDirection = glm::normalize(
    glm::vec3(glm::vec4(normalizedLookDirection, 0.0) *
              glm::rotate(angularSpeed * amount, orthoDirection)));
}

auto
FlyingCamera::RotateDown(float amount) -> void
{
  if (glm::dot(normalizedLookDirection, normalizedUpVector) <
      -maximumVerticalAngle)
    return;
  auto orthoDirection =
    glm::normalize(glm::cross(normalizedUpVector, normalizedLookDirection));
  normalizedLookDirection = glm::normalize(
    glm::vec3(glm::vec4(normalizedLookDirection, 0.0) *
              glm::rotate(-angularSpeed * amount, orthoDirection)));
}

auto
FlyingCamera::SetMovementSpeed(float unitsPerMovement) -> void
{
  movementSpeed = unitsPerMovement;
}

auto
FlyingCamera::GetMovementSpeed() const -> float
{
  return movementSpeed;
}

auto
FlyingCamera::SetAngularSpeed(float degreesPerMovement) -> void
{
  angularSpeed = degreesPerMovement;
}
auto
FlyingCamera::GetAngularSpeed() const -> float
{
  return angularSpeed;
}
