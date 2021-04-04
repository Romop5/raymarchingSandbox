#include "SandboxApplication.hpp"
#include <imgui.h>

using namespace raymarcher;

SandboxApplication::SandboxApplication()
{    
    ge::gl::init();
    auto sdfLiteral = R"(
    float df(vec3 pos)
    {
        float gd = ground(pos, floorElevation);
        //float a = sphere(pos, 1.0);
        //float ab = sphere(pos+vec3(0.5+sin(iTime),0.0,sin(iTime)*-0.0), 1.0);
        
        //float d = unite(a,b);
        return unite(gd,object(pos));
        //return uniteSmooth(ab,a);
    }
    )";


    auto rm = std::make_shared<raymarcher::Raymarcher>();
    auto orbiter = std::make_shared<raymarcher::OrbitCamera>();
    orbiter->SetCenter(glm::vec3(0.0, 3.0,0.0));
    orbiter->SetDistance(5.0);
    /* globalCamera = std::make_shared<raymarcher::GLFWCamera>(flyingCamera, raymarcher::GLFWCamera::CameraType::FLYING_CAMERA); */
    globalCamera = std::make_shared<raymarcher::GLFWCamera>(orbiter, raymarcher::GLFWCamera::CameraType::ORBITER_CAMERA);
    rm->SetCamera(globalCamera);
    auto sdf = std::make_shared<raymarcher::SDF>(sdfLiteral);
    rm->SetSDF(sdf);


    adapter.Initialize(500,500);

    auto widget = std::make_shared<raymarcher::RendererWidget>(rm);
    widget->SetViewportSize(300,300);


    /* auto widget2 = std::make_shared<raymarcher::RendererWidget>(rm); */
    /* widget2->SetViewportSize(300,100); */

    widgetManager = std::make_unique<raymarcher::WidgetManager>();
    widgetManager->AddWidget(widget);
    /* widgetManager->AddWidget(widget2); */
}

auto SandboxApplication::Resize(size_t newWidth, size_t newHeight) -> void
{
    adapter.Resize(newWidth, newHeight);
}

auto SandboxApplication::Render() -> void
{
    if(adapter.IsVisible())
    {
        adapter.BeginFrame();
        ImGui::ShowDemoWindow(nullptr);
        
        if(widgetManager)
        {
            widgetManager->Render();
        }
        /* if(ImGui::Begin("Widget", nullptr)) */
        /* { */
        /*     widget->Render(); */
        /*     ImGui::End(); */
        /* } */
        adapter.EndFrame();
        adapter.RenderCurrentFrame();
    }

}

auto SandboxApplication::MouseCursorChanged(GLFWwindow* window, double absoluteX, double absoluteY) -> void
{
    static double lastXpos = 0.0, lastYpos = 0.0;
    double relativeX = absoluteX-lastXpos;
    double relativeY = absoluteY-lastYpos;
    lastXpos = absoluteX;
    lastYpos = absoluteY;

    adapter.OnMousePosition(absoluteX, absoluteY);
    if(adapter.IsVisible())
    {
        return;
    }

    globalCamera->MouseCursorChanged(window, relativeX, relativeY);
}

auto SandboxApplication::MouseButtonPressed(GLFWwindow* window, int button, int action) -> void 
{
    if(adapter.WantCaptureMouse())
    {
        adapter.OnButton(button, action == GLFW_PRESS);
    }
}

auto SandboxApplication::ScrollChanged(GLFWwindow* window, double relativeX, double relativeY) -> void
{
    globalCamera->ScrollChanged(window, relativeX, relativeY);
}

auto SandboxApplication::KeyPressed(GLFWwindow* window, int key, int action) -> void
{
    if(key == GLFW_KEY_F11 && action == GLFW_PRESS)
    {
        adapter.SetVisibility(!adapter.IsVisible());
        //glfwSetInputMode(window, GLFW_CURSOR, adapter.IsVisible()?GLFW_CURSOR_NORMAL:GLFW_CURSOR_DISABLED);
    }

    if(adapter.WantCaptureKeyboard())
    {
        adapter.OnKey(key, action == GLFW_PRESS);
        return;
    }
    globalCamera->KeyPressed(window, key);
}
