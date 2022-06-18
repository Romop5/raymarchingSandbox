
#include <cctype>

#include "IconsFontAwesome5.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <bindings/imgui_impl_opengl3.h>
#include <spdlog/spdlog.h>

#include "ImguiAdapter.hpp"
using namespace raymarcher;

const auto INTERNAL_KEY_CTRL_A = 255;
const auto INTERNAL_KEY_CTRL_C = 254;
const auto INTERNAL_KEY_CTRL_V = 253;
const auto INTERNAL_KEY_CTRL_X = 252;
const auto INTERNAL_KEY_CTRL_Y = 251;
const auto INTERNAL_KEY_CTRL_Z = 250;

namespace helper {
size_t
MapGLFWKeyTo256Array(size_t xkey)
{
  // Map ASCII to ASCII A-Z (already done by X11 lib)
  // if(GLFW_KEY_SPACE<= xkey && GLFW_KEY_ASCIITILDE >= xkey)
  //    return xkey;
  switch (xkey) {
    case GLFW_KEY_TAB:
      return 1;
    case GLFW_KEY_LEFT:
      return 2;
    case GLFW_KEY_RIGHT:
      return 3;
    case GLFW_KEY_UP:
      return 4;
    case GLFW_KEY_DOWN:
      return 5;
    case GLFW_KEY_PAGE_UP:
      return 6;
    case GLFW_KEY_PAGE_DOWN:
      return 7;
    case GLFW_KEY_HOME:
      return 8;
    case GLFW_KEY_END:
      return 9;
    case GLFW_KEY_INSERT:
      return 10;
    case GLFW_KEY_DELETE:
      return 11;
    case GLFW_KEY_SPACE:
      return 12;
    case GLFW_KEY_BACKSPACE:
      return 13;
    case GLFW_KEY_ESCAPE:
      return 14;
    case GLFW_KEY_ENTER:
      return 15;
  }
  return 256;
}
};

bool
ImguiAdapter::Initialize(size_t width, size_t height)
{
  glewInit();
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  io.WantCaptureMouse = true;
  io.WantCaptureKeyboard = true;
  io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
  // io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
  io.ConfigFlags = ImGuiConfigFlags_NavEnableKeyboard;

  io.KeyMap[ImGuiKey_Tab] = helper::MapGLFWKeyTo256Array(GLFW_KEY_TAB);
  io.KeyMap[ImGuiKey_LeftArrow] = helper::MapGLFWKeyTo256Array(GLFW_KEY_LEFT);
  io.KeyMap[ImGuiKey_RightArrow] = helper::MapGLFWKeyTo256Array(GLFW_KEY_RIGHT);
  io.KeyMap[ImGuiKey_UpArrow] = helper::MapGLFWKeyTo256Array(GLFW_KEY_UP);
  io.KeyMap[ImGuiKey_DownArrow] = helper::MapGLFWKeyTo256Array(GLFW_KEY_DOWN);
  io.KeyMap[ImGuiKey_PageUp] = helper::MapGLFWKeyTo256Array(GLFW_KEY_PAGE_UP);
  io.KeyMap[ImGuiKey_PageDown] =
    helper::MapGLFWKeyTo256Array(GLFW_KEY_PAGE_DOWN);
  io.KeyMap[ImGuiKey_Home] = helper::MapGLFWKeyTo256Array(GLFW_KEY_HOME);
  io.KeyMap[ImGuiKey_End] = helper::MapGLFWKeyTo256Array(GLFW_KEY_END);
  io.KeyMap[ImGuiKey_Insert] = helper::MapGLFWKeyTo256Array(GLFW_KEY_INSERT);
  io.KeyMap[ImGuiKey_Delete] = helper::MapGLFWKeyTo256Array(GLFW_KEY_DELETE);
  io.KeyMap[ImGuiKey_Backspace] =
    helper::MapGLFWKeyTo256Array(GLFW_KEY_BACKSPACE);
  io.KeyMap[ImGuiKey_Space] = helper::MapGLFWKeyTo256Array(GLFW_KEY_SPACE);
  io.KeyMap[ImGuiKey_Enter] = helper::MapGLFWKeyTo256Array(GLFW_KEY_ENTER);
  io.KeyMap[ImGuiKey_Escape] = helper::MapGLFWKeyTo256Array(GLFW_KEY_ESCAPE);
  io.KeyMap[ImGuiKey_KeyPadEnter] =
    helper::MapGLFWKeyTo256Array(GLFW_KEY_BACKSPACE);
  io.KeyMap[ImGuiKey_A] = INTERNAL_KEY_CTRL_A;
  io.KeyMap[ImGuiKey_C] = INTERNAL_KEY_CTRL_C;
  io.KeyMap[ImGuiKey_V] = INTERNAL_KEY_CTRL_V;
  io.KeyMap[ImGuiKey_X] = INTERNAL_KEY_CTRL_X;
  io.KeyMap[ImGuiKey_Y] = INTERNAL_KEY_CTRL_Y;
  io.KeyMap[ImGuiKey_Z] = INTERNAL_KEY_CTRL_Z;

  io.MouseDrawCursor = true;

  Resize(width, height);

  ImGui_ImplOpenGL3_Init();

  // Add support for icons
  io.Fonts->AddFontDefault();

  // merge in icons from Font Awesome
  static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_16_FA, 0 };
  ImFontConfig icons_config;
  icons_config.MergeMode = true;
  icons_config.PixelSnapH = true;
  icons_config.GlyphMinAdvanceX = 13.0f;

  const auto font_path = FONT_ICON_FILE_NAME_FAS;
  const auto font_ptr =
    io.Fonts->AddFontFromFileTTF(font_path, 13.0f, &icons_config, icons_ranges);
  // use FONT_ICON_FILE_NAME_FAR if you want regular instead of solid

  if (!font_ptr) {
    spdlog::error("ImguiAdapter: Failed to load font '{}'", font_path);
  }

  return true;
}

void
ImguiAdapter::Resize(size_t newWidth, size_t newHeight)
{
  ImGuiIO& io = ImGui::GetIO();
  io.DisplaySize = ImVec2((float)newWidth, (float)newHeight);
}

void
ImguiAdapter::BeginFrame()
{
  ImGui_ImplOpenGL3_NewFrame();
  ImGuiIO& io = ImGui::GetIO();
  // Setup display size (every frame to accommodate for window resizing)
  // TODO
  int w = 500, h = 500;
  int display_w, display_h;
  // w = context.getCurrentViewport().getWidth();
  // h = context.getCurrentViewport().getHeight();
  // io.DisplaySize = ImVec2((float)w, (float)h);
  // io.DisplayFramebufferScale = ImVec2(1.0,1.0);
  io.FontGlobalScale = m_Scaling;

  // Setup time step
  io.DeltaTime = (float)(1.0f / 60.0f);

  ImGui::NewFrame();
}
void
ImguiAdapter::EndFrame()
{
  ImGui::Render();
}

void
ImguiAdapter::RenderCurrentFrame()
{
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void
ImguiAdapter::Destroy()
{
  ImGui_ImplOpenGL3_Shutdown();
}

void
ImguiAdapter::OnCharacter(size_t character)
{
  ImGuiIO& io = ImGui::GetIO();
  io.AddInputCharacter(character);
}

void
ImguiAdapter::OnKey(size_t key, bool isDown, ModifierBitmap mod)
{
  ImGuiIO& io = ImGui::GetIO();
  io.KeyCtrl = mod & ModifierBitmap::CTRL;
  io.KeyShift = mod & ModifierBitmap::SHIFT;
  if (mod == ModifierBitmap::CTRL) {
    if (key == GLFW_KEY_A) {
      io.KeysDown[INTERNAL_KEY_CTRL_A] = isDown;
    }
    if (key == GLFW_KEY_C) {
      io.KeysDown[INTERNAL_KEY_CTRL_C] = isDown;
    }
    if (key == GLFW_KEY_V) {
      io.KeysDown[INTERNAL_KEY_CTRL_V] = isDown;
    }
    if (key == GLFW_KEY_X) {
      io.KeysDown[INTERNAL_KEY_CTRL_X] = isDown;
    }
    if (key == GLFW_KEY_Y) {
      io.KeysDown[INTERNAL_KEY_CTRL_Y] = isDown;
    }
    if (key == GLFW_KEY_Z) {
      io.KeysDown[INTERNAL_KEY_CTRL_Z] = isDown;
    }
    return;
  }
  if (helper::MapGLFWKeyTo256Array(key) == 256)
    return;
  io.KeysDown[helper::MapGLFWKeyTo256Array(key)] = isDown;
  /*if(isDown && std::isalnum(key))
  {
      io.AddInputCharacter(key);
  }*/
}

void
ImguiAdapter::OnMousePositionRelative(float deltaX, float deltaY)
{
  posX += deltaX;
  posY += deltaY;
  ImGuiIO& io = ImGui::GetIO();
  io.MousePos = ImVec2(posX, posY);
}

void
ImguiAdapter::OnMousePosition(float x, float y)
{
  ImGuiIO& io = ImGui::GetIO();
  io.MousePos = ImVec2(x, y);
}

void
ImguiAdapter::OnScroll(float deltaX, float deltaY)
{
  ImGuiIO& io = ImGui::GetIO();
  static float absoluteY = 0.0;
  absoluteY += deltaY;
  io.MouseWheel = absoluteY;
}

void
ImguiAdapter::OnButton(size_t buttonID, bool isPressed)
{
  assert(buttonID < 5);
  ImGuiIO& io = ImGui::GetIO();
  if (buttonID == 3) {
    io.MouseWheel = 1.0;
  } else if (buttonID == 4) {
    io.MouseWheel = -1.0;
  } else {
    io.MouseDown[buttonID] = isPressed;
  }
}

bool
ImguiAdapter::IsVisible()
{
  return m_Visibility;
}
void
ImguiAdapter::SetVisibility(bool isVisible)
{
  m_Visibility = isVisible;
}

void
ImguiAdapter::SetScaling(float scale)
{
  m_Scaling = scale;
}

bool
ImguiAdapter::WantCaptureMouse() const
{
  ImGuiIO& io = ImGui::GetIO();
  return (m_Visibility && io.WantCaptureMouse);
}
bool
ImguiAdapter::WantCaptureKeyboard() const
{
  ImGuiIO& io = ImGui::GetIO();
  return (m_Visibility && io.WantCaptureKeyboard);
}
