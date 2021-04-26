#ifndef RAYMARCHER_MENUWIDGET_HPP
#define RAYMARCHER_MENUWIDGET_HPP

#include "WidgetBase.hpp"
#include "WidgetManager.hpp"
#include "SDFLibrary.hpp"

namespace raymarcher 
{
/**
 * @brief Renders RayMarching's scene in a ImGUI window
 */
class MenuWidget: public WidgetBase
{
    public:
        MenuWidget() = default;
        explicit MenuWidget(WidgetManager& windowManager);

        auto RenderContent() -> void override;
    private:
        auto LoadSDFWidget() -> void;
        auto Load(std::string path) -> void;
        WidgetManager& windowManager;
        SDFLibrary library;
};

}

#endif
