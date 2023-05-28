#include <game/game.h>

namespace game
{
	sf::Clock AnimationSystem::lifetime_clock = sf::Clock();

	template<typename _Type>
	_Type lerp(float t, _Type start, _Type end)
	{
		return start + (end - start) * t;
	}

	sf::Color vec3ToColor(const sf::Vector3f& v)
	{
		return sf::Color(v.x * 255U, v.x * 255U, v.z * 255U, 255U);
	}

	void AnimationSystem::onUpdate(double dt, flecs::world& world)
	{
		using namespace s2de::components;

		auto animations = world.filter<Sprite, AnimationState>();
		animations.each([this](Sprite& sprite, AnimationState& state)
			{
				const auto rel_time = AnimationSystem::lifetime_clock.getElapsedTime().asSeconds() - state.state_start_time;

				switch (state.state_code)
				{
					case AnimStateCode::Hurt:
					{
						/* Linearly interolate for half of one second */
						const double duration = 0.5;
						const auto   t        = rel_time / duration;
						if (t > 1)
							state.state_code = AnimStateCode::None;
						else
							sprite.color = vec3ToColor(lerp(t*t, sf::Vector3f(0, 0, 0), sf::Vector3f(1, 1, 1)));

						break;
					}
					case AnimStateCode::Dying:
					{
						const double duration = 1.0;
						const auto   t        = (float)(rel_time / duration);

						if (t > 1) state.state_code = AnimStateCode::Dead;
						else
						{
							sprite.color.a = (uint8_t)lerp(t*t, 255.f, 0.f);
							sprite.offset  = lerp(t, sf::Vector2f(0, 0), sf::Vector2f(0, -32.f));
						}

						break;
					}
				}
			});
	}
}