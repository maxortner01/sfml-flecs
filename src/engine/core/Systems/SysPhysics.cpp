#include <engine/Core.h>

#include <iostream>

namespace s2de
{
    void SysPhysics::onUpdate(double dt, flecs::world& world) 
    {
        using namespace s2de::components;

        auto f = world.filter<Force, Transform, Velocity>();
        f.each([dt](Force& force, Transform& transform, Velocity& velocity) 
        {
            sf::Vector2f a = (force.direction - 3.f * force.k * velocity.vector) / force.m;
            velocity.vector += a * (float)dt;
        });

        auto vel_transform = world.filter<Transform, Velocity>();
        vel_transform.each([dt](Transform& transform, Velocity& velocity) 
        {
            transform.position += velocity.vector * (float)dt;
        });
    }
}