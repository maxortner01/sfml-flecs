#include <iostream>

#include <flecs.h>
#include <SFML/Graphics.hpp>

// put your game source code here

int main()
{
    sf::RenderWindow window(sf::VideoMode({ 1280, 720 }), "wat");

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();

        window.display();
    }
}