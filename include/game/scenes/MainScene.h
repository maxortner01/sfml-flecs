#pragma once

#include "../entities/Player.h"
#include <engine/Core.h>

namespace game
{
    class Player;

    class GameScreen : 
        public s2de::Scene
    {
        Player _player;

    public:
        const sf::Vector2f POSITION = sf::Vector2f(38, 14);
        const sf::Vector2f SIZE     = sf::Vector2f(290, 328);
        
        GameScreen();

        void onRender();
        void onUpdate(double dt);
        void onEvent(const sf::Event& event);        
    };

    class MainScene :
        public s2de::Scene
    {
        GameScreen _game_screen;

    public:
        using s2de::Scene::Scene;

        void onRender();
        void onUpdate(double dt);
        void onEvent(const sf::Event& event);
    };
}