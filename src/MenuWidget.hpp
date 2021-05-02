#ifndef RAYMARCHER_MENUWIDGET_HPP
#define RAYMARCHER_MENUWIDGET_HPP

#include "WidgetBase.hpp"
#include "WidgetManager.hpp"
#include "SDFLibrary.hpp"
#include "FPSMeter.hpp"

namespace raymarcher 
{
/**
 * @brief Provides buttons to access functionality of Sandbox
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

        bool showFPS;
        FPSMeter fpsMeter;
};

}

#endif
