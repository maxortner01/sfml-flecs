#include <iostream>

#include <engine/Core.h>
#include <game/game.h>

#include <flecs.h>
#include <SFML/Graphics.hpp>

// put your game source code here

using namespace s2de;

int main()
{
    Scene* scene = new game::MainScene({ 640, 360 });
    Application::create(sf::Vector2u{ 1280, 720 }, scene);
    Application::get().run();
}