#include <engine/Core.h>

namespace s2de
{
    Application::Application(const sf::Vector2u& dimensions, Scene* start_scene) :
        _window(sf::VideoMode(dimensions), "test")
    {   
        _scenes.push_back(start_scene);
    }

    Application::~Application()
    {   }

    void Application::run()
    {
        sf::Clock frame_clock;
        while (_window.isOpen() && _scenes.size())
        {
            double dt = frame_clock.restart().asSeconds();

            // grab the top scene, if it's done delete it and grab the new
            // top scene
            Scene* current_scene = _scenes.back();
            if (current_scene->getState() == SceneState::Complete)
            {
                delete current_scene;
                _scenes.pop_back();
                current_scene = _scenes.back();
            }

            /* Get the event objects and pass the sfml events to them */
            sf::Event event;
            while (_window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    _window.close();

                current_scene->eventChildren(event);
                current_scene->onEvent(event);
            }

            /* Frame rendering begins */
            _window.clear(sf::Color(33, 30, 32, 255));

            /* Update objects, scriptable objects and then systems */
            current_scene->onUpdate(dt);
            current_scene->updateChildren(dt);

            /* We want to render all the scenes, so we iterate through them and render them to the window */
            for (auto* scene : _scenes)
            {
                // Clear the surface and render the objects
                scene->_surface.clear(sf::Color::Transparent);
                scene->renderChildren();

                // Finally, call the scene render
                scene->onRender();
                scene->_surface.setView(scene->getView());

                // Now we draw the surface to the window
                const sf::Vector2f scale = sf::Vector2f(
                    (float)_window.getSize().x / (float)scene->getSize().x,
                    (float)_window.getSize().y / (float)scene->getSize().y
                );

                sf::Sprite surface(scene->getTexture());                
                surface.setScale(scale);
                _window.draw(surface);
            }

            _window.setTitle("FPS: " + std::to_string((int)(1.0 / dt)));
            _window.display();
        }

        for (auto* scene : _scenes) delete scene;
        _scenes.clear();
    }
}