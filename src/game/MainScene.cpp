#include <game/scenes/MainScene.h>
#include <engine/Core.h>

#include <iostream>

using namespace s2de;

namespace game
{
    GameScreen::GameScreen() :
        s2de::Scene(sf::Vector2u{ 290, 328 })
    {   
        auto floater = _world.entity();
        floater.set(components::Transform {
            .position = sf::Vector2f(10, 10),
            .scale    = sf::Vector2f(1, 1),
            .rotation = sf::radians(0)
        })
        .set(components::Sprite {
            .texture    = getTexture("res/textures/floaty.png"),
            .rectangle  = sf::IntRect({ 0, 0 }, { 32, 32 }),
            .frames     = 4,
            .frame_time = 1
        });
    }

    void GameScreen::onRender()
    {
        
    }

    void GameScreen::onUpdate(double dt)
    {
        
    }

    void GameScreen::onEvent(const sf::Event& event)
    {

    }


    void MainScene::onRender()
    {
        _game_screen.getRenderTexture().clear(sf::Color(33 - 5, 30 - 5, 32 - 5, 255));
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
        _game_screen.onEvent(event);
    }
}