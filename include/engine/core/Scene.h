#pragma once

#include <vector>

#include "System.h"
#include "../Utility.h"

namespace s2de
{
    enum class SceneState
    {
        Good,
        Complete
    };

    class Scene :
        public utility::NoCopy,
        public utility::State<SceneState>
    {
    public:
        friend class Application;

        Scene(const sf::Vector2u& dimensions);
        virtual ~Scene();

        sf::Vector2u       getSize() const;
        const sf::Texture& getTexture() const;
        const sf::View&    getView() const;
        void               setView(const sf::View& view);

        virtual void onRender() = 0;
        virtual void onUpdate(double dt) = 0;
        virtual void onEvent(const sf::Event& event) = 0;

        sf::Texture* getTexture(const std::string& path);

        void eventChildren(const sf::Event& event);
        void updateChildren(double dt);
        void renderChildren();

        // Systems
        const std::vector<System*>& getSystems() const;
        template<typename T, typename... Args> 
        void addSystem(Args&&... args)
        {
            static_assert(std::is_base_of<System, T>::value);
            _systems.push_back(new T(args...));
        }

        flecs::world& getWorld();
        sf::RenderTexture& getRenderTexture() { return _surface; }

    protected:
        sf::RenderTexture    _surface;
        flecs::world         _world;
        
    private:
        sf::Clock clock;
        std::vector<System*> _systems;

        std::unordered_map<std::string, sf::Texture*> _textures;
        const sf::Vector2u _dimensions;
        sf::View _view;
    };
}