#ifndef RAYMARCHER_RENDERER_WIDGET_HPP
#define RAYMARCHER_RENDERER_WIDGET_HPP

#include <geGL/geGL.h>
#include "Raymarcher.hpp"
#include "IWidget.hpp"

namespace raymarcher 
{
/**
 * @brief Renders RayMarching's scene in a ImGUI window
 */
class RendererWidget: public IWidget
{
    public:
        RendererWidget() = default;
        explicit RendererWidget(std::shared_ptr<Raymarcher> rm);

        auto Render() -> void;
        auto SetViewportSize(size_t width, size_t height) -> void;
    private:
        auto Reinitialize() -> void;
        std::shared_ptr<Raymarcher>             raymarcher;
        std::shared_ptr<ge::gl::Framebuffer>    targetFBO;
        std::shared_ptr<ge::gl::Texture>        colorImage;

        size_t viewportWidth;
        size_t viewportHeight;
};

}

#endif
