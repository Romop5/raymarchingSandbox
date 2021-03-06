#ifndef RAYMARCHER_BVH_CALCULATOR_WIDGET_HPP
#define RAYMARCHER_BVH_CALCULATOR_WIDGET_HPP

#include <memory>

#include "raymarching/Raymarcher.hpp"
#include "widgets/WindowWidget.hpp"

namespace raymarcher {
class SpherePrimitive;
class BVHLibrary;

/**
 * @brief GUI control over BVHLibrary
 */
class BVHCalculatorWidget : public WindowWidget
{
public:
  BVHCalculatorWidget();
  ~BVHCalculatorWidget();

  auto RenderContent() -> void override;
  auto Optimize() -> void;
  auto GenerateGeometry() -> void;

private:
  auto DisplayElement(SpherePrimitive&, size_t level) -> void;
  std::unique_ptr<BVHLibrary> pimpl;
};

}

#endif
