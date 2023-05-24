#pragma once

#include <bitset>
#include <engine/Core.h>

namespace game
{
    class Player :
        public s2de::components::InputObject,
        public s2de::components::ScriptableObject
    {
        std::bitset<4> _input;
        flecs::entity  _entity;

    public:
        Player(const flecs::world& world);

        void onUpdate(double dt, flecs::world& world) override;
        void onEvent(const sf::Event& event) override;

        const flecs::entity& getEntity() const { return _entity; }
    };
}