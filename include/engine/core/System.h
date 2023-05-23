#pragma once

#include <SFML/Graphics.hpp>
#include <flecs.h>

namespace s2de
{
    struct System
    {
        virtual void onRender(sf::RenderTarget&, flecs::world&) { }
        virtual void onUpdate(double, flecs::world&) = 0;
    };
}