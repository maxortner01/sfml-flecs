#include <game/game.h>
#include <iostream>

using namespace s2de;

namespace game
{
    Player::Player(const flecs::world& world) :
        _entity(world.entity())
    {
        _entity.set(components::Transform {
            .position = sf::Vector2f(0, 0),
            .scale = sf::Vector2f(0.2, 0.2),
            .rotation = sf::radians(0)
        })
        .set(components::Force {
            .k = 6.5f,
            .m = 1.f,
            .max_velocity = 650.f,
            .direction = sf::Vector2f(0, 0)
        })
        .set(components::Velocity {
            .vector = sf::Vector2f(0, 0)
        })
        .set(components::Rectangle {
            .color = sf::Color::White,
            .dimensions = sf::Vector2f(32, 32)
        })
        .set(components::Input {
            .object = static_cast<components::InputObject*>(this)
        })
        .set(components::Scriptable {
            .object = static_cast<components::ScriptableObject*>(this)
        })
        .set(components::Depth {
            .z = 0.f
        })
        .add<components::Player>();
    }

    void Player::onUpdate(double dt, flecs::world& world)
    {
        auto position  = _entity.get<components::Transform>()->position;
        components::Force* force = _entity.get_mut<components::Force>();
        
        force->direction = sf::Vector2f(0, 0);
        sf::Vector2f new_force;
        const float mag = force->max_velocity * force->k;
        if (_input[0]) new_force.y -= mag;
        if (_input[1]) new_force.y += mag;
        if (_input[2]) new_force.x -= mag;
        if (_input[3]) new_force.x += mag;
        if (new_force.length()) new_force = new_force.normalized() * mag;

        force->direction = new_force;
    }

    void Player::onEvent(const sf::Event& event) 
    {
        if (event.type == sf::Event::KeyPressed)
        {
            switch (event.key.code)
            {
            case sf::Keyboard::W: _input[0] = 1; break;
            case sf::Keyboard::S: _input[1] = 1; break;
            case sf::Keyboard::A: _input[2] = 1; break;
            case sf::Keyboard::D: _input[3] = 1; break;
            default: break;
            }
        }
        else if (event.type == sf::Event::KeyReleased)
        {
            switch (event.key.code)
            {
            case sf::Keyboard::W: _input[0] = 0; break;
            case sf::Keyboard::S: _input[1] = 0; break;
            case sf::Keyboard::A: _input[2] = 0; break;
            case sf::Keyboard::D: _input[3] = 0; break;
            default: break;
            }
        }
    }
}
