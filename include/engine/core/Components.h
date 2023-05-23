#pragma once

#include <SFML/Graphics.hpp>
#include <flecs.h>

namespace s2de::components
{
    struct Transform
    {
        sf::Vector2f position;
        sf::Vector2f scale;
        sf::Angle    rotation;
    };
    
    struct Velocity
    {
        sf::Vector2f vector;
    };

    struct hp
    {
        float value;
        float max;
        float offset;
    };

    struct Force
    {
        sf::Vector2f direction;
        float m;
        float k;
        float max_velocity;
    };

    struct Sprite
    {
        sf::Texture* texture;
        sf::IntRect  rectangle;
        uint32_t     frames;
        double frame_time;
    };

    struct Rectangle
    {
        sf::Vector2f dimensions;
        sf::Color color;
    };

    struct ScriptableObject
    {
        virtual void onRender(sf::RenderTarget&) { };
        virtual void onUpdate(double, flecs::world&) { };
    };

    struct InputObject
    {
        virtual void onEvent(const sf::Event& event) = 0;
    };

    struct Scriptable
    {
        ScriptableObject* object;
    };

    struct Input
    {
        InputObject* object;
    };
}