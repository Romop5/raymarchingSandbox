#ifndef RAYMARCHER_BVH_CALCULATOR_WIDGET_HPP
#define RAYMARCHER_BVH_CALCULATOR_WIDGET_HPP

#include "Raymarcher.hpp"
#include "WindowWidget.hpp"
#include <memory>

namespace raymarcher
{
class SpherePrimitive;
class BVHCalculatorPimpl;

class BVHCalculator : public WindowWidget
{
    public:
        BVHCalculator();
        ~BVHCalculator();

        auto RenderContent() -> void override;

        struct OptimizationParameters
        {
            size_t maxLevel = 1;
        };
        auto Generate(const OptimizationParameters params) -> void;
    private:
        auto DisplayElement(SpherePrimitive&, size_t level) -> void;
        std::unique_ptr<BVHCalculatorPimpl> pimpl;

       OptimizationParameters params;
};

}

#endif
