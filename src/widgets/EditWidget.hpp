#ifndef RAYMARCHER_EDITWIDGET_HPP
#define RAYMARCHER_EDITWIDGET_HPP

#include "widgets/WindowWidget.hpp"

namespace raymarcher 
{
// Fwd decl
class Raymarcher;
/**
 * @brief Allows editing, compiling, previewing and saving of SDF function source code.
 *
 * An edit widget serves for prototyping and editing SDF source codes.
 * The source code can be compiled and associated window with scene's preview is opened.
 */
class EditWidget: public WindowWidget
{
    public:
    EditWidget() = default;
    EditWidget(std::string name, std::string startingCode, std::string file = "");

    /// Render buttons, text edit, etc.
    auto RenderContent() -> void override;

    /// Force recompilation of SDF and update of preview window
    auto Recompile() -> void;

    protected:
    /// Save current code to 'filename'
    auto SaveAs(std::string filename) -> void;

    /// Helper: set current state as error with 'error' message
    auto SetLastError(std::string error) -> void;
    
    /// Helper: set current state as info with 'msg' message
    auto SetLastStatus(std::string msg) -> void;

    /// Create preview window for current code
    auto CreateRenderWidget(std::shared_ptr<raymarcher::Raymarcher> rm) -> std::shared_ptr<WidgetBase>;

    /// Construct Save as pop-up widget
    auto SaveAsSubwidget() -> void;

    private:
    /// SDF source code
    std::string code;

    /// Last path, determined using Save as widget, or empty
    std::string filename;

    /// Is auto-compile enabled in GUI
    bool isAutocompileEnabled;

    /// Status message for user
    struct Status 
    {
        bool isError;
        std::string lastMessage;
    } status;

    // Instance of compiled renderer
    std::shared_ptr<Raymarcher> rm;
};

}

#endif
