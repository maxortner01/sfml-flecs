#include <game/game.h>

namespace game
{
	void SysHpDraw::onRender(s2de::Scene& scene, flecs::world& world)
	{
		using namespace s2de::components;

		auto hps = world.filter<const HP, const Transform>();
		hps.each([&scene](const HP& hp, const Transform& transform)
			{
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
	}

	void SysHpDraw::onUpdate(double dt, flecs::world& world)
	{

	}
}