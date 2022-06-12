cmake_minimum_required (VERSION 2.8.12)
project (gl)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")

find_package(OpenGL)
if (OPENGL_FOUND)
  MESSAGE("OpenGL Correctly Found")
  include_directories(${OPENGL_INCLUDE_DIR})
else (OPENGL_FOUND)
  MESSAGE("OpenGL environment missing")
endif()


#add_subdirectory(ext/glfw)
#include_directories(ext/glfw/include)

find_package(geGL REQUIRED)

# # Compile imgui
# add_library(imgui SHARED 
# 3rd/imgui/imgui.cpp
# 3rd/imgui/imgui_demo.cpp
# 3rd/imgui/imgui_draw.cpp
# 3rd/imgui/imgui_tables.cpp
# 3rd/imgui/imgui_widgets.cpp
# 3rd/imgui/imconfig.h
# 3rd/imgui/imgui.h
# 3rd/imgui/imgui_internal.h
# 3rd/imgui/imstb_rectpack.h
# 3rd/imgui/imstb_textedit.h
# 3rd/imgui/imstb_truetype.h

# 3rd/imgui/backends/imgui_impl_opengl3.h
# 3rd/imgui/backends/imgui_impl_opengl3.cpp

# 3rd/imgui/misc/cpp/imgui_stdlib.cpp
# 3rd/imgui/misc/cpp/imgui_stdlib.h
# )
# target_include_directories(imgui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/3rd/imgui)
# target_include_directories(imgui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/3rd/imgui-markdown/)
# target_link_libraries(imgui PUBLIC GLEW)

add_library(common
    src/helpers/Args.cpp
    src/helpers/FileHelper.cpp
    src/helpers/observer.hpp
)
set_property(TARGET common PROPERTY CXX_STANDARD 17)
target_link_libraries(common PUBLIC
    CONAN_PKG::spdlog
)
target_include_directories(common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_library(bvhlibrary
    src/bvh/BVHLibrary.cpp
    src/bvh/BVHGenerator.cpp
)
set_property(TARGET bvhlibrary PROPERTY CXX_STANDARD 17)
target_link_libraries(bvhlibrary PUBLIC 
    common
    CONAN_PKG::glm
)

add_executable (sandbox
    src/main.cpp

    src/raymarching/SDF.cpp
    src/raymarching/Raymarcher.cpp
    src/raymarching/Raymarcher_Helpers.cpp
    src/raymarching/SDFLibrary.cpp

    src/rendering/FlyingCamera.cpp
    src/rendering/OrbitCamera.cpp
    src/rendering/InterpolatedCamera.cpp
    src/rendering/GLFWCamera.cpp

    src/widgets/WidgetBase.cpp
    src/widgets/WidgetManager.cpp
    src/widgets/WindowWidget.cpp
    src/widgets/RendererWidget.cpp
    src/widgets/RendererAttributesWidget.cpp
    src/widgets/MenuWidget.cpp
    src/widgets/WelcomeWidget.cpp
    src/widgets/EditWidget.cpp
    src/widgets/BVHCalculatorWidget.cpp
    src/widgets/FPSMeter.cpp

    src/application/SandboxApplication.cpp
    src/application/TestApplication.cpp
    src/application/ImguiAdapter.cpp

    src/bindings/imgui_impl_opengl3.cpp
    src/bindings/imgui_stdlib.cpp
)
set_property(TARGET sandbox PROPERTY CXX_STANDARD 17)
target_link_libraries(sandbox 
    geGL::geGL  
    bvhlibrary
    dl 
)
target_link_libraries(sandbox 
    CONAN_PKG::spdlog
    CONAN_PKG::imgui
    CONAN_PKG::glfw 
    CONAN_PKG::glew 
    ${OPENGL_LIBRARIES}
)
target_include_directories(sandbox PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

add_executable ( bvhgenerator 
    src/generator_main.cpp
)
set_property(TARGET bvhgenerator PROPERTY CXX_STANDARD 17)
target_link_libraries(bvhgenerator bvhlibrary)
