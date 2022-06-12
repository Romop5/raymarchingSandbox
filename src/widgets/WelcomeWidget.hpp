#ifndef RAYMARCHER_WELCOMEWIDGET_HPP
#define RAYMARCHER_WELCOMEWIDGET_HPP

#include "widgets/WindowWidget.hpp"

namespace raymarcher {
/**
 * @brief Provides buttons to access functionality of Sandbox
 */
class WelcomeWidget : public WindowWidget
{
public:
  WelcomeWidget() = default;
  explicit WelcomeWidget(WidgetManager& windowManager);

  auto RenderContent() -> void override;

private:
};

}
#endif
