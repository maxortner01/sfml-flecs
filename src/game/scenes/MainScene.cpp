#include <game/game.h>

#include <iostream>

using namespace s2de;

namespace game
{
    GameScreen::GameScreen() :
        s2de::Scene(sf::Vector2u{ 290, 328 }),
        _player(_world),
        _zoomed(false)
    {   
        sf::Image map;
        map.loadFromFile("res/maps/map1.png");

        addSystem<SysTilemap>(sf::Vector2f(64, 64), getTexture("res/textures/tile.png"), map, _world);
        addSystem<SysPhysics>();

        auto floater = _world.entity();
        floater.set(components::Transform {
            .position = sf::Vector2f(-40, -100),
            .scale    = sf::Vector2f(1, 1),
            .rotation = sf::radians(0)
        })
        .set(components::Sprite {
            .texture    = getTexture("res/textures/floaty.png"),
            .rectangle  = sf::IntRect({ 0, 0 }, { 32, 32 }),
            .frames     = 4,
            .frame_time = 1
        })
        .set(components::Depth {
            .z = 10.f
        });
    }

    void GameScreen::onRender()
    {
        _surface.setView(getView());
    }

    void GameScreen::onUpdate(double dt)
    {
        const sf::Vector2f target_zoom = (sf::Vector2f)getSize() * (_zoomed?4.f:1.f);
        setView(
            sf::View(
                getView().getCenter() + (_player.getEntity().get<const components::Transform>()->position - getView().getCenter()) * 2.5f * (float)dt,
                getView().getSize()   + (target_zoom - getView().getSize()) * 4.5f * (float)dt
            )
        );
    }

    void GameScreen::onEvent(const sf::Event& event)
    {
        if (event.type == sf::Event::KeyPressed)
        {
            switch (event.key.code)
            {
            case sf::Keyboard::Space: (_zoomed?_zoomed = false:_zoomed = true); break;
            default: break;
            }
        }
    }

    MainScene::MainScene(const sf::Vector2u& dimensions) :
        Scene(dimensions)
    {
        setView(sf::View((sf::Vector2f)dimensions / 2.f, (sf::Vector2f)dimensions));
    }

    void MainScene::onRender()
    {
        //_game_screen.getRenderTexture().clear(sf::Color(33 - 5, 30 - 5, 32 - 5, 255));
        _game_screen.getRenderTexture().clear(sf::Color(33, 30, 32, 255));
        _game_screen.renderChildren();
        _game_screen.onRender();
        _game_screen.getRenderTexture().display();

        sf::Sprite game_sprite(_game_screen.getTexture());
        game_sprite.setPosition(_game_screen.POSITION + sf::Vector2f(getSize().x / 2.f - 360.f / 2.f, 0));
        _surface.draw(game_sprite);

        sf::Sprite main_sprite(*getTexture("res/textures/main screen2.png"));
        main_sprite.setOrigin(main_sprite.getGlobalBounds().getSize() / 2.f);
        main_sprite.setPosition((sf::Vector2f)_surface.getSize() / 2.f);
        _surface.draw(main_sprite);

        _surface.display();
    }

    void MainScene::onUpdate(double dt)
    {
        _game_screen.updateChildren(dt);
        _game_screen.onUpdate(dt);
    }

    void MainScene::onEvent(const sf::Event& event)
    {
        _game_screen.eventChildren(event);
        _game_screen.onEvent(event);
    }
}
