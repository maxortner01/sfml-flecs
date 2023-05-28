#include <engine/Core.h>

#include <bitset>
#include <iostream>

namespace s2de
{
    SysTilemap::SysTilemap(const sf::Vector2f& tile_size, sf::Texture* tilemap, const sf::Image& map, flecs::world& world) :
        _tile_size(tile_size),
        _tilemap(tilemap)
    {   
        /* Start up and generate tilemap right away (create entities) */
        generateTileMap(_tiles, map, world);
    }

    void SysTilemap::onUpdate(double dt, flecs::world& world) 
    {
        using namespace components;
    }

    void SysTilemap::generateTileMap(Tilemap& tilemap, const sf::Image& map, flecs::world& world)
    {
        for (uint32_t y = 0; y < map.getSize().y; y++)
        {
            for (uint32_t x = 0; x < map.getSize().x; x++)
            {
                const sf::Color pixel = map.getPixel({x, y});
                if (pixel == sf::Color::White) createTile(sf::Vector2i(x, y), Ground1 - 1, world);

                std::bitset<8> adjacency;
                uint32_t k = 0;
                for (int32_t i = -1; i <= 1; i++)
                    for (int32_t j = -1; j <= 1; j++)
                    {
                        if (i == 0 && j == 0) continue;

                        adjacency[k++] = 0;
                        const sf::Vector2i index = sf::Vector2i(j + x, i + y);
                        if (index.x < 0 || index.y < 0 || index.x >= map.getSize().x || index.y >= map.getSize().y)
                            continue;

                        if (map.getPixel((sf::Vector2u)index) == sf::Color::White)
                            adjacency[k - 1] = 1;
                    }

                /*
                std::bitset<8> wallright;
                wallright[0] = 0; wallright[1] = 0; wallright[2] = 0;
                wallright[3] = 0; /               / wallright[4] = 0;
                wallright[5] = 0; wallright[6] = 1; wallright[7] = 0; */
    
                if (pixel == sf::Color::White) continue;

                bool need_corner = false;

                /* Top wall and roof */
                if (adjacency[1] == 0 && adjacency[6] == 1) 
                { 
                    createTile(sf::Vector2i(x, y), WallRight - 1, world, 0.f);

                    if (!(adjacency[4] && adjacency[6]))
                        createTile(sf::Vector2i(x - 1, y - 1), RoofC - 1, world, 0.f);
                    else need_corner = true;
                }

                /* Middle roof and wall */
                if (adjacency[1] && adjacency[6])
                {
                    createTile(sf::Vector2i(x, y), WallRight - 1, world, 0.f);
                    createTile(sf::Vector2i(x, y), RoofG - 1, world, 1.f);
                }
                /* Left wall and roof */
                if (!adjacency[3] && adjacency[4])
                {
                    createTile(sf::Vector2i(x, y), WallLeft - 1, world, 0.f);

                    if (!(adjacency[4] && adjacency[6]))
                        createTile(sf::Vector2i(x, y), RoofA - 1, world, 1.f);
                    else need_corner = true;
                }
                if (adjacency[7] && !adjacency[6] && !adjacency[4])
                {
                    createTile(sf::Vector2i(x, y), RoofB - 1, world, 1.f);
                }

                if (need_corner)
                {
                    createTile(sf::Vector2i(x, y), RoofF - 1, world, 1.f);
                }
            }
        }
    }

    void SysTilemap::createTile(const sf::Vector2i& coordinate, uint32_t index, flecs::world& world, float elevation)
    {
        using namespace components;

        const auto pos = sf::Vector3f(coordinate.x, coordinate.y, elevation);

        world.entity()
            .set(Transform {
                .position = pos,
                .scale    = { 1, 1 },
                .rotation = sf::radians(0)
            })
            .set(Sprite {
                .texture = _tilemap,
                .rectangle = sf::IntRect({ 64 * (int32_t)index, 0 }, { 64, 64 }),
                .frames = 1,
                .frame_time = 1.f,
                .color = sf::Color::White
            })
            .add<Tile>();
    }

/* Offset so we don't have to worry about doing map coordinate transformations on negative coordinates */
#define OFFSET 200

    sf::Vector2f SysTilemap::mapToWorld(const sf::Vector2i& map) const
    {
        const algo::Matrix basis_matrix = {
            {  1 * _tile_size.x / 2.f, -1 * _tile_size.x / 2.f },
            { 0.5f * _tile_size.y / 2.f, 0.5f * _tile_size.y / 2.f } 
        };

        const sf::Vector2f basis[] = {
            sf::Vector2f( basis_matrix[0][0], basis_matrix[1][0]), 
            sf::Vector2f( basis_matrix[0][1], basis_matrix[1][1]), 
        };

        return (float)map.x * basis[0] + (float)map.y * basis[1];

        //return sf::Vector2f( 16.f * map.x - OFFSET, 16.f * map.y - 8.f * (map.x % 2) - OFFSET );
    }

    sf::Vector2i SysTilemap::worldToMap(const sf::Vector2f& world) const
    {
        const algo::Matrix basis_matrix = {
            { 1.f  * _tile_size.x / 2.f, -1.f * _tile_size.x / 2.f },
            { 0.5f * _tile_size.y / 2.f, 0.5f * _tile_size.y / 2.f } 
        };

        algo::Matrix inverse_matrix;
        algo::inverse2x2(inverse_matrix, basis_matrix);

        const sf::Vector2f basis[] = {
            sf::Vector2f( inverse_matrix[0][0], inverse_matrix[1][0]), 
            sf::Vector2f( inverse_matrix[0][1], inverse_matrix[1][1]), 
        };

        return (sf::Vector2i)(world.x * basis[0]) + (sf::Vector2i)(world.y * basis[1]);

        //int map_x = (world.x + OFFSET) / 16.f;
        //return sf::Vector2i(map_x, (world.y + OFFSET + 8.f * (map_x % 2)) / 16.f);
    }

#undef OFFSET
}
