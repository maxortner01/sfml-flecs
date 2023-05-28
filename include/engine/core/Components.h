#pragma once

#include <SFML/Graphics.hpp>
#include <flecs.h>

namespace s2de::components
{
    /* Tags */
    enum class Player {};
    enum class Tile {};
    enum class Bullet {};
    enum class Gravity {};

    enum EnemyType : uint8_t
    {
        Floaty
    };

    struct Enemy 
    {
        EnemyType type;
    };

    struct Transform
    {
        sf::Vector3f position;
        sf::Vector2f scale;
        sf::Angle    rotation;
    };
    
    struct Velocity
    {
        sf::Vector3f vector;
    };

    struct HP
    {
        float value;
        float max;
    };

    struct Force
    {
        sf::Vector3f direction;
        float m;
        float k;
        float max_velocity;
    };

    enum class AnimStateCode : uint32_t
    {
        None,
        Hurt,
        Dying,
        Dead
    };

    struct AnimationState
    {
        AnimStateCode state_code;
        double        state_start_time;
    };

    struct Sprite
    {
        sf::Texture* texture;
        sf::IntRect  rectangle;
        uint32_t     frames;
        double frame_time;
        sf::Vector2f offset;
        sf::Color    color;
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