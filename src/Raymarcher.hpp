#ifndef RAYMARCHER_RAYMARCHER_HPP
#define RAYMARCHER_RAYMARCHER_HPP
#include <memory>
#include <glm/glm.hpp>
#include "ISDF.hpp"
#include "ICamera.hpp"

namespace raymarcher
{

enum class ShadingMode
{
    /// Render white where ray hits surface
    MASK,
    /// Render using Phong shading
    PHONG,
    /// Render depth (similar to rendering depth buffer)
    DEPTH,
    /// Render normals (each vector is encoded as RGB)
    NORMAL,
    /// Render count of iterations per fragment as intensity
    ITERATIONS
};
/**
 * @brief Generic rendered of SDF
 */
class Raymarcher
{
    public:

    class Pimpl;

    Raymarcher();
    ~Raymarcher();

    auto SetCamera(std::shared_ptr<ICamera> camera) -> void;
    auto SetSDF(std::shared_ptr<ISDF> sdf) -> void;
   
    /* Lighting-specific methods */
    auto SetShadingMode(ShadingMode mode) -> void ;

    /* Phong related */
    auto SetSun(glm::vec3 directory) -> void;
    auto SetSkyColour(glm::vec3 color) -> void;

    /* Rendering-specific methods */
    auto Render() -> void;

    private:
    std::unique_ptr<Pimpl> pimpl;
};

}

#endif
