#ifndef RAYMARCHER_MENUWIDGET_HPP
#define RAYMARCHER_MENUWIDGET_HPP

#include "application/IApplication.hpp"
#include "raymarching/SDFLibrary.hpp"
#include "widgets/FPSMeter.hpp"
#include "widgets/LoadFileWidget.hpp"
#include "widgets/WidgetBase.hpp"

#include "WidgetManager.hpp"

namespace raymarcher {
/**
 * @brief Provides buttons to access functionality of Sandbox
 */
class MenuWidget : public WidgetBase
{
public:
  MenuWidget() = default;
  explicit MenuWidget(IApplication& application, WidgetManager& windowManager);

  auto RenderContent() -> void override;

private:
  auto RenderContentWidget(bool is_menu) -> void;
  auto LoadSDFWidget() -> void;
  auto Load(std::string path) -> void;

  IApplication& application;
  WidgetManager& windowManager;
  SDFLibrary library;

  bool showFPS;
  FPSMeter fpsMeter;
};

}

#endif
