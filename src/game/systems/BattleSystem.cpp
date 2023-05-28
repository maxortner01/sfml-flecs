#include <game/game.h>

namespace game
{
    /* https://stackoverflow.com/questions/2049582/how-to-determine-if-a-point-is-in-a-2d-triangle */
    float sign(const sf::Vector2f& p1, const sf::Vector2f& p2, const sf::Vector2f& p3)
    {
        return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
    }

    bool pointInTriangle(const sf::Vector2f& p, const sf::Vector2f& p1, const sf::Vector2f& p2, const sf::Vector2f& p3)
    {
        auto d1 = sign(p, p1, p2);
        auto d2 = sign(p, p2, p3);
        auto d3 = sign(p, p3, p1);

        bool has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
        bool has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

        return !(has_neg && has_pos);
    }

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
            sf::Vector2f(-1.f,  .5f),
            sf::Vector2f( 1.f,  .5f),
            sf::Vector2f( 1.f, 1.5f),
            sf::Vector2f(-1.f, 1.5f)
        };

        sf::VertexArray array;
        array.resize(5);
        array.setPrimitiveType(sf::PrimitiveType::LineStrip);
        for (int i = 0; i < 5; i++)
        {
            const auto rotated = points[i % 4].rotatedBy(sf::radians(atan2f(dir.y, dir.x) - 3.14159f / 2.f));
            array[i].position  = scene.toWorld(sf::Vector3f(rotated.x, rotated.y, 0) + player_pos);
            array[i].color     = sf::Color(255, 255, 255, 127);

            if (i < 4) _attack_bounds[i % 4] = sf::Vector3f(rotated.x, rotated.y, 0) + player_pos;
        }        

        //scene.getRenderTexture().draw(array);
	}

	void BattleSystem::onUpdate(double dt, flecs::world& world)
	{
        using namespace s2de::components;

        const auto player_pos = world.filter<const s2de::components::Player, const Transform>().first().get<const Transform>()->position;

        std::vector<flecs::entity> _dead_enemies;
        auto enemies = world.filter<const Enemy, Force, HP, const Transform>();
        enemies.each([&_dead_enemies, &player_pos, this](flecs::entity e, const Enemy& enemy, Force& force, HP& hp, const Transform& transform)
            {
                const sf::Vector2f proj = sf::Vector2f(transform.position.x, transform.position.y);
                
                sf::Vector2f points[4];
                for (uint32_t i = 0; i < 4; i++)
                    points[i] = sf::Vector2f(this->_attack_bounds[i].x, this->_attack_bounds[i].y);

                if (
                    (pointInTriangle(proj, points[0], points[1], points[2]) ||
                        pointInTriangle(proj, points[0], points[2], points[3])) && sf::Mouse::isButtonPressed(sf::Mouse::Left)
                    )
                {
                    auto dir = (transform.position - player_pos);
                    dir.z = 0; dir = dir.normalized();

                    force.direction += dir * 40.f;
                    hp.value -= 0.01;
                }

                if (hp.value <= 0) _dead_enemies.push_back(e);
            });

        /* destroy bullets or hurt player */
        std::vector<flecs::entity> _dead_bullets;
        auto bullets = world.filter<Bullet, const Transform>();
        bullets.each([&_dead_bullets](flecs::entity e, Bullet bullet, const Transform& transform)
            {
                if (transform.position.z <= 0) _dead_bullets.push_back(e);
            });

        for (auto& e : _dead_bullets) e.destruct();
        for (auto& e : _dead_enemies) e.destruct();
	}
}