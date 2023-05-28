#pragma once

#include <engine/Core.h>

namespace game
{
	struct AnimationSystem : public s2de::System
	{
		void onUpdate(double dt, flecs::world& world) override;

		static sf::Clock lifetime_clock;
	};
}