#ifndef RAYMARCHER_WIDGET_MANAGER_HPP
#define RAYMARCHER_WIDGET_MANAGER_HPP

#include <memory>
#include <vector>
#include "widgets/WidgetBase.hpp"

namespace raymarcher 
{

template<typename T>
class Observer;

/**
 * @brief Manages life time / updating / rendering of the contained widgets
 */
class WidgetManager 
{
    public:
        WidgetManager();
        ~WidgetManager();

        struct WidgetEntry
        {
            std::shared_ptr<WidgetBase>     widget;
            std::unique_ptr<Observer<bool>> onExitObserver;
        };
        using WidgetStorageType = std::vector<WidgetEntry>;

        enum LayoutType
        {
            VERTICAL_BOX,
            HORIZONTAL_BOX
        };

        auto AddWidget(std::shared_ptr<raymarcher::WidgetBase> newWidget) -> void;
        auto HasWidget(WidgetBase::WidgetID id) -> bool;
        auto RemoveWidget(WidgetBase::WidgetID id) -> void;
        auto RemoveAllWidgets() -> void;
        auto Render() -> void;
        auto SetLayout(LayoutType type) -> void;
        auto HasAnyWidget() const -> bool;
    private:
        WidgetStorageType widgets;
        LayoutType layout;
};

}

#endif
