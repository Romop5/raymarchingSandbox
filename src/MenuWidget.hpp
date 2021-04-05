#ifndef RAYMARCHER_MENUWIDGET_HPP
#define RAYMARCHER_MENUWIDGET_HPP

#include "IWidget.hpp"
#include "WidgetManager.hpp"

namespace raymarcher 
{
/**
 * @brief Renders RayMarching's scene in a ImGUI window
 */
class MenuWidget: public IWidget
{
    public:
        MenuWidget() = default;
        explicit MenuWidget(WidgetManager& windowManager);

        auto Render() -> void override;
    private:
        WidgetManager& windowManager;
};

}

#endif
