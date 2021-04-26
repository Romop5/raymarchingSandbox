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
    auto SaveAs(std::string filename) -> void;
    auto SaveAsSubwidget() -> void;
    auto SetLastError(std::string error) -> void;
    auto SetLastStatus(std::string msg) -> void;
    std::string code;
    std::string filename;

    bool isStatusError;
    std::string lastStatus;

    std::shared_ptr<Raymarcher> rm;
};

}

#endif
