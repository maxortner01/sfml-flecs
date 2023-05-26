#pragma once

#include "../System.h"

namespace s2de
{
    struct SysTilemap : public System
    {
        enum Tiles
        {
            None,
            Ground1,
            Ground2,
            RoofA,
            RoofB,
            RoofC,
            RoofD,
            RoofE,
            RoofF,
            RoofG,
            WallRight,
            WallLeft,
            WallTop,
            WallBottom
        };

        typedef std::vector<std::vector<uint8_t>> Tilemap;

        SysTilemap(const sf::Vector2f& tile_size, sf::Texture* tilemap, const sf::Image& map, flecs::world& world);
        void onUpdate(double, flecs::world&) override;

    private:
        void generateTileMap(Tilemap& tilemap, const sf::Image& map, flecs::world& world);

        void createTile(const sf::Vector2i& coordinate, uint32_t index, flecs::world& world, float elevation = 0.f);
        void generateTile(const sf::Vector2i& coordinate, flecs::world& world) const;
        sf::Vector2f mapToWorld(const sf::Vector2i& map) const;
        sf::Vector2i worldToMap(const sf::Vector2f& world) const;

        sf::Texture* _tilemap;
        sf::Vector2f _tile_size;
        Tilemap _tiles;
    };
}