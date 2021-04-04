#include <iostream>
#include <GLFW/glfw3.h>
#include <imgui.h>

#include "Raymarcher.hpp"
#include "FlyingCamera.hpp"
#include "OrbitCamera.hpp"
#include "GLFWCamera.hpp"
#include "SDF.hpp"
#include "ImguiAdapter.hpp"


constexpr auto defaultWindowWidth = 600;
constexpr auto defaultWindowHeight = 400;

raymarcher::ImguiAdapter adapter;
std::shared_ptr<raymarcher::GLFWCamera> g_sceneCamera;

static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}
static void scroll_callback(GLFWwindow* window, double xpos, double ypos)
{
    static double lastXpos = 0.0, lastYpos = 0.0;
    double relativeX = xpos-lastXpos;
    double relativeY = ypos-lastYpos;
    lastXpos = xpos;
    lastYpos = ypos;

    std::cout << "Scroll: " << xpos << " - " << ypos << std::endl;
    g_sceneCamera->ScrollChanged(window, xpos, ypos);
}

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if(adapter.WantCaptureMouse())
    {
        adapter.OnButton(button, action == GLFW_PRESS);
    }
}

static void cursor_callback(GLFWwindow* window, double xpos, double ypos)
{
    static double lastXpos = 0.0, lastYpos = 0.0;
    double relativeX = xpos-lastXpos;
    double relativeY = ypos-lastYpos;
    lastXpos = xpos;
    lastYpos = ypos;

    std::cout << "Cursor: " << xpos << " - " << ypos << std::endl;
    adapter.OnMousePosition(xpos, ypos);
    if(adapter.IsVisible())
    {
        return;
    }

    g_sceneCamera->MouseCursorChanged(window, relativeX, relativeY);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (key == GLFW_KEY_F11 && action == GLFW_PRESS)
    {
        adapter.SetVisibility(!adapter.IsVisible());
        //glfwSetInputMode(window, GLFW_CURSOR, adapter.IsVisible()?GLFW_CURSOR_NORMAL:GLFW_CURSOR_DISABLED);
    }

    if(adapter.WantCaptureKeyboard())
    {
        adapter.OnKey(key, action == GLFW_PRESS);
        return;
    }
    g_sceneCamera->KeyPressed(window, key);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    adapter.Resize(width, height);
}

int main(void)
{
    GLFWwindow* window;
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);
    window = glfwCreateWindow(defaultWindowWidth, defaultWindowHeight, "Simple example", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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


    raymarcher::Raymarcher rm;
    auto flyingCamera = std::make_shared<raymarcher::FlyingCamera>();
    auto orbiter = std::make_shared<raymarcher::OrbitCamera>();
    orbiter->SetCenter(glm::vec3(0.0, 3.0,0.0));
    orbiter->SetDistance(5.0);
    /* g_sceneCamera = std::make_shared<raymarcher::GLFWCamera>(flyingCamera, raymarcher::GLFWCamera::CameraType::FLYING_CAMERA); */
    g_sceneCamera = std::make_shared<raymarcher::GLFWCamera>(orbiter, raymarcher::GLFWCamera::CameraType::ORBITER_CAMERA);
    rm.SetCamera(g_sceneCamera);
    auto sdf = std::make_shared<raymarcher::SDF>(sdfLiteral);
    rm.SetSDF(sdf);


    adapter.Initialize(defaultWindowWidth, defaultWindowHeight);

    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, cursor_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        rm.Render();

        if(adapter.IsVisible())
        {
            adapter.BeginFrame();
            ImGui::ShowDemoWindow(nullptr);
            adapter.EndFrame();
            adapter.RenderCurrentFrame();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
