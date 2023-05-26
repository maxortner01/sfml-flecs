#pragma once

#include <engine/Core.h>

namespace game
{
	struct EnemySystem : public s2de::System
	{
		EnemySystem(sf::Texture* bullet);

		void onRender(sf::RenderTarget&, flecs::world&) override;
		void onUpdate(double dt, flecs::world& world) override;

	private:
		sf::Clock _time;
		sf::Texture* _bullet;
	};
}