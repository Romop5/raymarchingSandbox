#ifndef RAYMARCHER_FLYINGCAMERA_HPP
#define RAYMARCHER_FLYINGCAMERA_HPP

#include "ICamera.hpp"
namespace raymarcher 
{
/**
 * @brief Represents a free-space camera
 */
class FlyingCamera: public ICamera
{
    public:
    FlyingCamera();
    auto GetTransformation() const -> const glm::mat4& override;

    auto MoveForward();
    auto MoveBackward();
    auto MoveLeft();
    auto MoveRight();
    auto MoveUp();
    auto MoveDown();

    auto RotateLeft();
    auto RotateRight();
    auto RotateUp();
    auto RotateDown();

    auto SetMovementSpeed(float unitsPerMovement);
    auto SetMovementSpeed() const -> float;

    auto SetAngularSpeed(float degreesPerMovement);
    auto SetAngularSpeed() const -> float;
    private:
    glm::vec3 position;
    glm::vec3 normalizedLookDirection;
    glm::vec3 normalizedUpVector;

    float movementSpeed;
    float angularSpeed;
};

}

#endif
