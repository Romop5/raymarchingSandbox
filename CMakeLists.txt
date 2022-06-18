cmake_minimum_required (VERSION 2.8.12)
project (gl)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)


include(FetchContent)
FetchContent_Declare(
  geGL 
  GIT_REPOSITORY https://github.com/dormon/geGL.git
  GIT_TAG 5464a364329982d5d5b04574e16458581cbeb806 
)

FetchContent_MakeAvailable(geGL)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")

find_package(OpenGL)
if (OPENGL_FOUND)
  MESSAGE("OpenGL Correctly Found")
  include_directories(${OPENGL_INCLUDE_DIR})
else (OPENGL_FOUND)
  MESSAGE("OpenGL environment missing")
endif()

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


    src/bindings/ImguiAdapter.cpp
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
