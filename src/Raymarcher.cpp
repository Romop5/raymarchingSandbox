#include "Raymarcher.hpp"
#include "Raymarcher_Helpers.hpp"
#include "helpers.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

using namespace raymarcher;

class Raymarcher::Pimpl
{
    public:
    auto SetCamera(std::shared_ptr<ICamera> camera) -> void;
    auto SetSDF(std::shared_ptr<ISDF> sdf) -> void;
   
    /* Lighting-specific methods */
    auto SetShadingMode(raymarcher::ShadingMode mode) -> void;

    /* Phong related */
    auto SetSun(glm::vec3 directory) -> void;
    auto SetSkyColour(glm::vec3 color) -> void;

    /* Rendering-specific methods */
    auto SetRaymarchingAttributes(const RaymarchingAttributes& attributes) -> void;
    auto Render() -> void;

    private:
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

auto Raymarcher::Pimpl::SetSDF(std::shared_ptr<ISDF> sdf) -> void
{
    this->sdf = sdf;
    Compile();
}

auto Raymarcher::Pimpl::SetShadingMode(raymarcher::ShadingMode mode) -> void
{
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
        time += 0.05;
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
    }
}

auto Raymarcher::Pimpl::Compile() -> bool
{
    auto program = std::make_unique<ge::gl::Program>();

    auto vs = std::make_shared<ge::gl::Shader>(GL_VERTEX_SHADER);
    vs->compile(ConstructRenderedVertexShader());

    auto fs = std::make_shared<ge::gl::Shader>(GL_FRAGMENT_SHADER);
    fs->compile(ConstructRenderedFragmentShader(sdf->GetGLSLCode())); 
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

auto Raymarcher::SetSDF(std::shared_ptr<ISDF> sdf) -> void
{
    pimpl->SetSDF(sdf);
}

auto Raymarcher::SetShadingMode(ShadingMode mode) -> void
{
    pimpl->SetShadingMode(mode);
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

auto Raymarcher::Render() -> void
{
    pimpl->Render();
}
