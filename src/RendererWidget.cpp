#include "RendererWidget.hpp"
#include <imgui.h>
#include <iostream>
#include <GL/gl.h>
#include "IApplication.hpp"
#include "RendererAttributesWidget.hpp"

using namespace raymarcher;

RendererWidget::RendererWidget(std::shared_ptr<Raymarcher> rm, std::shared_ptr<GLFWCamera> cam) :
    raymarcher { std::move(rm) },
    camera { std::move(cam) },
    viewportWidth { 100 },
    viewportHeight { 100 }
{
    raymarcher->SetCamera(camera);
    SetTitle("Preview");
    Reinitialize();
}

auto RendererWidget::RenderContent() -> void
{
    if(!targetFBO || !colorImage)
    {
        Reinitialize();
        return;
    }

    targetFBO->bind();
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glClear(GL_COLOR_BUFFER_BIT);
    glViewport(0,0, viewportWidth, viewportHeight);
    raymarcher->Render();
    targetFBO->unbind();
    glPopAttrib();


    if(ImGui::Button("Settings"))
    {
        AddWidget(std::make_shared<RendererAttributesWidget>(raymarcher));
    }

    if(ImGui::BeginChild("Render"))
    {
        ImGui::Image(reinterpret_cast<void*>(colorImage->getId()), ImGui::GetWindowSize(), ImVec2(1,1), ImVec2(0, 0));
        if(ImGui::IsItemClicked())
        {
            auto& app = IApplication::GetApplication();
            app.SetFocus(camera);
        }
    }
    ImGui::EndChild();

    WindowWidget::RenderContent();
}

auto RendererWidget::Render() -> void
{
    ImGui::SetNextWindowSize(ImVec2(200,200), ImGuiCond_Once);
    WindowWidget::Render();
}

auto RendererWidget::SetViewportSize(size_t width, size_t height) -> void
{
    viewportWidth = width;
    viewportHeight = height;
    Reinitialize();
}


auto RendererWidget::Reinitialize() -> void
{
    colorImage = std::make_shared<ge::gl::Texture>(GL_TEXTURE_2D, GL_RGB8, 1, viewportWidth, viewportHeight);
    targetFBO = std::make_shared<ge::gl::Framebuffer>();
    targetFBO->attachTexture(GL_COLOR_ATTACHMENT0, colorImage.get());
    auto status = targetFBO->check();
    if(!status)
    {
        std::cout << "Texture: " << colorImage->getInfo() << std::endl;
        std::cout << "Info: " << targetFBO->getInfo() << std::endl;
        assert(false);
    }
    std::cout << "Reinitialized " << std::endl;
}
