#pragma once

#include <engine/Core.h>

namespace game
{
	struct BattleSystem : public s2de::System
	{
		BattleSystem();

		void onRender(s2de::Scene& scene, flecs::world& world) override;
		void onUpdate(double dt, flecs::world& world) override;

	public:
		sf::Vector2f _mouse_pos;
		std::vector<sf::Vector3f> _attack_bounds;
	};
}
