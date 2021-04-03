#include <GLFW/glfw3.h>

#include "Raymarcher.hpp"
#include "FlyingCamera.hpp"
#include "SDF.hpp"

static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

int main(void)
{
    GLFWwindow* window;
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);
    window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
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
    flyingCamera->SetMovementSpeed(10.0);
    flyingCamera->MoveBackward();
    rm.SetCamera(flyingCamera);
    auto sdf = std::make_shared<raymarcher::SDF>(sdfLiteral);
    rm.SetSDF(sdf);

    glfwSetKeyCallback(window, key_callback);
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        rm.Render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
