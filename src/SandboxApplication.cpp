#include "SandboxApplication.hpp"

#include <imgui.h>

#include "SDF.hpp"
#include "WindowWidget.hpp"
#include "MenuWidget.hpp"

using namespace raymarcher;

SandboxApplication::SandboxApplication()
{    
    ge::gl::init();

    adapter.Initialize(500,500);
    adapter.SetVisibility(true);

    auto menuWindow = std::make_shared<WindowWidget>();
    menuWindow->AddWidget(std::make_shared<MenuWidget>(widgetManager));
    menuWindow->SetTitle("Menu");
    widgetManager.AddWidget(menuWindow);
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
        
        widgetManager.Render();
        /* if(ImGui::Begin("Widget", nullptr)) */
        /* { */
        /*     widget->Render(); */
        /*     ImGui::End(); */
        /* } */
        adapter.EndFrame();
        adapter.RenderCurrentFrame();
    }

}

auto SandboxApplication::CharacterPressed(GLFWwindow* window, unsigned int character) -> void
{
    adapter.OnCharacter(character);
}

auto SandboxApplication::MouseCursorChanged(GLFWwindow* window, double absoluteX, double absoluteY) -> void
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

auto SandboxApplication::MouseButtonPressed(GLFWwindow* window, int button, int action) -> void 
{
    if(adapter.WantCaptureMouse())
    {
        adapter.OnButton(button, action == GLFW_PRESS);
    }
}

auto SandboxApplication::ScrollChanged(GLFWwindow* window, double relativeX, double relativeY) -> void
{
    if(inputHandler)
    {
        inputHandler->ScrollChanged(window, relativeX, relativeY);
        return;
    }
}

auto SandboxApplication::KeyPressed(GLFWwindow* window, int key, int action) -> void
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        inputHandler.reset();
    }
    if(key == GLFW_KEY_F11 && action == GLFW_PRESS)
    {
        adapter.SetVisibility(!adapter.IsVisible());
        //glfwSetInputMode(window, GLFW_CURSOR, adapter.IsVisible()?GLFW_CURSOR_NORMAL:GLFW_CURSOR_DISABLED);
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

auto SandboxApplication::SetFocus(std::shared_ptr<IGLFWInputHandler> handler) -> void
{
    inputHandler = std::move(handler);
}
