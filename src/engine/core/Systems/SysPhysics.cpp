#include <engine/Core.h>

#include <iostream>

namespace s2de
{
    void SysPhysics::onUpdate(double dt, flecs::world& world) 
    {
        using namespace s2de::components;

        std::vector<flecs::entity> es;
        auto grav = world.filter<const Gravity, const Sprite, Velocity>();
        grav.each([dt, &es](const Gravity g, const Sprite& sprite, Velocity& vel)
            {
                vel.vector -= sf::Vector3f(0, 0, 9.8) * (float)dt * 0.5f;
            });

        auto f = world.filter<Force, Transform, Velocity>();
        f.each([dt](Force& force, Transform& transform, Velocity& velocity) 
        {
            sf::Vector3f a = (force.direction - 3.f * force.k * velocity.vector) / force.m;
            velocity.vector += sf::Vector3f(a.x, a.y, 0) * (float)dt;
        });

        auto vel_transform = world.filter<Transform, Velocity>();
        vel_transform.each([dt](Transform& transform, Velocity& velocity) 
        {
            transform.position += velocity.vector * (float)dt;
        });
    }
}