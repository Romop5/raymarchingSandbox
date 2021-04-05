#ifndef RAYMARCHER_EDITWIDGET_HPP
#define RAYMARCHER_EDITWIDGET_HPP

#include "WindowWidget.hpp"

namespace raymarcher 
{
/**
 * @brief Renders RayMarching's scene in a ImGUI window
 */
class EditWidget: public IWidget
{
    public:
        EditWidget() = default;
        explicit EditWidget(std::string startingCode);

        auto Render() -> void override;
    private:
    std::string code;

    std::shared_ptr<IWidget> previewWidget;

};

}

#endif
