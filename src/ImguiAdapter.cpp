#include "ImguiAdapter.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cctype>
#include <backends/imgui_impl_opengl3.h>

using namespace raymarcher;

namespace helper
{
    size_t MapGLFWKeyTo256Array(size_t xkey)
    {
        // Map ASCII to ASCII A-Z (already done by X11 lib)
        //if(GLFW_KEY_SPACE<= xkey && GLFW_KEY_ASCIITILDE >= xkey)
        //    return xkey;
        switch(xkey)
        {
            case GLFW_KEY_TAB: return 0;
            case GLFW_KEY_LEFT: return 1;
            case GLFW_KEY_RIGHT: return 2;
            case GLFW_KEY_UP: return 4;
            case GLFW_KEY_DOWN: return 5;
            case GLFW_KEY_PAGE_UP: return 6;
            case GLFW_KEY_PAGE_DOWN: return 7;
            case GLFW_KEY_HOME: return 8;
            case GLFW_KEY_END: return 9;
            case GLFW_KEY_INSERT: return 10;
            case GLFW_KEY_DELETE: return 11;
            case GLFW_KEY_SPACE: return 12;
            case GLFW_KEY_BACKSPACE: return 13;
            case GLFW_KEY_ESCAPE: return 14;
        }
        return 255;
    }
};

bool ImguiAdapter::Initialize(size_t width, size_t height)
{
    glewInit();
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); 
    io.WantCaptureMouse = true;
    io.WantCaptureKeyboard= true;
    io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
    //io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
    io.ConfigFlags = ImGuiConfigFlags_NavEnableKeyboard;

    io.KeyMap[ImGuiKey_Tab] = helper::MapGLFWKeyTo256Array(GLFW_KEY_TAB);
    io.KeyMap[ImGuiKey_LeftArrow] = helper::MapGLFWKeyTo256Array(GLFW_KEY_LEFT);
    io.KeyMap[ImGuiKey_RightArrow] = helper::MapGLFWKeyTo256Array(GLFW_KEY_RIGHT);
    io.KeyMap[ImGuiKey_UpArrow] = helper::MapGLFWKeyTo256Array(GLFW_KEY_UP);
    io.KeyMap[ImGuiKey_DownArrow] = helper::MapGLFWKeyTo256Array(GLFW_KEY_DOWN);
    io.KeyMap[ImGuiKey_PageUp] = helper::MapGLFWKeyTo256Array(GLFW_KEY_PAGE_UP);
    io.KeyMap[ImGuiKey_PageDown] = helper::MapGLFWKeyTo256Array(GLFW_KEY_PAGE_DOWN);
    io.KeyMap[ImGuiKey_Home] = helper::MapGLFWKeyTo256Array(GLFW_KEY_HOME);
    io.KeyMap[ImGuiKey_End] = helper::MapGLFWKeyTo256Array(GLFW_KEY_END);
    io.KeyMap[ImGuiKey_Insert] = helper::MapGLFWKeyTo256Array(GLFW_KEY_INSERT);
    io.KeyMap[ImGuiKey_Delete] = helper::MapGLFWKeyTo256Array(GLFW_KEY_DELETE);
    io.KeyMap[ImGuiKey_Backspace] = helper::MapGLFWKeyTo256Array(GLFW_KEY_BACKSPACE);
    io.KeyMap[ImGuiKey_Space] = helper::MapGLFWKeyTo256Array(GLFW_KEY_SPACE);
    io.KeyMap[ImGuiKey_Enter] = helper::MapGLFWKeyTo256Array(GLFW_KEY_ENTER);
    io.KeyMap[ImGuiKey_Escape] = helper::MapGLFWKeyTo256Array(GLFW_KEY_ESCAPE);
    io.KeyMap[ImGuiKey_KeyPadEnter] = helper::MapGLFWKeyTo256Array(GLFW_KEY_BACKSPACE);
    /*io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
    io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
    io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
    io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
    io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
    io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;
    */

    io.MouseDrawCursor = true;

    Resize(width, height);

    ImGui_ImplOpenGL3_Init();
    return true;
}

void ImguiAdapter::Resize(size_t newWidth, size_t newHeight)
{
    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2((float)newWidth, (float)newHeight);
}

void ImguiAdapter::BeginFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGuiIO& io = ImGui::GetIO();
    // Setup display size (every frame to accommodate for window resizing)
    // TODO
    int w = 500, h = 500;
    int display_w, display_h;
    //w = context.getCurrentViewport().getWidth();
    //h = context.getCurrentViewport().getHeight();
    //io.DisplaySize = ImVec2((float)w, (float)h);
    //io.DisplayFramebufferScale = ImVec2(1.0,1.0);
    io.FontGlobalScale = m_Scaling;

    // Setup time step
    io.DeltaTime = (float)(1.0f / 60.0f);

    ImGui::NewFrame();
}
void ImguiAdapter::EndFrame()
{
    ImGui::Render();
}

void ImguiAdapter::RenderCurrentFrame()
{
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImguiAdapter::Destroy()
{
    ImGui_ImplOpenGL3_Shutdown();
}

void ImguiAdapter::OnCharacter(size_t character)
{
    ImGuiIO& io = ImGui::GetIO();
    io.AddInputCharacter(character);
}

void ImguiAdapter::OnKey(size_t key, bool isDown)
{
    ImGuiIO& io = ImGui::GetIO();
    io.KeysDown[helper::MapGLFWKeyTo256Array(key)] = isDown;
    /*if(isDown && std::isalnum(key))
    {
        io.AddInputCharacter(key);
    }*/
}

void ImguiAdapter::OnMousePositionRelative(float deltaX, float deltaY)
{
    posX += deltaX;
    posY += deltaY;
    ImGuiIO& io = ImGui::GetIO();
    io.MousePos = ImVec2(posX,posY);
}

void ImguiAdapter::OnMousePosition(float x, float y)
{
    ImGuiIO& io = ImGui::GetIO();
    io.MousePos = ImVec2(x, y);
}

void ImguiAdapter::OnButton(size_t buttonID, bool isPressed)
{
    assert(buttonID < 5);
    ImGuiIO& io = ImGui::GetIO();
    if(buttonID == 3)
    {
        io.MouseWheel = 1.0;
    } else if(buttonID == 4)
    {
        io.MouseWheel = -1.0;
    } else 
    {
        io.MouseDown[buttonID] = isPressed;
    }
}

bool ImguiAdapter::IsVisible()
{
    return m_Visibility;
}
void ImguiAdapter::SetVisibility(bool isVisible)
{
    m_Visibility = isVisible;
}

void ImguiAdapter::SetScaling(float scale)
{
    m_Scaling = scale;
}

bool ImguiAdapter::WantCaptureMouse() const
{
    ImGuiIO& io = ImGui::GetIO(); 
    return (m_Visibility && io.WantCaptureMouse);
}
bool ImguiAdapter::WantCaptureKeyboard() const
{
    ImGuiIO& io = ImGui::GetIO(); 
    return (m_Visibility && io.WantCaptureKeyboard);
}
