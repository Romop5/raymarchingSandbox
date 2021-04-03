#include "GLFWCamera.hpp"
#include <GLFW/glfw3.h>
#include "FlyingCamera.hpp"
#include "OrbitCamera.hpp"

using namespace raymarcher;

raymarcher::GLFWCamera::GLFWCamera(std::shared_ptr<ICamera> cameraPtr, CameraType cameraType) :
    type { cameraType },
    camera { cameraPtr }
{
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
        flyingCamera.SetAngularSpeed(0.01);
        if(relativeX> 0.0)
            flyingCamera.RotateLeft();
        if(relativeX< 0.0)
            flyingCamera.RotateRight();

        
        if(relativeY> 0.0)
            flyingCamera.RotateDown();
        if(relativeY< 0.0)
            flyingCamera.RotateUp();
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
            case GLFW_KEY_UP:
                flyingCamera.MoveUp();
                break;
            case GLFW_KEY_DOWN:
                flyingCamera.MoveDown();
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
                orbiter.SetDistance(orbiter.GetDistance()-0.1);
                break;
            case GLFW_KEY_S:
                orbiter.SetDistance(orbiter.GetDistance()+0.1);
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
