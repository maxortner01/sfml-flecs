#pragma once

#include "../entities/Player.h"
#include <engine/Core.h>

namespace game
{
    class Player;

    class GameScreen : 
        public s2de::Scene
    {
        bool _zoomed;
        sf::Vector2f _mouse_pos;
        Player _player;

    public:
        const sf::Vector2f POSITION = sf::Vector2f(38, 14);
        const sf::Vector2f SIZE     = sf::Vector2f(290, 328);
        
        GameScreen();

        sf::Vector2f toWorld(const sf::Vector3f& pos) const override;

        void onRender();
        void onUpdate(double dt);
        void onEvent(const sf::Event& event);        
    };

    class MainScene :
        public s2de::Scene
    {
        GameScreen _game_screen;

    public:
        MainScene(const sf::Vector2u& dimensions);

        void onRender();
        void onUpdate(double dt);
        void onEvent(const sf::Event& event);
    };
}