#ifndef RAYMARCHER_WIDGETBASE_HPP
#define RAYMARCHER_WIDGETBASE_HPP

#include <memory>

namespace raymarcher
{

template<typename T>
class Observee;

class WidgetBase
{
    public:
    class Impl;

    WidgetBase();
    virtual ~WidgetBase();

    using WidgetID = unsigned long;
    static constexpr auto InvalidID = 0;
    auto GetID() -> WidgetID;
    auto GetOnExitEventRegister() -> Observee<bool>&;
    virtual auto Render() -> void;

    protected:
    virtual auto RenderContent() -> void;

    virtual auto Delete() -> void;

    private:
    std::unique_ptr<Impl> pimpl;
    WidgetID id;
};
}
#endif
