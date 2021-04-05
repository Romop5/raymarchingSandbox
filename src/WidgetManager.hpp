#ifndef RAYMARCHER_WIDGET_MANAGER_HPP
#define RAYMARCHER_WIDGET_MANAGER_HPP

#include <memory>
#include <vector>
#include "WidgetBase.hpp"

namespace raymarcher 
{
/**
 * @brief Manages life time / updating / rendering of the contained widgets
 */
class WidgetManager 
{
    public:
        WidgetManager() = default;

        using WidgetStorageType = std::vector<std::shared_ptr<WidgetBase>>;

        enum LayoutType
        {
            VERTICAL_BOX,
            HORIZONTAL_BOX
        };

        auto AddWidget(std::shared_ptr<raymarcher::WidgetBase> newWidget) -> void;
        auto Render() -> void;
        auto SetLayout(LayoutType type) -> void;
    private:
        WidgetStorageType widgets;
        LayoutType layout;
};

}

#endif
