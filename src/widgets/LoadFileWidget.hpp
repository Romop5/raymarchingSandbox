#ifndef RAYMARCHER_LOADFILEWIDGET_HPP
#define RAYMARCHER_LOADFILEWIDGET_HPP

#include "widgets/WindowWidget.hpp"

namespace raymarcher {
/**
 * @brief Provides buttons to access functionality of Sandbox
 */
class LoadFileWidget : public WindowWidget
{
public:
  LoadFileWidget() = default;
  explicit LoadFileWidget(WidgetManager& windowManager);

  auto RenderContent() -> void override;

protected:
  auto LoadFile(std::string path) -> void;

private:
  WidgetManager& windowManager_;
};

}
#endif
