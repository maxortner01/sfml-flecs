#include <game/game.h>

namespace game
{
    BattleSystem::BattleSystem()
    {
        _attack_bounds.resize(4);
    }

	void BattleSystem::onRender(s2de::Scene& scene, flecs::world& world)
	{
        using namespace s2de;

        auto mouse_pos = (sf::Vector2f)sf::Mouse::getPosition(Application::get().getWindow());
        const auto scene_rect = Application::get().getSceneRect(scene);
        mouse_pos.x -= scene_rect.left;
        mouse_pos.x *= scene.getSize().x / scene_rect.width;
        mouse_pos.y *= scene.getSize().y / scene_rect.height;
        mouse_pos += (scene.getView().getCenter() - scene.getView().getSize() / 2.f);

        sf::Vector2f pos;
        pos.x = 1.f / 64.f * (mouse_pos.x + 2 * mouse_pos.y);
        pos.y = 4.f / 64.f * mouse_pos.y - pos.x;
        _mouse_pos = pos;

        const auto player_pos = world.filter<const components::Player, const components::Transform>().first().get<const components::Transform>()->position;
        const auto dir = (_mouse_pos - sf::Vector2f(player_pos.x, player_pos.y)).normalized();

        sf::Vector2f points[] = {
            sf::Vector2f(-1.f, .5f),
            sf::Vector2f(1.f, .5f),
            sf::Vector2f(1.f, 1.5f),
            sf::Vector2f(-1.f, 1.5f)
        };

        sf::VertexArray array;
        array.resize(5);
        array.setPrimitiveType(sf::PrimitiveType::LineStrip);
        for (int i = 0; i < 5; i++)
        {
            const auto rotated = points[i % 4].rotatedBy(sf::radians(atan2f(dir.y, dir.x) - 3.14159f / 2.f));
            _attack_bounds[i % 4] = sf::Vector3f(rotated.x, rotated.y, 0) + player_pos;
            array[i].position = scene.toWorld(_attack_bounds[i % 4] + player_pos);
        }

        scene.getRenderTexture().draw(array);
	}

	void BattleSystem::onUpdate(double dt, flecs::world& world)
	{
        /* Grab all the enemies and do an intersection test with the attack boundary */
	}
}