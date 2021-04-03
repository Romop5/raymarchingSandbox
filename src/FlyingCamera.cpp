#include "FlyingCamera.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

using namespace raymarcher;

FlyingCamera::FlyingCamera() :
    position { glm::vec3(0.0) },
    normalizedLookDirection{ glm::vec3(0, 0, 1.0) },
    normalizedUpVector { glm::vec3(0, 1.0, 0) },
    movementSpeed{ 1.0 },
    angularSpeed{ 1.0 }
{
}

auto FlyingCamera::GetTransformation() const -> const glm::mat4&
{
    static glm::mat4 transform;
    auto rotation = glm::mat4(glm::mat3(glm::cross(normalizedUpVector, normalizedLookDirection),
                                        normalizedUpVector,
                                        normalizedLookDirection));

    transform = glm::translate(-position)*rotation;
    return transform;
}

auto FlyingCamera::MoveForward() -> void
{
    position += normalizedLookDirection * movementSpeed;
}

auto FlyingCamera::MoveBackward() -> void
{
    position -= normalizedLookDirection * movementSpeed;
}

auto FlyingCamera::MoveLeft() -> void
{
    auto leftDirection = glm::cross(normalizedLookDirection, normalizedUpVector);
    position += leftDirection * movementSpeed;
}

auto FlyingCamera::MoveRight() -> void
{
    auto leftDirection = glm::cross(normalizedLookDirection, normalizedUpVector);
    position -= leftDirection * movementSpeed;
}

auto FlyingCamera::MoveUp() -> void
{
    position += normalizedUpVector * movementSpeed;
}

auto FlyingCamera::MoveDown() -> void
{
    position -= normalizedUpVector * movementSpeed;
}

auto FlyingCamera::RotateLeft() -> void
{
    normalizedLookDirection = glm::vec3(glm::vec4(normalizedLookDirection, 0.0) * 
                              glm::rotate(angularSpeed, normalizedUpVector));
}

auto FlyingCamera::RotateRight() -> void
{
    normalizedLookDirection = glm::vec3(glm::vec4(normalizedLookDirection, 0.0) * 
                              glm::rotate(-angularSpeed, normalizedUpVector));
}

auto FlyingCamera::RotateUp() -> void
{
    auto orthoDirection = glm::cross(normalizedUpVector, normalizedLookDirection);
    normalizedUpVector= glm::vec3(glm::vec4(normalizedUpVector, 0.0) * 
                        glm::rotate(angularSpeed, orthoDirection));
}

auto FlyingCamera::RotateDown() -> void
{
    auto orthoDirection = glm::cross(normalizedUpVector, normalizedLookDirection);
    normalizedUpVector= glm::vec3(glm::vec4(normalizedUpVector, 0.0) * 
                        glm::rotate(-angularSpeed, orthoDirection));
}

auto FlyingCamera::SetMovementSpeed(float unitsPerMovement) -> void
{
    movementSpeed = unitsPerMovement;
}

auto FlyingCamera::GetMovementSpeed() const -> float
{
    return movementSpeed;
}

auto FlyingCamera::SetAngularSpeed(float degreesPerMovement) -> void
{
    angularSpeed = degreesPerMovement;
}
auto FlyingCamera::GetAngularSpeed() const -> float
{
    return angularSpeed;
}
