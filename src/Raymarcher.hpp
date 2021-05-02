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
    PHONG           = 0,
    /// Render depth (similar to rendering depth buffer)
    DEPTH           = 1,
    /// Render normals (each vector is encoded as RGB)
    NORMAL          = 2,
    /// Render count of iterations per fragment as intensity
    ITERATIONS      = 3,
    /// Render bitmap (white for geometry, black for else)
    BITMAP          = 4,
};

/**
 * @brief Affect rendering output
 */
struct RaymarchingAttributes
{
    /// Focus distance
    float        fx                 = 1.0;
    /// Width/Height ratio
    float        aspectRatio        = 1.0;
    /// Maximum iterations per single ray
    size_t       maximumIterations  = 100;
    /// Epsilon when comparing distance to surface
    double       maximumPrecision   = 0.001;
    /// Scene's far plane
    double       farPlaneDistance   = 200.0;

    /// Type of shading
    ShadingMode  mode;
    /// Phong: ambient coefficient
    float        ambientCoef        = 0.0;
    /// Phong: specularity coefficient
    float        specularityCoef    = 0.3;
    /// Phong: sun intensity 
    bool         isSunDirectional   = false;
    /// Phong: sun intensity 
    float        sunIntensity       = 1.0;
    /// Phong: sun position / direction 
    glm::vec3    sunPosition        = glm::vec3(1.0);
    /// Phong: sun color
    glm::vec3    sunColor           = glm::vec3(1.0);
    /// Phong: fog color
    glm::vec3    fogColor           = glm::vec3(0.871,0.871,1.0);
    /// Phong: Should render shadows in Phong Shading
    bool         renderShadows      = true;
    /// Phong: Should render fog in Phong Shading
    bool         renderFog          = true;
};
/**
 * @brief Generic renderer of Signed-Distance Functions
 *
 * This renderer implements different shading modes, accepts 
 * various parameters.
 */
class Raymarcher
{
    public:

    class Pimpl;

    Raymarcher();
    ~Raymarcher();

    auto SetCamera(std::shared_ptr<ICamera> camera) -> void;
    auto GetCamera() -> std::shared_ptr<ICamera>;

    /// Set and compile SDF
    auto SetSDF(std::shared_ptr<ISDF> sdf) -> void;

    /* Lighting-specific methods */
    auto SetShadingMode(ShadingMode mode) -> void ;
    auto GetShadingMode() -> ShadingMode;

    /* Phong related */
    auto IsSunDirectional() const -> bool;
    auto SetSunDirectional(bool setDirectional) -> void;
    auto SetSun(glm::vec3 directory) -> void;
    auto GetSun() const -> glm::vec3;
    auto SetSunIntensity(float intensity) -> void;
    auto GetSunIntensity() const -> float;
    auto SetSunColour(glm::vec3 color) -> void;
    auto GetSunColour() const -> glm::vec3;

    auto SetFogColour(glm::vec3 color) -> void;
    auto GetFogColour() const -> glm::vec3;

    auto SetSpecularityCoef(float c) -> void;
    auto GetSpecularityCoef() -> float;

    auto SetRenderFog(bool shouldRenderFog) -> void;
    auto IsFogRendered() -> bool;

    auto SetRenderShadows(bool shouldRenderShadow) -> void;
    auto IsShadowsRendered() -> bool;

    /* Rendering-specific methods */
    auto SetRaymarchingAttributes(const RaymarchingAttributes& attributes) -> void;
    auto GetRaymarchingAttributes() const -> const RaymarchingAttributes&;

    auto SetFx(float fx) -> void;
    auto GetFx() const -> float;

    auto SetAspect(float aspect) -> void;
    auto GetAspect() const -> float;

    auto SetMaximumIterations(size_t maximum) -> void;
    auto GetMaximumIterations() -> size_t;

    auto SetEps(float eps) -> void;
    auto GetEps() -> float;

    auto SetAmbientCoef(float c) -> void;
    auto GetAmbientCoef() -> float;

    auto SetFarPlaneDistance(float distance) -> void;
    auto GetFarPlaneDistance() -> float;

    /* User-data passing */
    auto GetUserUniforms() -> const std::vector<ISDF::Uniform>&;
    auto SetUniform(std::string name, float value) -> bool;

    /* Rendering itself */

    /// Render to bound framebuffer
    auto Render() -> void;
    private:
    std::unique_ptr<Pimpl> pimpl;
};

}

#endif
