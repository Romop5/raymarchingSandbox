#ifndef RAYMARCHER_FLYINGCAMERA_HPP
#define RAYMARCHER_FLYINGCAMERA_HPP

#include "rendering/ICamera.hpp"
namespace raymarcher 
{
/**
 * @brief Represents a free-space camera
 */
class FlyingCamera: public ICamera
{
    public:
    FlyingCamera();

    auto SetPosition(const glm::vec3& pos) -> void;

    auto GetTransformation() const -> const glm::mat4& override;

    auto MoveForward() -> void;
    auto MoveBackward() -> void;
    auto MoveLeft() -> void;
    auto MoveRight() -> void;
    auto MoveUp() -> void;
    auto MoveDown() -> void;

    auto RotateLeft(float amount) -> void;
    auto RotateRight(float amount) -> void;
    auto RotateUp(float amount) -> void;
    auto RotateDown(float amount) -> void;

    auto SetMovementSpeed(float unitsPerMovement) -> void;
    auto GetMovementSpeed() const -> float;

    auto SetAngularSpeed(float degreesPerMovement) -> void;
    auto GetAngularSpeed() const -> float;
    private:
    glm::vec3 position;
    glm::vec3 normalizedLookDirection;
    glm::vec3 normalizedUpVector;

    float movementSpeed;
    float angularSpeed;

    mutable glm::mat4 transform;
};

}

#endif
