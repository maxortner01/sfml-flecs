#pragma once

#include "../System.h"

namespace s2de
{
    struct SysPhysics : public System
    {
        void onUpdate(double, flecs::world&) override;
    };
} 
