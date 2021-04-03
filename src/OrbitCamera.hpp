#ifndef RAYMARCHER_ORBITCAMERA_HPP
#define RAYMARCHER_ORBITCAMERA_HPP

#include "ICamera.hpp"
namespace raymarcher 
{
/**
 * @brief Simple orbiter
 */
class OrbitCamera: public ICamera
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
    glm::vec3 center;
    float radius;
    float horizontalAngle;
    float verticalAngle;

    float angularSpeed;
};

}

#endif
