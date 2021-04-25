#ifndef RAYMARCHER_EDITWIDGET_HPP
#define RAYMARCHER_EDITWIDGET_HPP

#include "WindowWidget.hpp"

namespace raymarcher 
{
// Fwd decl
class Raymarcher;
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
    auto Recompile() -> void;
    std::string code;

    std::string lastError;

    std::shared_ptr<Raymarcher> rm;
};

}

#endif
