#ifndef RAYMARCHER_WIDGET_MANAGER_HPP
#define RAYMARCHER_WIDGET_MANAGER_HPP

#include <memory>
#include <vector>
#include "IWidget.hpp"

namespace raymarcher 
{
/**
 * @brief Manages life time / updating / rendering of the contained widgets
 */
class WidgetManager 
{
    public:
        WidgetManager() = default;
        auto AddWidget(std::shared_ptr<raymarcher::IWidget> newWidget) -> void;
        auto Render() -> void;
    private:
        std::vector<std::shared_ptr<IWidget>> widgets;
};

}

#endif
