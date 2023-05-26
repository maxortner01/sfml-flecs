#pragma once

#include <engine/Core.h>

namespace game
{
	struct EnemySystem : public s2de::System
	{
		void onRender(sf::RenderTarget&, flecs::world&) override;
		void onUpdate(double dt, flecs::world& world) override;

	private:
		sf::Clock _time;
	};
}