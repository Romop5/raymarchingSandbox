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
    EditWidget(std::string name, std::string startingCode, std::string file = "");

    auto Render() -> void override;
    auto RenderContent() -> void override;
    auto Recompile() -> void;
    private:
    auto SaveAs(std::string filename) -> void;
    auto SaveAsSubwidget() -> void;
    auto SetLastError(std::string error) -> void;
    auto SetLastStatus(std::string msg) -> void;

    auto CreateRenderWidget(std::shared_ptr<raymarcher::Raymarcher> rm) -> std::shared_ptr<WidgetBase>;
    std::string code;
    std::string filename;

    bool isAutocompileEnabled;

    bool isStatusError;
    std::string lastStatus;

    std::shared_ptr<Raymarcher> rm;
};

}

#endif
