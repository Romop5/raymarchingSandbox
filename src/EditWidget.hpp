#ifndef RAYMARCHER_EDITWIDGET_HPP
#define RAYMARCHER_EDITWIDGET_HPP

#include "WindowWidget.hpp"

namespace raymarcher 
{
/**
 * @brief Renders RayMarching's scene in a ImGUI window
 */
class EditWidget: public WindowWidget
{
    public:
        EditWidget() = default;
        EditWidget(std::string name, std::string startingCode);

        auto Render() -> void override;
        auto RenderContent() -> void override;
    private:
    std::string code;

    //std::shared_ptr<WidgetBase> previewWidget;
};

}

#endif
