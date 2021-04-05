#include "WidgetBase.hpp"

using namespace raymarcher;

WidgetBase::WidgetBase()
{
    static WidgetID sessionUniqueWidgetID = 0;
    id = sessionUniqueWidgetID++;
}

auto WidgetBase::Render() -> void
{
    RenderContent();
}

auto WidgetBase::RenderContent() -> void
{
}
