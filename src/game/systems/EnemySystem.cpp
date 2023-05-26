#include <game/game.h>

#include <iostream>

namespace game
{
	void EnemySystem::onRender(sf::RenderTarget& target, flecs::world& world)
	{
		using namespace s2de::components;

		auto enemies = world.filter<const Enemy, const Sprite, const Transform>();
		std::cout << enemies.count() << "\n";

		enemies.each([&target](const Enemy& enemy, const Sprite& sprite, const Transform& transform)
			{
				switch (enemy.type)
				{
				case EnemyType::Floaty: 
				{
					sf::CircleShape circle;
					circle.setRadius(12.f);
					circle.setPosition(transform.position);
					circle.setOrigin(circle.getGlobalBounds().getSize() / 2.f);
					circle.setScale(sf::Vector2f(0.8, 0.45));
					circle.setFillColor(sf::Color(33, 30, 32, 107));
					target.draw(circle);
				}
				}
			});
	}

	void EnemySystem::onUpdate(double dt, flecs::world& world)
	{
		using namespace s2de::components;

		auto player = world.filter<s2de::components::Player, const Transform>().first();
		assert(player);

		auto player_position = player.get<const Transform>()->position;

		auto enemies = world.filter<const Enemy, const Transform, Force>();
		enemies.each([&player_position, &dt, &enemies](flecs::entity e1, const Enemy& enemy, const Transform& transform, Force& force)
			{
				const float distance = (player_position - transform.position).length();
				const sf::Vector2f dir = (player_position - transform.position) / distance;

				if (distance >= 70.f)
					force.direction = dir * 500.f;
				else
					force.direction = dir.rotatedBy(sf::degrees(110)) * 250.f ;

				enemies.each([&e1, &transform, &force](flecs::entity e2, const Enemy& enemy, const Transform& transform2, Force& force2)
					{
						if (e1 == e2) return;

						const float distance = (transform2.position - transform.position).length();
						const float mag = 5200.f / (distance * distance);
						const sf::Vector2f dir = (transform.position - transform2.position) / distance;
						force.direction += dir * mag;
					});
			});
	}
}