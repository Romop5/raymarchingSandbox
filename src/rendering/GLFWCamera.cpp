#include "rendering/GLFWCamera.hpp"
#include <GLFW/glfw3.h>
#include "rendering/FlyingCamera.hpp"
#include "rendering/OrbitCamera.hpp"
#include "rendering/InterpolatedCamera.hpp"

using namespace raymarcher;

constexpr auto angleSpeedIncrement = 10.0;
constexpr auto orbiterDistanceIncrement = 0.1;

raymarcher::GLFWCamera::GLFWCamera(std::shared_ptr<ICamera> cameraPtr, CameraType cameraType) :
    type { cameraType },
    camera { cameraPtr }
{
}

auto GLFWCamera::GetType() const -> CameraType
{
    return type;
}


auto GLFWCamera::UpdateFrame() -> void
{
    if(type == INTERPOLATED_CAMERA)
    {
        auto& interpolatedCamera = static_cast<InterpolatedCamera&>(*camera);
        static size_t frameID = 0;
        interpolatedCamera.UpdateFrame(frameID++);
    }
}

auto GLFWCamera::GetTransformation() const -> const glm::mat4& 
{
    return camera->GetTransformation();
}

auto GLFWCamera::MouseCursorChanged(GLFWwindow* window, double relativeX, double relativeY) const -> void 
{
    if(type == FLYING_CAMERA)
    {
        auto& flyingCamera = static_cast<FlyingCamera&>(*camera);
        if(relativeX> 0.0)
            flyingCamera.RotateLeft(relativeX);
        if(relativeX< 0.0)
            flyingCamera.RotateRight(abs(relativeX));

        
        if(relativeY> 0.0)
            flyingCamera.RotateDown(relativeY);
        if(relativeY< 0.0)
            flyingCamera.RotateUp(abs(relativeY));
    }

    if(type == ORBITER_CAMERA)
    {
        auto& orbiter = static_cast<OrbitCamera&>(*camera);
        if(relativeX> 0.0)
            orbiter.RotateLeft();
        if(relativeX< 0.0)
            orbiter.RotateRight();
        
        if(relativeY> 0.0)
            orbiter.RotateDown();
        if(relativeY< 0.0)
            orbiter.RotateUp();
    }
}

auto GLFWCamera::ScrollChanged(GLFWwindow* window, double relativeX, double relativeY) const -> void
{
    if (type == ORBITER_CAMERA)
    {
        auto& orbiter = static_cast<OrbitCamera&>(*camera);
        orbiter.SetDistance(orbiter.GetDistance() - relativeY);
    }
}

auto GLFWCamera::KeyPressed(GLFWwindow* window, int key) const -> void 
{
    if(type == FLYING_CAMERA)
    {
        auto& flyingCamera = static_cast<FlyingCamera&>(*camera);
        const auto speed = flyingCamera.GetAngularSpeed();
        switch(key)
        {
            case GLFW_KEY_W:
                flyingCamera.MoveForward();
                break;
            case GLFW_KEY_S:
                flyingCamera.MoveBackward();
                break;
            case GLFW_KEY_A:
                flyingCamera.MoveLeft();
                break;
            case GLFW_KEY_D:
                flyingCamera.MoveRight();
                break;
            case GLFW_KEY_PAGE_UP:
                flyingCamera.MoveUp();
                break;
            case GLFW_KEY_PAGE_DOWN:
                flyingCamera.MoveDown();
                break;
            case GLFW_KEY_UP:
                flyingCamera.RotateUp(speed);
                break;
            case GLFW_KEY_DOWN:
                flyingCamera.RotateDown(speed);
                break;
            case GLFW_KEY_LEFT:
                flyingCamera.RotateRight(speed);
                break;
            case GLFW_KEY_RIGHT:
                flyingCamera.RotateLeft(speed);
                break;
            case GLFW_KEY_KP_ADD:
                flyingCamera.SetAngularSpeed(flyingCamera.GetAngularSpeed()*angleSpeedIncrement);
                break;
            case GLFW_KEY_KP_SUBTRACT:
                flyingCamera.SetAngularSpeed(flyingCamera.GetAngularSpeed()*(1.0f/angleSpeedIncrement));
                break;
            default:
                break;
        }
    } else if (type == ORBITER_CAMERA)
    {
        auto& orbiter = static_cast<OrbitCamera&>(*camera);
        switch(key)
        {
            case GLFW_KEY_W:
                orbiter.SetDistance(orbiter.GetDistance()-orbiterDistanceIncrement);
                break;
            case GLFW_KEY_S:
                orbiter.SetDistance(orbiter.GetDistance()+orbiterDistanceIncrement);
                break;
            case GLFW_KEY_A:
                orbiter.RotateLeft();
                break;
            case GLFW_KEY_D:
                orbiter.RotateRight();
                break;
            case GLFW_KEY_UP:
                orbiter.RotateUp();
                break;
            case GLFW_KEY_DOWN:
                orbiter.RotateDown();
                break;
            default:
                break;
        }

    }

}
