#ifndef RAYMARCHER_MENUWIDGET_HPP
#define RAYMARCHER_MENUWIDGET_HPP

#include "WidgetBase.hpp"
#include "WidgetManager.hpp"

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
        WidgetManager& windowManager;
};

}

#endif
