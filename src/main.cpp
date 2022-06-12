#include <iostream>

#include <GLFW/glfw3.h>
#include <imgui.h>
#include <spdlog/cfg/env.h>
#include <spdlog/spdlog.h>

#include "application/IApplication.hpp"
#include "application/SandboxApplication.hpp"
#include "application/TestApplication.hpp"
#include "helpers/Args.hpp"

std::unique_ptr<raymarcher::IApplication> g_application;

constexpr auto defaultWindowWidth = 1600;
constexpr auto defaultWindowHeight = 900;
constexpr auto defaultWindowName = "Raymarching Sandbox";
constexpr auto multisample_level = 4;

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

  spdlog::trace("scroll_callback: {} - {}", xpos, ypos);
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
  spdlog::trace("cursor_callback: {} - {}", xpos, ypos);
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
  //
  // Parse command line arguments
  //
  spdlog::cfg::load_env_levels();
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
  args.AddLongOpt("fullscreen",
                  "fullscreen",
                  "f",
                  raymarcher::Arguments::paramLogic::NO_PARAM);
  args.AddLongOpt("cameraSequenceID", "seq", "s");

  args.Parse(argc, argv);

  //
  // Create window & application
  //
  GLFWwindow* window;
  glfwSetErrorCallback(error_callback);
  if (!glfwInit())
    exit(EXIT_FAILURE);

  if (args.HasArgument("multisample")) {
    spdlog::info("main: using multisample: {}", multisample_level);
    glfwWindowHint(GLFW_SAMPLES, multisample_level);
  }

  const auto fullscreen_option =
    args.HasArgument("fullscreen") ? glfwGetPrimaryMonitor() : NULL;
  window = glfwCreateWindow(defaultWindowWidth,
                            defaultWindowHeight,
                            defaultWindowName,
                            fullscreen_option,
                            NULL);
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

  //
  // Run application
  //
  while (!glfwWindowShouldClose(window) && g_application->ShouldContinue()) {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    g_application->Render();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  //
  // Clean up
  //
  glfwDestroyWindow(window);
  glfwTerminate();
  exit(EXIT_SUCCESS);
}
