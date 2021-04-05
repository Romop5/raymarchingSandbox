/**
 * @brief Thread-nonsafe implementation of Observer pattern
 */
#ifndef RAYMARCHER_OBSERVER_HPP
#define RAYMARCHER_OBSERVER_HPP

#include <memory>
#include <vector>
#include <functional>

namespace raymarcher
{

template<typename T>
class Callback;

template<typename T>
class Observee;

template<typename T>
class Observer
{    
    public:
    Observer() = default;
    ~Observer();
    Observer(const Observer& o);
    Observer(Observer&& o);

    Observer& operator=(const Observer& o);
    Observer& operator=(Observer&& o);

    friend class Observee<T>;
    private:
    std::shared_ptr<Callback<T>> callback;
};

template<typename T>
class Observee: public std::enable_shared_from_this<Observee<T>>
{
    public:
    Observee();
    ~Observee();

    Observee(const Observee& o);
    Observee(Observee&& o);

    Observee& operator=(const Observee& o);
    Observee& operator=(Observee&& o);

    auto Register(std::function<void(T)> functor) -> Observer<T>;
    void Call(T a);

    friend class Callback<T>;
    private:
    auto Unregister(std::shared_ptr<Callback<T>> callback) -> void;
    std::vector<std::shared_ptr<Callback<T>>> callbacks;
};

template<typename T>
class Callback: public std::enable_shared_from_this<Callback<T>>
{
    public:
    Callback();
    ~Callback();

    Callback(const Callback& o);
    Callback(Callback&& o);
    Callback& operator=(const Callback& o);
    Callback& operator=(Callback&& o);

    friend class Observee<T>;
    friend class Observer<T>;
    private:
    auto Unregister() -> void;
    std::shared_ptr<Observee<T>> observee;
    std::function<void(T)> callback;
};


//-----------------------------------------------------------------------------
template<typename T>
Observer<T>::~Observer<T>()
{
    callback->Unregister();
    callback.reset();
}

template<typename T>
Observer<T>::Observer(const Observer<T>& o)
{
    this->callback = o.callback;
}

template<typename T>
Observer<T>::Observer(Observer<T>&& o)
{
    std::swap(this->callback, o.callback);
}

template<typename T>
Observer<T>& Observer<T>::operator=(const Observer<T>& o)
{
    this->callback = o.callback;
    return *this;
}

template<typename T>
Observer<T>& Observer<T>::operator=(Observer<T>&& o)
{
    std::swap(this->callback, o.callback);
    return *this;
}

//-----------------------------------------------------------------------------
template<typename T>
Observee<T>::Observee()
{
}

template<typename T>
Observee<T>::~Observee()
{
    for(auto& observer: callbacks)
    {
        Unregister(observer);
    }
}

template<typename T>
Observee<T>::Observee(const Observee<T>& o)
{
    this->callbacks = o.callbacks;
}

template<typename T>
Observee<T>::Observee(Observee<T>&& o)
{
    std::swap(this->callbacks, o.callbacks);
}


template<typename T>
Observee<T>& Observee<T>::operator=(const Observee<T>& o)
{
    this->callbacks = o.callbacks;
    return *this;
}

template<typename T>
Observee<T>& Observee<T>::operator=(Observee<T>&& o)
{
    std::swap(this->callbacks, o.callbacks);
    return *this;
}


template<typename T>
auto Observee<T>::Register(std::function<void(T)> functor) -> Observer<T>
{
    auto callback = std::make_shared<Callback<T>>();
    callback->observee = std::enable_shared_from_this<Observee<T>>::shared_from_this();
    callback->callback = std::move(functor);

    callbacks.push_back(callback);

    Observer<T> observer;
    observer.callback = callback;
    return observer;
}

template<typename T>
auto Observee<T>::Unregister(std::shared_ptr<Callback<T>> callback) -> void
{
    callback->observee.reset();

    for(auto it = callbacks.begin(); it != callbacks.end(); it++)
    {
        if(*it == callback)
        {
            callbacks.erase(it);
            return;
        }
    }
}

template<typename T>
void Observee<T>::Call(T a)
{
    for(auto& observer: callbacks)
    {
        observer->callback(a);
    }
}

//-----------------------------------------------------------------------------
template<typename T>
Callback<T>::Callback()
{
}

template<typename T>
Callback<T>::~Callback()
{
    Unregister(); 
}


template<typename T>
Callback<T>::Callback(const Callback<T>& o)
{
    this->observee = o.observee;
    this->callback = o.callback;
}

template<typename T>
Callback<T>::Callback(Callback<T>&& o)
{
    std::swap(this->observee, o.observee);
    std::swap(this->callback, o.callback);
}

template<typename T>
Callback<T>& Callback<T>::operator=(const Callback<T>& o)
{
    this->observee = o.observee;
    this->callback = o.callback;
    return *this;
}

template<typename T>
Callback<T>& Callback<T>::operator=(Callback<T>&& o)
{
    std::swap(this->observee, o.observee);
    std::swap(this->callback, o.callback);
    return *this;
}

template<typename T>
auto Callback<T>::Unregister() -> void
{
    if(observee)
    {
        
        observee->Unregister(std::enable_shared_from_this<Callback<T>>::shared_from_this());
    }
}
}
#endif
