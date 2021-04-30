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
    float fx                    = 1.0;
    float aspectRatio           = 1.0;
    bool renderShadows          = true;
    bool renderFog              = true;
    size_t maximumIterations    = 100;
    double maximumPrecision     = 0.001;
    double farPlaneDistance     = 200.0;

    ShadingMode  mode;
    float        ambientCoef        = 0.0;
    float        specularityCoef    = 0.3;
    glm::vec3    sunColor           = glm::vec3(1.0);
    glm::vec3    fogColor           = glm::vec3(0.871,0.871,1.0);
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

    auto Render() -> void;
    private:
    std::unique_ptr<Pimpl> pimpl;
};

}

#endif
