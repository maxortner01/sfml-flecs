#include <engine/Core.h>
#include <iostream>

namespace s2de
{
    Scene::Scene(const sf::Vector2u& dimensions) :  
        _dimensions(dimensions)
    {
        assert(_surface.create(dimensions));
    }

    Scene::~Scene()
    {
        // wtf
        _surface.~RenderTexture();

        for (auto it = _textures.begin(); it != _textures.end(); it++)
            delete (*it).second;

        for (auto* system : _systems) delete system;
    }

    sf::Vector2u Scene::getSize() const
    { return _dimensions; }

    const sf::Texture& Scene::getTexture() const
    { return _surface.getTexture(); }

    const sf::View& Scene::getView() const
    { return _view; }

    void Scene::setView(const sf::View& view) 
    {
        _view = view;
        _surface.setView(view);
    }

    sf::Texture* Scene::getTexture(const std::string& path)
    {
        auto tex = _textures.find(path);
        if (tex != _textures.end())
            return (*tex).second;
        else
        {
            sf::Texture* _texture = new sf::Texture();
            _texture->loadFromFile(path);
            _textures.insert(std::pair<std::string, sf::Texture*>(path, _texture));
            return _texture;
        }
    }

    void Scene::eventChildren(const sf::Event& event)
    {
        auto input = _world.filter<components::Input>();
        input.each([event](components::Input& input)
        { input.object->onEvent(event); });
    }

    void Scene::updateChildren(double dt)
    {
        auto scriptable_objects = _world.filter<components::Scriptable>();
        scriptable_objects.each([dt, this](components::Scriptable& script)
        { script.object->onUpdate(dt, this->_world); });

        for (auto* system : getSystems()) 
            system->onUpdate(dt, getWorld());
    }

    void Scene::renderChildren()
    {
        auto sprites = _world.filter<components::Sprite, components::Transform>();
        sprites.each([this](components::Sprite& sprite, components::Transform& transform) 
        {
            sf::IntRect rectangle = sprite.rectangle;
            rectangle.left += sprite.rectangle.width * ((int)(this->clock.getElapsedTime().asSeconds() / sprite.frame_time * sprite.frames) % sprite.frames);

            sf::Sprite object(*sprite.texture);
            object.setTextureRect(rectangle);
            object.setPosition(transform.position);
            object.setScale(transform.scale);
            object.setRotation(transform.rotation);
            this->_surface.draw(object);
        });

        auto rectangles = _world.filter<components::Rectangle, components::Transform>();
        rectangles.each([this](components::Rectangle& rect, components::Transform& transform)
        {
            sf::RectangleShape sf_rect;
            sf_rect.setSize(rect.dimensions);
            sf_rect.setOrigin(sf_rect.getSize() / 2.f);
            sf_rect.setFillColor(rect.color);
            sf_rect.setPosition(transform.position);
            sf_rect.setScale(transform.scale);
            sf_rect.setRotation(transform.rotation);
            this->_surface.draw(sf_rect);
        });

        /* on render doesn't work for some reason... */
        //auto scriptable_objects = _world.filter<components::Scriptable>();
        //scriptable_objects.each([this](components::Scriptable& script)
        //{ script.object->onRender(this->_surface); });

        // Render the systems
        for (auto* system : getSystems())
            system->onRender(_surface, _world);
    }

    const std::vector<System*>& Scene::getSystems() const
    { return _systems; }

    flecs::world& Scene::getWorld() 
    { return _world; }
}
