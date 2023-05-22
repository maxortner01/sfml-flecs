#pragma once

#include <cassert>

namespace s2de::utility
{
    template<typename _Type>
    class Singleton
    {
        inline static _Type* _object = nullptr;

    public:
        inline static _Type& get();
        inline static void destroy();
    };

    template<typename _Type>
    _Type& Singleton<_Type>::get()
    {
        if (!_object) _object = new _Type();
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