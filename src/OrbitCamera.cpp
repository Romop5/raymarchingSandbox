#include "OrbitCamera.hpp"
#include <algorithm>

using namespace raymarcher;

OrbitCamera::OrbitCamera() :
    center { glm::vec3(0.0) },
    radius { 1.0 },
    horizontalAngle { 0.0 },
    verticalAngle { 0.0 },
    angularSpeed { 0.01 }
{
}

auto OrbitCamera::GetTransformation() const -> const glm::mat4&
{
    auto position = radius*glm::vec3(sin(horizontalAngle) * cos (verticalAngle),
                              sin(verticalAngle),
                              cos(horizontalAngle) * cos(verticalAngle));
    auto normalizedPosition = -glm::normalize(position);
    const auto lookUp = glm::vec3(0.0, 1.0, 0.0); 
    auto third = glm::normalize(glm::cross(normalizedPosition, lookUp));
    auto up = glm::cross(third, normalizedPosition);

    static auto transform = glm::mat4(1.0);
    transform = glm::mat4(glm::mat3(third, up, normalizedPosition));
    transform[3] = glm::vec4(-position, 1.0);
    return transform;
}

auto OrbitCamera::SetCenter(glm::vec3 position) -> void
{
    center = position;
}

auto OrbitCamera::SetDistance(float newRadius) -> void
{
    radius = std::max(newRadius, 0.0f);
}

auto OrbitCamera::GetDistance() const -> float
{
    return radius;
}

auto OrbitCamera::RotateLeft() -> void
{
    horizontalAngle += angularSpeed;
}

auto OrbitCamera::RotateRight() -> void
{
    horizontalAngle -= angularSpeed;
}

auto OrbitCamera::RotateUp() -> void
{
    verticalAngle += angularSpeed;
    if(verticalAngle  > 0.9)
        verticalAngle = 0.9;
}

auto OrbitCamera::RotateDown() -> void
{
    verticalAngle -= angularSpeed;
    if(verticalAngle  < -0.9)
        verticalAngle = -0.9;
}

auto OrbitCamera::SetAngularSpeed(float degreesPerMovement) -> void
{
    angularSpeed = degreesPerMovement;
}

auto OrbitCamera::GetAngularSpeed() const -> float
{
    return angularSpeed;
}


