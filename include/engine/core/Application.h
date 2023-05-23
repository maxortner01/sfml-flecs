#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include <engine/Utility.h>

namespace s2de
{
    class Scene;

    class Application :
        public utility::Singleton<Application>
    {
        std::vector<Scene*> _scenes;
        sf::RenderWindow    _window;

        Application(const sf::Vector2u& dimensions, Scene* start_scene);
        ~Application();

    public:
        friend class utility::Singleton<Application>;

        void run();
    };
}