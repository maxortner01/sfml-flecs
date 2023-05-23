#pragma once

#include <cassert>

namespace s2de::utility
{
    template<typename _Type>
    class Singleton
    {
        inline static _Type* _object = nullptr;

    public:
        template<typename... Args>
        inline static void create(Args&&... args);
        inline static _Type& get();
        inline static void destroy();
    };

    template<typename _Type>
    template<typename... Args>
    void Singleton<_Type>::create(Args&&... args)
    {
        if (_object) destroy();
        _object = new _Type(args...);
    }

    template<typename _Type>
    _Type& Singleton<_Type>::get()
    {
        assert(_object);
        return *_object;
    }

    template<typename _Type>
    void Singleton<_Type>::destroy()
    {
        assert(_object);
        delete _object;
        _object = nullptr;
    }
}