#include "RendererWidget.hpp"
#include <imgui.h>
#include <iostream>
#include <GL/gl.h>

using namespace raymarcher;

RendererWidget::RendererWidget(std::shared_ptr<Raymarcher> rm) :
    raymarcher { std::move(rm) },
    viewportWidth { 100 },
    viewportHeight { 100 }
{
    Reinitialize();
}

auto RendererWidget::Render() -> void
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


    if(ImGui::BeginChild("Render"))
    {
        ImGui::Image(reinterpret_cast<void*>(colorImage->getId()), ImVec2(viewportWidth, viewportHeight), ImVec2(1,1), ImVec2(0, 0));
    }
    ImGui::EndChild();
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
