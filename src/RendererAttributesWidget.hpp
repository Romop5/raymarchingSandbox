#ifndef RAYMARCHER_RENDERER_ATTRIBUTES_WIDGET_HPP
#define RAYMARCHER_RENDERER_ATTRIBUTES_WIDGET_HPP

#include "Raymarcher.hpp"
#include "WindowWidget.hpp"
#include <map>

namespace raymarcher
{
class RendererAttributesWidget: public WindowWidget
{
    public:
        RendererAttributesWidget() = default;
        explicit RendererAttributesWidget(std::shared_ptr<Raymarcher> rm);

        auto RenderContent() -> void override;
    private:
        std::shared_ptr<Raymarcher>             raymarcher;
        std::map<std::string, float>            lastUniformValue;
};

}

#endif
