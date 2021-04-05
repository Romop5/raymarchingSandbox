#include "WidgetBase.hpp"

#include "observer.hpp"

using namespace raymarcher;

class WidgetBase::Impl
{
    public:
    Impl();
    auto GetID() -> WidgetID;
    auto GetOnExitEventRegister() -> Observee<bool>&;
    private:
    WidgetID id;
    raymarcher::Observee<bool> onExitEvent;
};

WidgetBase::Impl::Impl()
{
    static WidgetID sessionUniqueWidgetID = InvalidID+1;
    id = sessionUniqueWidgetID++;
}


auto WidgetBase::Impl::GetID() -> WidgetID
{
    return id;
}


Observee<bool>& WidgetBase::Impl::GetOnExitEventRegister()
{
    return onExitEvent;
}

//-----------------------------------------------------------------------------
WidgetBase::WidgetBase() :
    pimpl { std::make_unique<Impl>() }
{
}

WidgetBase::~WidgetBase() = default;

auto WidgetBase::GetID() -> WidgetID
{
    return pimpl->GetID();
}

auto WidgetBase::GetOnExitEventRegister() -> Observee<bool>&
{
    return pimpl->GetOnExitEventRegister();
}

auto WidgetBase::Render() -> void
{
    RenderContent();
}

auto WidgetBase::RenderContent() -> void
{
}
