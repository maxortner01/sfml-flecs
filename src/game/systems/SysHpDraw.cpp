#include <game/game.h>

namespace game
{
	SysHpDraw::SysHpDraw(sf::Texture* _healthbar) :
		health_texture(_healthbar)
	{ 
		healthbar.create(
			sf::Vector2u(
				health_texture->getSize().x,
				(uint32_t)(health_texture->getSize().y / 3.f)
			)
		);

		inside_health.create(
			sf::Vector2u(
				110, 8
			)
		);
	}

	void SysHpDraw::onRender(s2de::Scene& scene, flecs::world& world)
	{
		using namespace s2de::components;

		/* Enemy HP */
		auto hps = world.filter<const Enemy, const HP, const Transform, const AnimationState>();
		hps.each([&scene](const Enemy& enemy, const HP& hp, const Transform& transform, const AnimationState& state)
			{
				if (state.state_code == AnimStateCode::Dying) return;

				sf::RectangleShape rectangle;
				rectangle.setSize(sf::Vector2f(24.f, 4.f));
				rectangle.setOrigin(sf::Vector2f(0, rectangle.getSize().y / 2.f));
				rectangle.setPosition(scene.toWorld(transform.position));
				rectangle.move(sf::Vector2f(0, -16));
				rectangle.move(sf::Vector2f(rectangle.getSize().x / -2.f, 0));

				rectangle.setFillColor(sf::Color(33, 30, 32, 255));
				rectangle.setOutlineColor(sf::Color(85, 85, 104, 255));
				rectangle.setOutlineThickness(1.f);
				scene.getRenderTexture().draw(rectangle);

				rectangle.setScale(sf::Vector2f(std::max(hp.value / hp.max, 0.f), 1));
				rectangle.setFillColor(sf::Color(193, 203, 179, 255));
				rectangle.setOutlineThickness(0.f);
				scene.getRenderTexture().draw(rectangle);
			});

		/* Player HP */
		const auto player_hp = world.filter<const s2de::components::Player, const Transform, HP>().first().get<const HP>();
		const auto hp_scaled = player_hp->value / player_hp->max;
		/* Inside Health */
		inside_health.clear(sf::Color::Transparent);
		sf::RectangleShape health_rect;
		health_rect.setSize(sf::Vector2f(110, 8));
		health_rect.setFillColor(sf::Color(182, 212, 197));
		health_rect.setScale(sf::Vector2f(hp_scaled, 1.f));
		inside_health.draw(health_rect);

		sf::Sprite health_cap(*health_texture);
		health_cap.setTextureRect(
			sf::IntRect(
				{ 0, 3 },
				{ 6, 8 }
			)
		);
		health_cap.move(sf::Vector2f(health_rect.getGlobalBounds().width, 0));
		inside_health.draw(health_cap);
		inside_health.display();
		

		/* Health bar texture */
		healthbar.clear(sf::Color::Transparent);
		sf::Sprite healthbar_sprite(*health_texture);
		const int32_t height = (int32_t)(health_texture->getSize().y / 3.f);
		healthbar_sprite.setTextureRect(
			sf::IntRect(
				{ 0, height * 2 },
				{ (int32_t)health_texture->getSize().x, height }
			)
		);
		healthbar.draw(healthbar_sprite);

		{
			sf::Sprite inside_sprite(inside_health.getTexture());
			inside_sprite.setPosition({ 3.f, 3.f });
			healthbar.draw(inside_sprite);
		}

		healthbar_sprite.setTextureRect(
			sf::IntRect(
				{ 0, height },
				{ (int32_t)health_texture->getSize().x, height }
			)
		);
		healthbar.draw(healthbar_sprite);

		healthbar.display();

		auto player = world.filter<s2de::components::Player, const HP>();
		const sf::Vector2u point = sf::Vector2u(46, 326);

		sf::Sprite health(healthbar.getTexture());
		health.setPosition(scene.getRenderTexture().getView().getCenter() - (sf::Vector2f)scene.getSize() / 2.f);
		health.move(sf::Vector2f(26, 310));
		scene.getRenderTexture().draw(health);
	}

	void SysHpDraw::onUpdate(double dt, flecs::world& world)
	{

	}
}