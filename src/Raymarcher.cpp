#include "Raymarcher.hpp"
#include "Raymarcher_Helpers.hpp"
#include "helpers.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

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
    auto SetRaymarchingAttributes(const RaymarchingAttributes& attributes) -> void;
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
auto Raymarcher::Pimpl::SetCamera(std::shared_ptr<ICamera> camera) -> void
{
    this->camera = camera;
}

auto Raymarcher::Pimpl::GetCamera() -> std::shared_ptr<ICamera>
{
    return this->camera;
}

auto Raymarcher::Pimpl::SetSDF(std::shared_ptr<ISDF> sdf) -> void
{
    this->sdf = sdf;
    Compile();
}

auto Raymarcher::Pimpl::SetSun(glm::vec3 directory) -> void
{
}

auto Raymarcher::Pimpl::SetSkyColour(glm::vec3 color) -> void
{
}

auto Raymarcher::Pimpl::SetRaymarchingAttributes(const RaymarchingAttributes& attributes) -> void
{
    this->attributes = attributes;
    UpdateUniforms();
}

auto Raymarcher::Pimpl::Render() -> void
{
    if(program && camera)
    {
        auto transform = camera->GetTransformation();
        program->use();
        const auto origin = -glm::vec3(transform[3]);
        auto identity = glm::mat4(1.0);
        program->setMatrix4fv("camera_rotation", glm::value_ptr(transform));
        program->set3fv("camera_origin", glm::value_ptr(origin));
        static float time = 0.0;
        time += 0.005;
        program->set1f("iTime", time);

        fullscreenQuad.draw();
    }
}

auto Raymarcher::Pimpl::UpdateUniforms() -> void
{
    if(program)
    {
        program->set1i("g_maxIterations", attributes.maximumIterations);
        program->set1f("g_eps", attributes.maximumPrecision);
        program->set1f("farPlane", attributes.farPlaneDistance);
        program->set1f("ambientRatio", attributes.ambientCoef);
        program->set1f("specularity", attributes.specularityCoef);
        program->set1i("renderFog", attributes.renderFog);
        program->set1i("renderShadows", attributes.renderShadows);
    }
}

auto Raymarcher::Pimpl::Compile() -> bool
{
    auto program = std::make_unique<ge::gl::Program>();

    auto vs = std::make_shared<ge::gl::Shader>(GL_VERTEX_SHADER);
    vs->compile(ConstructRenderedVertexShader());
    if(!vs->getCompileStatus())
    {
        throw std::runtime_error(vs->getInfoLog());
    }

    auto fs = std::make_shared<ge::gl::Shader>(GL_FRAGMENT_SHADER);
    auto finalFsCode = ConstructRenderedFragmentShader(sdf->GetGLSLCode());
    std::cout << "Compiling FS: " << std::endl
              << finalFsCode << std::endl;
    fs->compile(finalFsCode);
    if(!fs->getCompileStatus())
    {
        throw std::runtime_error(fs->getInfoLog());
    }
    program->link({vs,fs});

    this->program = std::move(program);

    UpdateUniforms();
    return true;
}

///////////////////////////////////////////////////////////////////////////////
// Raymarcher
///////////////////////////////////////////////////////////////////////////////

Raymarcher::Raymarcher() :
    pimpl{ std::make_unique<Raymarcher::Pimpl>() } 
{
}

Raymarcher::~Raymarcher() = default;

auto Raymarcher::SetCamera(std::shared_ptr<ICamera> camera) -> void
{
    pimpl->SetCamera(camera);
}

auto Raymarcher::GetCamera() -> std::shared_ptr<ICamera>
{
    return pimpl->GetCamera();
}

auto Raymarcher::SetSDF(std::shared_ptr<ISDF> sdf) -> void
{
    pimpl->SetSDF(sdf);
}

auto Raymarcher::SetShadingMode(ShadingMode mode) -> void
{
    pimpl->attributes.mode = mode;
    pimpl->program->set1i("coloringMode", static_cast<int>(mode));
}

auto Raymarcher::GetShadingMode() -> ShadingMode
{
    return pimpl->attributes.mode;
}

auto Raymarcher::SetSun(glm::vec3 directory) -> void
{
    pimpl->SetSun(directory);
}

auto Raymarcher::SetSkyColour(glm::vec3 color) -> void
{
    pimpl->SetSkyColour(color);
}

auto Raymarcher::SetRaymarchingAttributes(const RaymarchingAttributes& attributes) -> void
{
    pimpl->SetRaymarchingAttributes(attributes);
}


auto Raymarcher::SetMaximumIterations(size_t maximum) -> void
{
    pimpl->attributes.maximumIterations = maximum;
    pimpl->UpdateUniforms();
}

auto Raymarcher::GetMaximumIterations() -> size_t
{
    return pimpl->attributes.maximumIterations;
}


auto Raymarcher::SetEps(float eps) -> void
{
    pimpl->attributes.maximumPrecision = eps;
    pimpl->UpdateUniforms();
}

auto Raymarcher::GetEps() -> float
{
    return pimpl->attributes.maximumPrecision;
}

auto Raymarcher::SetAmbientCoef(float c) -> void
{
    pimpl->attributes.ambientCoef = c;
    pimpl->UpdateUniforms();
}

auto Raymarcher::GetAmbientCoef() -> float
{
    return pimpl->attributes.ambientCoef;
}

auto Raymarcher::SetSpecularityCoef(float c) -> void
{
    pimpl->attributes.specularityCoef = c;
    pimpl->UpdateUniforms();
}

auto Raymarcher::GetSpecularityCoef() -> float
{
    return pimpl->attributes.specularityCoef;
}

auto Raymarcher::SetFarPlaneDistance(float distance) -> void
{
    pimpl->attributes.farPlaneDistance = distance;
    pimpl->UpdateUniforms();
}

auto Raymarcher::GetFarPlaneDistance() -> float
{
    return pimpl->attributes.farPlaneDistance;
}


auto Raymarcher::SetRenderFog(bool shouldRenderFog) -> void
{
    pimpl->attributes.renderFog = shouldRenderFog;
    pimpl->UpdateUniforms();
}

auto Raymarcher::IsFogRendered() -> bool
{
    return pimpl->attributes.renderFog;
}

auto Raymarcher::SetRenderShadows(bool shouldRenderShadow) -> void
{
    pimpl->attributes.renderShadows = shouldRenderShadow;
    pimpl->UpdateUniforms();
}

auto Raymarcher::IsShadowsRendered() -> bool
{
    return pimpl->attributes.renderShadows;
}

auto Raymarcher::Render() -> void
{
    pimpl->Render();
}

