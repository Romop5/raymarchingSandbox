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
    /// Render using Phong shading
    PHONG = 0,
    /// Render depth (similar to rendering depth buffer)
    DEPTH = 1,
    /// Render normals (each vector is encoded as RGB)
    NORMAL = 2,
    /// Render count of iterations per fragment as intensity
    ITERATIONS = 3,
};

struct RaymarchingAttributes
{
    size_t maximumIterations    = 100;
    double maximumPrecision     = 0.001;

    ShadingMode  mode;
    float        ambientCoef    = 0.0;
    float        specularityCoef = 0.3;
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
    auto GetCamera() -> std::shared_ptr<ICamera>;
    auto SetSDF(std::shared_ptr<ISDF> sdf) -> void;
   
    /* Lighting-specific methods */
    auto SetShadingMode(ShadingMode mode) -> void ;
    auto GetShadingMode() -> ShadingMode;


    /* Phong related */
    auto SetSun(glm::vec3 directory) -> void;
    auto SetSkyColour(glm::vec3 color) -> void;

    /* Rendering-specific methods */
    auto SetRaymarchingAttributes(const RaymarchingAttributes& attributes) -> void;

    auto SetMaximumIterations(size_t maximum) -> void;
    auto GetMaximumIterations() -> size_t;

    auto SetEps(float eps) -> void;
    auto GetEps() -> float;

    auto SetAmbientCoef(float c) -> void;
    auto GetAmbientCoef() -> float;

    auto SetSpecularityCoef(float c) -> void;
    auto GetSpecularityCoef() -> float;

    auto Render() -> void;
    private:
    std::unique_ptr<Pimpl> pimpl;
};

}

#endif
