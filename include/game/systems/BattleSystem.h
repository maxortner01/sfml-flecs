#pragma once

#include <engine/Core.h>

namespace game
{
	struct BattleSystem : public s2de::System
	{
		BattleSystem();

		void onEvent(const sf::Event& event) override;
		void onRender(s2de::Scene& scene, flecs::world& world) override;
		void onUpdate(double dt, flecs::world& world) override;

	public:
		sf::Clock lifetime_clock;
		sf::Clock attack_timer;
		bool _mouse_click = false;
		sf::Vector2f _mouse_pos;
		std::vector<sf::Vector3f> _attack_bounds;
	};
}
