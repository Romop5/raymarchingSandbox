#include <iostream>

#include <GLFW/glfw3.h>
#include <imgui.h>

#include "application/IApplication.hpp"
#include "application/SandboxApplication.hpp"
#include "application/TestApplication.hpp"
#include "helpers/Args.hpp"

std::unique_ptr<raymarcher::IApplication> g_application;

constexpr auto defaultWindowWidth = 1600;
constexpr auto defaultWindowHeight = 900;

raymarcher::IApplication&
raymarcher::IApplication::GetApplication()
{
  return *g_application;
}

static void
error_callback(int error, const char* description)
{
  fputs(description, stderr);
}

static void
character_callback(GLFWwindow* window, unsigned int c)
{
  g_application->CharacterPressed(window, c);
}

static void
scroll_callback(GLFWwindow* window, double xpos, double ypos)
{
  static double lastXpos = 0.0, lastYpos = 0.0;
  double relativeX = xpos - lastXpos;
  double relativeY = ypos - lastYpos;
  lastXpos = xpos;
  lastYpos = ypos;

  std::cout << "Scroll: " << xpos << " - " << ypos << std::endl;
  g_application->ScrollChanged(window, relativeX, relativeY);
}

static void
mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
  g_application->MouseButtonPressed(window, button, action);
}

static void
cursor_callback(GLFWwindow* window, double xpos, double ypos)
{
  std::cout << "Cursor: " << xpos << " - " << ypos << std::endl;
  g_application->MouseCursorChanged(window, xpos, ypos);
}

static void
key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  // if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
  //     glfwSetWindowShouldClose(window, GL_TRUE);

  g_application->KeyPressed(window, key, action, mods);
}

void
framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  glViewport(0, 0, width, height);
  g_application->Resize(width, height);
}

int
main(int argc, const char* argv[])
{
  raymarcher::Arguments args;

  //              Name                --opt           -o
  args.AddLongOpt(
    "shouldRunTest", "test", "t", raymarcher::Arguments::paramLogic::NO_PARAM);
  args.AddLongOpt(
    "useFlyCam", "flycam", "f", raymarcher::Arguments::paramLogic::NO_PARAM);
  args.AddLongOpt("multisample",
                  "multisample",
                  "m",
                  raymarcher::Arguments::paramLogic::NO_PARAM);
  args.AddLongOpt("sdfFile", "input", "i");
  args.AddLongOpt("cameraSequenceID", "seq", "s");

  args.Parse(argc, argv);

  GLFWwindow* window;
  glfwSetErrorCallback(error_callback);
  if (!glfwInit())
    exit(EXIT_FAILURE);

  if (args.HasArgument("multisample")) {
    glfwWindowHint(GLFW_SAMPLES, 4);
  }
  window = glfwCreateWindow(
    defaultWindowWidth, defaultWindowHeight, "Raymarching Sandbox", NULL, NULL);
  if (!window) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  glfwMakeContextCurrent(window);

  if (args.HasArgument("shouldRunTest")) {
    raymarcher::TestApplication::StartParameters params;

    auto id = (args.HasArgument("cameraSequenceID")
                 ? std::stoi(args["cameraSequenceID"])
                 : params.cameraSequenceID);

    params.shouldRunWithFreeMovement = args.HasArgument("useFlyCam");
    params.filename = args.Value_or("sdfFile", params.filename);
    params.cameraSequenceID = id;
    g_application = std::make_unique<raymarcher::TestApplication>(params);
  } else {
    g_application = std::make_unique<raymarcher::SandboxApplication>();
  }

  g_application->Resize(defaultWindowWidth, defaultWindowHeight);

  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  glfwSetCharCallback(window, character_callback);
  glfwSetKeyCallback(window, key_callback);
  glfwSetCursorPosCallback(window, cursor_callback);
  glfwSetScrollCallback(window, scroll_callback);
  glfwSetMouseButtonCallback(window, mouse_button_callback);

  glEnable(GL_MULTISAMPLE);
  while (!glfwWindowShouldClose(window) && g_application->ShouldContinue()) {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    g_application->Render();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  glfwDestroyWindow(window);
  glfwTerminate();
  exit(EXIT_SUCCESS);
}
