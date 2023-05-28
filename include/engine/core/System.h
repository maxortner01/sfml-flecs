#pragma once

#include <SFML/Graphics.hpp>
#include <flecs.h>

namespace s2de
{
    struct System
    {
        virtual ~System() = default;
        virtual void onEvent(const sf::Event& event) { }
        virtual void onRender(Scene&, flecs::world&) { }
        virtual void onUpdate(double, flecs::world&) = 0;
    };
}