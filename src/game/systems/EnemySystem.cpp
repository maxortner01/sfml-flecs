#include <game/game.h>

#include <iostream>

namespace game
{
	EnemySystem::EnemySystem(sf::Texture* bullet) :
		_bullet(bullet)
	{	}

	void EnemySystem::onRender(s2de::Scene& scene, flecs::world& world)
	{
		using namespace s2de::components;

		auto enemies = world.filter<const Enemy, const Sprite, const Transform>();

		enemies.each([&scene](const Enemy& enemy, const Sprite& sprite, const Transform& transform)
			{
				switch (enemy.type)
				{
				case EnemyType::Floaty: 
				{
					sf::CircleShape circle;
					circle.setRadius(12.f);
					circle.setPosition(scene.toWorld(sf::Vector3f(transform.position.x, transform.position.y, 0)));
					circle.setOrigin(circle.getGlobalBounds().getSize() / 2.f);
					circle.setScale(sf::Vector2f(0.8, 0.45));
					circle.setFillColor(sf::Color(33, 30, 32, 107));
					scene.getRenderTexture().draw(circle);
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
				const sf::Vector3f dir_unnormalized = (player_position - transform.position);
				const float distance = dir_unnormalized.length();
				const sf::Vector3f dir = dir_unnormalized / distance;

				if (distance >= 2.f)
				{
					force.direction.x = dir.x * 2.f;
					force.direction.y = dir.y * 2.f;
				}
				else
				{
					force.direction.x = sf::Vector2f(dir.x, dir.y).rotatedBy(sf::degrees(110)).x * 1.f;
					force.direction.y = sf::Vector2f(dir.x, dir.y).rotatedBy(sf::degrees(110)).y * 1.f;
				}
					
				enemies.each([&e1, &transform, &force](flecs::entity e2, const Enemy& enemy, const Transform& transform2, const Sprite& sprite2, Force& force2)
					{
						if (e1 == e2) return;

						const float distance = (transform2.position - transform.position).length();
						const float mag = 0.05f / (distance * distance);
						const sf::Vector3f dir = (transform.position - transform2.position) / distance;

						force.direction.x += dir.x * mag;
						force.direction.y += dir.y * mag;
					});

				if (enemy.type == EnemyType::Floaty)
				{
					const float dist = (
							sf::Vector2f(player_position.x, player_position.y) - sf::Vector2f(transform.position.x, transform.position.y)
						).length();
					const float g = 9.8f * 0.5f;
					const float v0 = 3.5f;
					const float vx = 1.1f * g * dist / (v0 + sqrt(v0 * v0 + 4 * g * transform.position.z));
					sf::Vector3f vel = dir * vx;
					vel.z = v0;

					// shoot?
					srand(e1.id());
					double val = fmod(_time.getElapsedTime().asSeconds(), (rand() % 1000) / 1000.f);
					if (val < 0.005)
					{
						world.entity()
							.set(Transform{
									.position = transform.position,
									.scale    = sf::Vector2f(1, 1),
									.rotation = sf::radians(0)
								})
							.set(Velocity{
									.vector = vel
								})
							.set(Sprite{
									.texture = _bullet,
									.rectangle = sf::IntRect({ 0, 0 }, { 8, 8 }),
									.frames = 1,
									.frame_time = 1.f,
									.offset = sprite.offset
								})
							.add<Bullet>()
							.add<Gravity>();
					}
				}
			});
	}
}