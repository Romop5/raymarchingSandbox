#include "TestApplication.hpp"

#include <imgui.h>

#include "SDF.hpp"
#include "WindowWidget.hpp"
#include "MenuWidget.hpp"
#include "FlyingCamera.hpp"
#include "InterpolatedCamera.hpp"

using namespace raymarcher;

namespace {
    auto CreateTestApp() -> std::pair<std::shared_ptr<Raymarcher>, std::shared_ptr<GLFWCamera>>
    {
        auto code= R"(
vec4 df(vec3 pos)
{
    bool isFarFromOrigin = (distance(camera_origin, pos) > 20.0);

    vec4 gd = ground(pos, floorElevation);


    //pos.x = mod(pos.x+3.0, 6.0)-3.0;
    //pos.y = mod(pos.y+3.0, 6.0)-3.0;
    //pos.z = mod(pos.z+3.0, 6.0)-3.0;

    float boundingVol = sphere(pos, 3.0); 

    const float minDist = 3;
    if(boundingVol > minDist)
    {
        return unite(gd, vec4(boundingVol, vec3(0.0)));
    }

    //if(isFarFromOrigin)
    //{
    //   return unite(gd, vec4(sphere(pos, 1.0), vec3(0.0)));
    //}

    vec4 s01 = vec4(sphere(pos+vec3(0.5,0.1,0.0), 0.5), vec3(1.0));
    vec4 s02 = vec4(sphere(pos+vec3(-0.5,0.5,0.0), 0.2), vec3(1.0));
    vec4 s03 = vec4(sphere(pos+vec3(0.5,0.9,0.0), 0.3), vec3(1.0));
    vec4 s04 = vec4(sphere(pos+vec3(0.5,0.9,0.0), 0.3), vec3(1.0));
    vec4 s05 = vec4(sphere(pos+vec3(0.5,0.9,0.0), 0.1), vec3(1.0));
    vec4 s06 = vec4(sphere(pos+vec3(0.5,0.9,0.0), 0.5), vec3(1.0));
    vec4 s07 = vec4(sphere(pos+vec3(0.5,0.4,0.0), 0.2), vec3(1.0));
    vec4 s08 = vec4(sphere(pos+vec3(0.5,0.9,0.0), 0.1), vec3(1.0));
    vec4 s09 = vec4(sphere(pos+vec3(0.5,0.3,0.0), 0.6), vec3(1.0));
    vec4 s10 = vec4(sphere(pos+vec3(0.5,0.1,0.0), 0.2), vec3(1.0));
    vec4 s11 = vec4(sphere(pos+vec3(0.5,0.9,0.0), 0.2), vec3(1.0));
    vec4 s12 = vec4(sphere(pos+vec3(0.1,0.9,0.0), 0.2), vec3(1.0));
    vec4 s13 = vec4(sphere(pos+vec3(0.5,0.3,0.0), 0.2), vec3(1.0));
    vec4 s14 = vec4(sphere(pos+vec3(0.1,0.5,0.0), 0.9), vec3(1.0));
    vec4 s15 = vec4(sphere(pos+vec3(0.5,0.3,0.0), 0.5), vec3(1.0));
    vec4 s16 = vec4(sphere(pos+vec3(0.5,0.9,0.0), 0.3), vec3(1.0));
    vec4 s17 = vec4(sphere(pos+vec3(0.5,0.9,0.0), 0.2), vec3(1.0));
    vec4 uA = unite(s01, s02);
    vec4 uB = unite(s02, s03);
    vec4 uC = unite(s04, s05);
    vec4 uD = unite(s05, s06);
    vec4 uE = unite(s06, s07);
    vec4 uF = unite(s08, s09);
    vec4 uG = unite(s10, s11);
    vec4 uH = unite(s12, s13);
    vec4 uI = unite(s14, s15);
    vec4 uK = unite(s16, s17);
    vec4 u2A = unite(uA, uB);
    vec4 u2B = unite(uC, uD);
    vec4 u2C = unite(uE, uF);
    vec4 u2D = unite(uG, uH);
    vec4 u2E = unite(uI, uK);
    vec4 u3A = unite(u2A, u2B);
    vec4 u3B = unite(u2C, u2D);
    vec4 u3C = unite(u2D, u2E);
    vec4 u4A = unite(u3A, u3B);
    vec4 u4B = unite(u3B, u3C);
    vec4 u5A = unite(u4A, u3B);
    return unite(gd, u5A);
    //return unite(gd,object(pos));
    //return unite(gd,vec4(sphere(pos, 1.0), vec3(1.0,0.0,0.0)));
}
        )";


        auto rm = std::make_shared<raymarcher::Raymarcher>();
        auto orbiter = std::make_shared<raymarcher::OrbitCamera>();
        orbiter->SetCenter(glm::vec3(0.0, 3.0,0.0));
        orbiter->SetDistance(5.0);
        auto flycam = std::make_shared<raymarcher::FlyingCamera>();

        auto cameraPoints = std::vector<raymarcher::InterpolatedCamera::CameraPoint>
        {
            {  0, glm::vec3(-10.0,0.1,0.0), glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0,1.0,0.0) },
            { 50, glm::vec3( -5.0,0.0,0.2), glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0,1.0,0.0) },
            {100, glm::vec3( -2.0,0.1,0.0), glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0,1.0,0.0) },
            {200, glm::vec3( -2.0,20.0,0.0), glm::vec3(0.0, -1.0, 0.0), glm::vec3(1.0,0.0,0.0) },
            {250, glm::vec3( -20.0,1.0,0.0), glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0,1.0,0.0) },
            {300, glm::vec3( -10.0,0.1,-5.0), glm::vec3(0.0, 0.0, 1.0), glm::vec3(1.0,0.0,0.0) },
            {350, glm::vec3(  0.0,0.1,-3.0), glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0,1.0,0.0) },
        };
        auto interpolatedCamera = std::make_shared<raymarcher::InterpolatedCamera>(cameraPoints);
        //auto focusedCamera = std::make_shared<raymarcher::GLFWCamera>(orbiter, raymarcher::GLFWCamera::CameraType::ORBITER_CAMERA);
        auto focusedCamera = std::make_shared<raymarcher::GLFWCamera>(interpolatedCamera, raymarcher::GLFWCamera::CameraType::INTERPOLATED_CAMERA);
        rm->SetCamera(focusedCamera);
        //rm->SetCamera(orbiter);
        auto sdf = std::make_shared<raymarcher::SDF>(code);
        rm->SetSDF(sdf);

        return std::pair{rm, focusedCamera};;
    }
}
 

TestApplication::TestApplication()
{    
    ge::gl::init();

    adapter.Initialize(500,500);
    adapter.SetVisibility(true);

    auto [rm, cam] = CreateTestApp();
    raymarcher = rm;
    camera = cam;
    inputHandler = cam;
}

auto TestApplication::Resize(size_t newWidth, size_t newHeight) -> void
{
    adapter.Resize(newWidth, newHeight);
}

auto TestApplication::Render() -> void
{
    static size_t frameID = 0;
    frameID++;
    if(frameID > 350)
    {
        abort();
    }

    camera->UpdateFrame();
    fpsMeter.Measure();

    if(raymarcher)
    {
        raymarcher->Render();
    }

    adapter.BeginFrame();
    //ImGui::ShowDemoWindow(nullptr);
    //ImGui::ShowMetricsWindow(nullptr);
    fpsMeter.RenderOverlay();
    
    adapter.EndFrame();
    adapter.RenderCurrentFrame();
}

auto TestApplication::CharacterPressed(GLFWwindow* window, unsigned int character) -> void
{
    adapter.OnCharacter(character);
}

auto TestApplication::MouseCursorChanged(GLFWwindow* window, double absoluteX, double absoluteY) -> void
{
    static double lastXpos = 0.0, lastYpos = 0.0;
    double relativeX = absoluteX-lastXpos;
    double relativeY = absoluteY-lastYpos;
    lastXpos = absoluteX;
    lastYpos = absoluteY;

    if(inputHandler)
    {
        inputHandler->MouseCursorChanged(window, relativeX, relativeY);
        return;
    }

    adapter.OnMousePosition(absoluteX, absoluteY);
    if(adapter.IsVisible())
    {
        return;
    }
}

auto TestApplication::MouseButtonPressed(GLFWwindow* window, int button, int action) -> void 
{
    if(adapter.WantCaptureMouse())
    {
        adapter.OnButton(button, action == GLFW_PRESS);
    }
}

auto TestApplication::ScrollChanged(GLFWwindow* window, double relativeX, double relativeY) -> void
{
    if(inputHandler)
    {
        inputHandler->ScrollChanged(window, relativeX, relativeY);
        return;
    }
}

auto TestApplication::KeyPressed(GLFWwindow* window, int key, int action) -> void
{
    if(key == GLFW_KEY_F11 && action == GLFW_PRESS)
    {
        adapter.SetVisibility(!adapter.IsVisible());
    }

    if(inputHandler)
    {
        inputHandler->KeyPressed(window, key);
        return;
    }

    if(adapter.WantCaptureKeyboard())
    {
        adapter.OnKey(key, action == GLFW_PRESS);
        return;
    }
    
}

auto TestApplication::SetFocus(std::shared_ptr<IGLFWInputHandler> handler) -> void
{
    inputHandler = std::move(handler);
}
