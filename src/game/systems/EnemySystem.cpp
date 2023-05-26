#include <game/game.h>

#include <iostream>

namespace game
{
	EnemySystem::EnemySystem(sf::Texture* bullet) :
		_bullet(bullet)
	{	}

	void EnemySystem::onRender(sf::RenderTarget& target, flecs::world& world)
	{
		using namespace s2de::components;

		auto enemies = world.filter<const Enemy, const Sprite, const Transform>();

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

		auto enemies = world.filter<const Enemy, const Transform, const Sprite, Force>();
		enemies.each([&player_position, &dt, &enemies, this, &world](flecs::entity e1, const Enemy& enemy, const Transform& transform, const Sprite& sprite, Force& force)
			{
				const sf::Vector2f dir_unnormalized = (player_position - transform.position - sprite.offset);
				const float distance = dir_unnormalized.length();
				const sf::Vector2f dir = dir_unnormalized / distance;

				if (distance >= 70.f)
					force.direction = dir * 500.f;
				else
					force.direction = dir.rotatedBy(sf::degrees(110)) * 250.f ;

				enemies.each([&e1, &transform, &force](flecs::entity e2, const Enemy& enemy, const Transform& transform2, const Sprite& sprite2, Force& force2)
					{
						if (e1 == e2) return;

						const float distance = (transform2.position - transform.position).length();
						const float mag = 5200.f / (distance * distance);
						const sf::Vector2f dir = (transform.position - transform2.position) / distance;
						force.direction += dir * mag;
					});

				/*
				if (enemy.type == EnemyType::Floaty)
				{
					// shoot?
					srand(e1.id());
					double val = fmod(_time.getElapsedTime().asSeconds(), (rand() % 1000) / 1000.f);
					if (val < 0.1)
					{
						world.entity()
							.set(Transform{
									.position = transform.position,
									.scale    = sf::Vector2f(0.9, 0.9),
									.rotation = sf::radians(0)
								})
							.set(Velocity{
									.vector = dir.rotatedBy(sf::radians(distance)) * 75.f
								})
							.set(Sprite{
									.texture = _bullet,
									.rectangle = sf::IntRect({ 0, 0 }, { 8, 8 }),
									.frames = 1,
									.frame_time = 1.f,
									.offset = sprite.offset
								})
							.set(e1.get<MapCoordinates>())
							.add<Bullet>()
							.add<Gravity>();
					}
				}*/
			});
	}
}