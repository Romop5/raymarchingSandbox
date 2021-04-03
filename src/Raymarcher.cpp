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
    auto Render() -> void;

    private:
    auto Compile() -> bool;
    std::shared_ptr<ICamera> camera;
    std::shared_ptr<ISDF> sdf;

    std::unique_ptr<ge::gl::Program> program; 

    FullscreenQuad fullscreenQuad;
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

auto Raymarcher::Pimpl::Render() -> void
{
    if(program && camera)
    {
        auto transform = camera->GetTransformation();
        program->use();
        const auto rotation = glm::mat3(transform);
        const auto origin = -glm::vec3(transform[3]);
        program->setMatrix4fv("camera_rotation", glm::value_ptr(rotation));
        program->set3fv("camera_origin", glm::value_ptr(origin));
    }
}

auto Raymarcher::Pimpl::Compile() -> bool
{
    auto program = std::make_unique<ge::gl::Program>();

    auto vs = std::make_shared<ge::gl::Shader>(GL_VERTEX_SHADER);
    vs->compile(ConstructRenderedVertexShader());

    auto fs = std::make_shared<ge::gl::Shader>(GL_FRAGMENT_SHADER);
    vs->compile(ConstructRenderedFragmentShader(sdf->GetGLSLCode())); 
    program->link({vs,fs});

    this->program = std::move(program);
    return true;
}

///////////////////////////////////////////////////////////////////////////////
// Raymarcher
///////////////////////////////////////////////////////////////////////////////

Raymarcher::Raymarcher() :
    pimpl{ std::make_unique<Raymarcher::Pimpl>() } 
{
}

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

auto Raymarcher::Render() -> void
{
    pimpl->Render();
}
