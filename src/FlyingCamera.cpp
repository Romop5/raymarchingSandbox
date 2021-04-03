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
    auto rotation = glm::mat4(glm::mat3(glm::cross(normalizedUpVector, normalizedLookDirection),
                                        normalizedUpVector,
                                        normalizedLookDirection));

    return glm::translate(-position)*rotation;
}

auto FlyingCamera::MoveForward()
{
    position += normalizedLookDirection * movementSpeed;
}

auto FlyingCamera::MoveBackward()
{
    position -= normalizedLookDirection * movementSpeed;
}

auto FlyingCamera::MoveLeft()
{
    auto leftDirection = glm::cross(normalizedLookDirection, normalizedUpVector);
    position += leftDirection * movementSpeed;
}

auto FlyingCamera::MoveRight()
{
    auto leftDirection = glm::cross(normalizedLookDirection, normalizedUpVector);
    position -= leftDirection * movementSpeed;
}

auto FlyingCamera::MoveUp()
{
    position += normalizedUpVector * movementSpeed;
}

auto FlyingCamera::MoveDown()
{
    position -= normalizedUpVector * movementSpeed;
}

auto FlyingCamera::RotateLeft()
{
    normalizedLookDirection = glm::vec3(glm::vec4(normalizedLookDirection, 0.0) * 
                              glm::rotate(angularSpeed, normalizedUpVector));
}

auto FlyingCamera::RotateRight()
{
    normalizedLookDirection = glm::vec3(glm::vec4(normalizedLookDirection, 0.0) * 
                              glm::rotate(-angularSpeed, normalizedUpVector));
}

auto FlyingCamera::RotateUp()
{
    auto orthoDirection = glm::cross(normalizedUpVector, normalizedLookDirection);
    normalizedUpVector= glm::vec3(glm::vec4(normalizedUpVector, 0.0) * 
                        glm::rotate(angularSpeed, orthoDirection));
}

auto FlyingCamera::RotateDown()
{
    auto orthoDirection = glm::cross(normalizedUpVector, normalizedLookDirection);
    normalizedUpVector= glm::vec3(glm::vec4(normalizedUpVector, 0.0) * 
                        glm::rotate(-angularSpeed, orthoDirection));
}

auto FlyingCamera::SetMovementSpeed(float unitsPerMovement)
{
    movementSpeed = unitsPerMovement;
}

auto FlyingCamera::SetMovementSpeed() const -> float
{
    return movementSpeed;
}

auto FlyingCamera::SetAngularSpeed(float degreesPerMovement)
{
    angularSpeed = degreesPerMovement;
}
auto FlyingCamera::SetAngularSpeed() const -> float
{
    return angularSpeed;
}
