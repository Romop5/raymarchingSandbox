#include <imgui.h>

#include "WelcomeWidget.hpp"

using namespace raymarcher;

WelcomeWidget::WelcomeWidget(WidgetManager& windowManager)
{
  SetTitle("Welcome screen");
}

auto
WelcomeWidget::RenderContent() -> void
{
  WindowWidget::RenderContent();

  ImGui::Text(R"(
  _____                                      _     _
 |  __ \                                    | |   (_)
 | |__) |__ _ _   _ _ __ ___   __ _ _ __ ___| |__  _ _ __   __ _ 
 |  _  // _` | | | | '_ ` _ \ / _` | '__/ __| '_ \| | '_ \ / _` |
 | | \ \ (_| | |_| | | | | | | (_| | | | (__| | | | | | | | (_| |
 |_|  \_\__,_|\__, |_| |_| |_|\__,_|_|  \___|_| |_|_|_| |_|\__, |
         _____ __/ |           _ _                          __/ |
        / ____|___/           | | |                        |___/ 
       | (___   __ _ _ __   __| | |__   _____  __                
        \___ \ / _` | '_ \ / _` | '_ \ / _ \ \/ /                
        ____) | (_| | | | | (_| | |_) | (_) >  <                 
       |_____/ \__,_|_| |_|\__,_|_.__/ \___/_/\_\                
    )");
  ImGui::Text(R"(
Raymarching is an generic algorithm for finding an intersection with a ray and geometry, defined
using so-called Signed Distance Function (SDF). In comparison to rasterizers or ray
tracers, ray marching allows to use geometry, which is hard to express explicitely.
    )");

  if (ImGui::CollapsingHeader("Features of sandbox")) {
    ImGui::BulletText("Examples (primitives, operations, scenes)");
    ImGui::BulletText("Rich shading & rendering parameters");
    ImGui::BulletText("Live coding with auto-compile");
    ImGui::BulletText("File save/load (SDF)");
  }

  if (ImGui::CollapsingHeader("Algorithm of raymarching")) {
    ImGui::Text(R"(
A generic idea of raymarching is that the intersection with geometry can be found by moving
(marching) along the ray and determing the current distance to the closest geometry.
If such distance is small enough, the current point is declared as intersection, and the distance to
the point from the origin is reported.
)");
  }

  if (ImGui::CollapsingHeader("Sphere Tracing Algorithm")) {
    ImGui::Text(R"(
The definition above does not give an exact approach to compute the distance. Therefore, various
variants of raymarching exists. 
)");

    ImGui::Text(R"(
Sphere Tracing sphereTracing originates from 1995. The idea is that positions along the ray
are marched with distances, equal to the closest geometry, thus by computing SDF at each position.
The name of algorithm originates from the fact that evaluation of SDF at points along the ray gives
the sphere where none of geometry lies in.
)");
  }

  if (ImGui::CollapsingHeader("Implementation using GLSL")) {
    ImGui::Text(R"(
In practice, ray marching is implemented for instance using OpenGL and quad rendering.
A fullscreen quad is rendered and the algorithm is encoded in Fragment Shader.

Then, for each pixel of the quad, UV coordinate is expressed and translated so that the center of
screen is the origin of UV space. The position of camera, defining the origin o of rays, is passed
as shader's uniform, together with 4x4 transformation matrix M.

Then, by transforming local look vector l = (UV.x, UV.y, 1.0) by multiplying with matrix M and
normalizing the output vector, the world-space look vector is obtained. The vector is then passed to
Raymarching algorithm as ray direction.

In addition to maximum iterations, the computation can also be bounded by maximum distance,
effictively defining a far clipping plane.

After determining distance to geometry, the distance D can be visualized in multiple ways:

    Plain-rendering
    In this mode, the output color c of pixel p is computed as 'p = c <=> D > 0'
    This results to colour pixels where the geometry was hit by camera, or the second color (e.g.
    black).

    Z-buffer rendering
    In this mode, the output color c of pixel p is computed as linear interpolation of two colours where
    the parameter is the distance.
    This results to Z-buffer like rendering.

    Iterations rendering
    The output colour can be also expressed using the number of iterations n, which were required for
    Raymarching algorithm to finish for given pixel p. The n can be used to interpolate two
    colours. This mode is especially useful to demonstrate the amount of iterations that are needed
    at the edges of geometry.

    Phong rendering
    Finally, the ray marching can be used to display properly shaded models. 

        )");
  }

  if (ImGui::CollapsingHeader("Signed Distance Function")) {
    ImGui::Text(R"(
A SDF function is an arbitrary continuous function R^3->R, which maps
position to the distance to the nearest point of geometry. In contrast to a metric function, SDF can
also obtain negative values. These are for points, which are inside the object.

There are two major ways how to define an SDF function:

By defining an algebraic expression
    SDF functions are commonly made of primitive geometry such as spheres, cubes, etc.
    subsequently, the primitives are merged using operators. This results to CSG-like building of resulting SDF.
By using a texture
    By precomputing SDF function using grid of points, a 3D texture can be created.
    This is useful, for instance, for rendering geometry, expressed by a point cloud.

        )");
  }
}
