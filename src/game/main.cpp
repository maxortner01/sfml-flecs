#include <iostream>

#include <engine/Core.h>
#include <game/game.h>

#include <filesystem>
#include <flecs.h>
#include <SFML/Graphics.hpp>

// put your game source code here
using namespace s2de;

int main()
{
    std::cout << std::filesystem::current_path() << "\n";
    
    Scene* scene = new game::MainScene({ 640, 360 });
    Application::create(sf::Vector2u{ 1920, 1440 }, scene);
    Application::get().run();
    Application::destroy(); // <- not explicitly destroying leads SFML to freak out
}
