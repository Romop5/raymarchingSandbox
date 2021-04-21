cmake_minimum_required (VERSION 2.8.12)
project (gl)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")

find_package(OpenGL)
if (OPENGL_FOUND)
  MESSAGE("OpenGL Correctly Found")
  include_directories(${OPENGL_INCLUDE_DIR})
else (OPENGL_FOUND)
  MESSAGE("OpenGL environment missing")
endif()


option(GLFW_BUILD_EXAMPLES "Build the GLFW example programs" OFF)
option(GLFW_BUILD_TESTS "Build the GLFW test programs" OFF)
option(GLFW_BUILD_DOCS "Build the GLFW documentation" OFF)

#add_subdirectory(ext/glfw)
#include_directories(ext/glfw/include)

find_package(geGL REQUIRED)
find_package(GLFW REQUIRED)
find_package(GLM REQUIRED)

# Compile imgui
add_library(imgui SHARED 
    3rd/imgui/imgui.cpp
    3rd/imgui/imgui_demo.cpp
    3rd/imgui/imgui_draw.cpp
    3rd/imgui/imgui_tables.cpp
    3rd/imgui/imgui_widgets.cpp
    3rd/imgui/imconfig.h
    3rd/imgui/imgui.h
    3rd/imgui/imgui_internal.h
    3rd/imgui/imstb_rectpack.h
    3rd/imgui/imstb_textedit.h
    3rd/imgui/imstb_truetype.h

    3rd/imgui/backends/imgui_impl_opengl3.h
    3rd/imgui/backends/imgui_impl_opengl3.cpp

    3rd/imgui/misc/cpp/imgui_stdlib.cpp
    3rd/imgui/misc/cpp/imgui_stdlib.h
)
target_include_directories(imgui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/3rd/imgui)
target_link_libraries(imgui PUBLIC GLEW)


add_executable (sandbox
    src/main.cpp
    src/SDF.cpp
    src/Raymarcher.cpp
    src/Raymarcher_Helpers.cpp
    src/FlyingCamera.cpp
    src/OrbitCamera.cpp
    src/GLFWCamera.cpp
    src/ImguiAdapter.cpp

    src/WidgetBase.cpp
    src/WidgetManager.cpp
    src/WindowWidget.cpp
    src/RendererWidget.cpp
    src/RendererAttributesWidget.cpp
    src/MenuWidget.cpp
    src/EditWidget.cpp
    src/SDFLibrary.cpp
    src/SandboxApplication.cpp
    src/TestApplication.cpp
)
set_property(TARGET sandbox PROPERTY CXX_STANDARD 17)
target_link_libraries(sandbox geGL dl imgui)

target_link_libraries (sandbox glfw ${GLFW_LIBRARIES} ${OPENGL_LIBRARIES})
