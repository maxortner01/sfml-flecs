#pragma once

namespace s2de::utility
{
    template<typename _StateEnum>
    class State
    {   
        _StateEnum _state;

    protected:
        void setState(const _StateEnum& new_state) { _state = new_state; }

    public:
        State(const _StateEnum& initial_state = (_StateEnum)0) :
            _state(initial_state)
        {   }

        _StateEnum getState() const { return _state; }
    };
}