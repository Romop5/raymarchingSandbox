#include "helpers/helpers.hpp"
#include "raymarching/Raymarcher_Helpers.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <spdlog/spdlog.h>

#include "raymarching/Raymarcher.hpp"

using namespace raymarcher;

class Raymarcher::Pimpl
{
public:
  Pimpl() = default;
  auto SetCamera(std::shared_ptr<ICamera> camera) -> void;
  auto GetCamera() -> std::shared_ptr<ICamera>;
  auto SetSDF(std::shared_ptr<ISDF> sdf) -> void;

  /* Phong related */
  auto SetSun(glm::vec3 directory) -> void;
  auto SetSkyColour(glm::vec3 color) -> void;

  /* Rendering-specific methods */
  auto SetRaymarchingAttributes(const RaymarchingAttributes& attributes)
    -> void;
  auto Render() -> void;

  auto UpdateUniforms() -> void;
  auto Compile() -> bool;
  std::shared_ptr<ICamera> camera;
  std::shared_ptr<ISDF> sdf;

  std::unique_ptr<ge::gl::Program> program;

  FullscreenQuad fullscreenQuad;
  RaymarchingAttributes attributes;
};

///////////////////////////////////////////////////////////////////////////////
// Raymarcher:pimpl
///////////////////////////////////////////////////////////////////////////////
auto
Raymarcher::Pimpl::SetCamera(std::shared_ptr<ICamera> camera) -> void
{
  this->camera = camera;
}

auto
Raymarcher::Pimpl::GetCamera() -> std::shared_ptr<ICamera>
{
  return this->camera;
}

auto
Raymarcher::Pimpl::SetSDF(std::shared_ptr<ISDF> sdf) -> void
{
  this->sdf = sdf;
  Compile();
}

auto
Raymarcher::Pimpl::SetRaymarchingAttributes(
  const RaymarchingAttributes& attributes) -> void
{
  this->attributes = attributes;
  UpdateUniforms();
}

auto
Raymarcher::Pimpl::Render() -> void
{
  try {
    if (program && camera) {
      auto transform = camera->GetTransformation();
      program->use();
      const auto origin = -glm::vec3(transform[3]);
      auto identity = glm::mat4(1.0);
      program->setMatrix4fv("camera_rotation", glm::value_ptr(transform));
      program->set3fv("camera_origin", glm::value_ptr(origin));
      static float time = 0.0;
      time += 0.005;

      // FIX: uniforms may be optimized out by compiler if not referenced
      // so if the SDF function didn not use iTime, it would trigger an
      // exception when setting the value
      if (program->getUniformLocation("iTime") != -1) {
        program->set1f("iTime", time);
      }

      fullscreenQuad.draw();
    }
  } catch (std::exception& e) {
    spdlog::error("Raymarcher::Pimpl::Render: failed to render, reason: {}",
                  e.what());
  }
}

auto
Raymarcher::Pimpl::UpdateUniforms() -> void
{
  if (program) {
    program->set1f("fx", attributes.fx);
    program->set1f("aspectRatio", attributes.aspectRatio);
    program->set1i("g_maxIterations", attributes.maximumIterations);
    program->set1f("g_eps", attributes.maximumPrecision);
    program->set1f("g_stepRatio", attributes.stepRatio);
    program->set1f("farPlane", attributes.farPlaneDistance);
    program->set1f("ambientRatio", attributes.ambientCoef);
    program->set1f("specularity", attributes.specularityCoef);
    program->set1i("renderFog", attributes.renderFog);
    program->set1i("renderShadows", attributes.renderShadows);

    program->set1i("isSunDirectional", attributes.isSunDirectional);
    program->set1f("sunIntensity", attributes.sunIntensity);
    program->set3fv("sunColor", glm::value_ptr(attributes.sunColor));
    program->set3fv("sunPos", glm::value_ptr(attributes.sunPosition));
    program->set3fv("fogColor", glm::value_ptr(attributes.fogColor));
  }
}

auto
Raymarcher::Pimpl::Compile() -> bool
{
  auto program = std::make_unique<ge::gl::Program>();
  program->setNonexistingUniformWarning(true);

  auto vs = std::make_shared<ge::gl::Shader>(GL_VERTEX_SHADER);
  vs->compile(ConstructRenderedVertexShader());
  if (!vs->getCompileStatus()) {
    throw std::runtime_error(vs->getInfoLog());
  }

  auto fs = std::make_shared<ge::gl::Shader>(GL_FRAGMENT_SHADER);
  auto finalFsCode = ConstructRenderedFragmentShader(sdf->GetGLSLCode());
  spdlog::trace("Raymarcher::Pimpl::Compile FS:\n {}", finalFsCode);
  fs->compile(finalFsCode);
  if (!fs->getCompileStatus()) {
    throw std::runtime_error(fs->getInfoLog());
  }
  program->link({ vs, fs });

  this->program = std::move(program);

  UpdateUniforms();
  return true;
}

///////////////////////////////////////////////////////////////////////////////
// Raymarcher
///////////////////////////////////////////////////////////////////////////////

Raymarcher::Raymarcher()
  : pimpl{ std::make_unique<Raymarcher::Pimpl>() }
{}

Raymarcher::~Raymarcher() = default;

auto
Raymarcher::SetCamera(std::shared_ptr<ICamera> camera) -> void
{
  pimpl->SetCamera(camera);
}

auto
Raymarcher::GetCamera() -> std::shared_ptr<ICamera>
{
  return pimpl->GetCamera();
}

auto
Raymarcher::SetSDF(std::shared_ptr<ISDF> sdf) -> void
{
  pimpl->SetSDF(sdf);
}

auto
Raymarcher::SetShadingMode(ShadingMode mode) -> void
{
  pimpl->attributes.mode = mode;
  pimpl->program->set1i("coloringMode", static_cast<int>(mode));
}

auto
Raymarcher::GetShadingMode() -> ShadingMode
{
  return pimpl->attributes.mode;
}

auto
Raymarcher::IsSunDirectional() const -> bool
{
  return pimpl->attributes.isSunDirectional;
}

auto
Raymarcher::SetSunDirectional(bool setDirectional) -> void
{
  pimpl->attributes.isSunDirectional = setDirectional;
  pimpl->UpdateUniforms();
}

auto
Raymarcher::SetSun(glm::vec3 directory) -> void
{
  pimpl->attributes.sunPosition = directory;
  pimpl->UpdateUniforms();
}

auto
Raymarcher::GetSun() const -> glm::vec3
{
  return pimpl->attributes.sunPosition;
}

auto
Raymarcher::SetSunIntensity(float intensity) -> void
{
  pimpl->attributes.sunIntensity = intensity;
  pimpl->UpdateUniforms();
}

auto
Raymarcher::GetSunIntensity() const -> float
{
  return pimpl->attributes.sunIntensity;
}

auto
Raymarcher::SetSunColour(glm::vec3 color) -> void
{
  pimpl->attributes.sunColor = color;
  pimpl->UpdateUniforms();
}

auto
Raymarcher::GetSunColour() const -> glm::vec3
{
  return pimpl->attributes.sunColor;
}

auto
Raymarcher::SetFogColour(glm::vec3 color) -> void
{
  pimpl->attributes.fogColor = color;
  pimpl->UpdateUniforms();
}

auto
Raymarcher::GetFogColour() const -> glm::vec3
{
  return pimpl->attributes.fogColor;
}

auto
Raymarcher::SetRaymarchingAttributes(const RaymarchingAttributes& attributes)
  -> void
{
  pimpl->SetRaymarchingAttributes(attributes);
}

auto
Raymarcher::GetRaymarchingAttributes() const -> const RaymarchingAttributes&
{
  return pimpl->attributes;
}

auto
Raymarcher::SetFx(float fx) -> void
{
  pimpl->attributes.fx = fx;
  pimpl->UpdateUniforms();
}

auto
Raymarcher::GetFx() const -> float
{
  return pimpl->attributes.fx;
}

auto
Raymarcher::SetAspect(float aspect) -> void
{
  pimpl->attributes.aspectRatio = aspect;
  pimpl->UpdateUniforms();
}

auto
Raymarcher::GetAspect() const -> float
{
  return pimpl->attributes.aspectRatio;
}

auto
Raymarcher::SetMaximumIterations(size_t maximum) -> void
{
  pimpl->attributes.maximumIterations = maximum;
  pimpl->UpdateUniforms();
}

auto
Raymarcher::GetMaximumIterations() -> size_t
{
  return pimpl->attributes.maximumIterations;
}

auto
Raymarcher::SetEps(float eps) -> void
{
  pimpl->attributes.maximumPrecision = eps;
  pimpl->UpdateUniforms();
}

auto
Raymarcher::GetEps() -> float
{
  return pimpl->attributes.maximumPrecision;
}

auto
Raymarcher::SetStepRatio(float coef) -> void
{
  pimpl->attributes.stepRatio = coef;
  pimpl->UpdateUniforms();
}

auto
Raymarcher::GetStepRatio() const -> float
{
  return pimpl->attributes.stepRatio;
}

auto
Raymarcher::SetAmbientCoef(float c) -> void
{
  pimpl->attributes.ambientCoef = c;
  pimpl->UpdateUniforms();
}

auto
Raymarcher::GetAmbientCoef() -> float
{
  return pimpl->attributes.ambientCoef;
}

auto
Raymarcher::SetSpecularityCoef(float c) -> void
{
  pimpl->attributes.specularityCoef = c;
  pimpl->UpdateUniforms();
}

auto
Raymarcher::GetSpecularityCoef() -> float
{
  return pimpl->attributes.specularityCoef;
}

auto
Raymarcher::SetFarPlaneDistance(float distance) -> void
{
  pimpl->attributes.farPlaneDistance = distance;
  pimpl->UpdateUniforms();
}

auto
Raymarcher::GetFarPlaneDistance() -> float
{
  return pimpl->attributes.farPlaneDistance;
}

auto
Raymarcher::SetRenderFog(bool shouldRenderFog) -> void
{
  pimpl->attributes.renderFog = shouldRenderFog;
  pimpl->UpdateUniforms();
}

auto
Raymarcher::IsFogRendered() -> bool
{
  return pimpl->attributes.renderFog;
}

auto
Raymarcher::SetRenderShadows(bool shouldRenderShadow) -> void
{
  pimpl->attributes.renderShadows = shouldRenderShadow;
  pimpl->UpdateUniforms();
}

auto
Raymarcher::IsShadowsRendered() -> bool
{
  return pimpl->attributes.renderShadows;
}

auto
Raymarcher::GetUserUniforms() -> const std::vector<ISDF::Uniform>&
{
  return pimpl->sdf->GetUniforms();
}

auto
Raymarcher::SetUniform(std::string name, float value) -> bool
{
  try {
    pimpl->program->set1f(name, value);
  } catch (std::exception& e) {
    return false;
  }
  return true;
}

auto
Raymarcher::Render() -> void
{
  pimpl->Render();
}
