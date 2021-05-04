# Raymarching Sandbox

## About
A school project for VGE@FIT VUT subject.

The project has two goals:
- educational: it demonstrates use of raymarching with primitives, operations and simple scenes
- acceleration of SDF computing: the project implements BVH calculator + testing facility

## What's done

- Sandbox
    A GUI application, which allows live coding, experimenting with parameters of Ray marching,
    Phong shading etc.
- FPSTest
    A command-line option when starting sandbox.
- BVHGenerator
    A command-line utility, which generates dataset (scene made of spheres), compute and optimize
    BVH hierarchy, and produce GLSL's SDF code into optimized.sdf/nonoptimized.sdf

## Usage
Sandbox can be easily run as it is. SDFs are saved and loaded to .sdf files.

For running FPS test SDF file, use following parameters:

```
./sandbox --test  -i <file-to-sdf> [-f]
```
Optional parameter *-f* allows flying with FlyCamera over scene.
The scene's parameters can be accessed using F11.

## BVH optimizator

BVH optimizator is available from both GUI and command-line. In both cases, the calculator 
works over generated dataset.

The dataset is generated to contain clusters. 

```
./bvhgenerator -c <CLUSTERS> -l <MAXIMUM-BVH-HIERARCHY-LEVEL> -s <MAX-VOLUME-RATIO>
```
where
`-c` is number of clusters in generated dataset,
`-l` is maximal depth of BVH tree. E.g. by setting depth to 0, BVH tree contains a single parent
with all leaves as children.
`-s` is maximal ratio of child-to-parent volume. E.g. if ratio is 0.5, child's node is 
removed if its volume divided by parent is more than 0.5.

## Videos
https://www.youtube.com/watch?v=BY6HxV_988k
https://www.youtube.com/watch?v=lISFJknwtLg

## Author
Roman Dobias (xdobia11(at)stud.fit.vutbr.cz)

## References
- https://iquilezles.org/www/articles/distfunctions/distfunctions.htm  - SDF functions, Ray marching principles, etc
- https://www.shadertoy.com/view/Xds3zN - implementation in GLSL, calculating normal for surface, implementation of Raymarching algorithm

## Depedencies

- ImGUI (https://github.com/ocornut/imgui): C++ Immediate GUI library
- geGL (https://github.com/dormon/geGL): RAII-friendly C++ OpenGL wrapper
- GLFW (https://github.com/glfw/glfw): Window creation

