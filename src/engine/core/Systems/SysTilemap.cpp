#include <engine/Core.h>

#include <bitset>
#include <iostream>

namespace s2de
{
    SysTilemap::SysTilemap(const sf::Vector2f& tile_size, sf::Texture* tilemap, const sf::Image& map, flecs::world& world) :
        _tile_size(tile_size),
        _tilemap(tilemap)
    {   
        generateTileMap(_tiles, map, world);

        //_tiles.resize(6);
        //_tiles[0] = { 4, 5, 5, 5, 0, 0 };
        //_tiles[1] = { 3, 8, 6, 6, 0, 0 };
        //_tiles[2] = { 3, 7, 1, 1, 0, 0 };
        //_tiles[3] = { 3, 7, 1, 1, 0, 0 };
        //_tiles[4] = { 0, 0, 0, 0, 0, 0 };
        //_tiles[5] = { 0, 0, 0, 0, 0, 0 };
    }

    void SysTilemap::onUpdate(double dt, flecs::world& world) 
    {
        using namespace components;
        return;

        auto player = world.filter<components::Player, Transform>().first();
        auto map_coordinates = worldToMap(player.get<const Transform>()->position);

        /* List out all the adjacent tiles we want to check */
        std::vector<sf::Vector2i> adjacent;
        algo::pointsInCircle(adjacent, map_coordinates, 10);

        /* Make a list to check whether or not the adjacent cell exists  */
        std::vector<bool> indices(adjacent.size());
        std::fill(indices.begin(), indices.end(), 0);

        auto f = world.filter<Tile, MapCoordinates>();
        f.each([adjacent, &indices](flecs::entity e, Tile r, MapCoordinates& coordinates)
        {
            for (uint32_t j = 0; j < indices.size(); j++)
                if (coordinates.coordinates == adjacent[j])
                {
                    indices[j] = 1;
                    break;
                }
        });

        for (uint32_t i = 0; i < indices.size(); i++)
            if (!indices[i]) generateTile(adjacent[i], world);
    }

    void SysTilemap::generateTileMap(Tilemap& tilemap, const sf::Image& map, flecs::world& world)
    {
        //sf::Image image;
        //image.create(map.getSize(), sf::Color::Black);

        //tilemap.resize(map.getSize().y);
        for (uint32_t y = 0; y < map.getSize().y; y++)
        {
            //tilemap[y].resize(map.getSize().x);
            for (uint32_t x = 0; x < map.getSize().x; x++)
            {
                const sf::Color pixel = map.getPixel({x, y});
                if (pixel == sf::Color::White) 
                {
                    createTile(sf::Vector2i(x, y), 0, world); 
                    continue; 
                }
                continue;

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

                std::bitset<8> wallright;
                wallright[0] = 0; wallright[1] = 0; wallright[2] = 0;
                wallright[3] = 0; /*             */ wallright[4] = 0;
                wallright[5] = 0; wallright[6] = 1; wallright[7] = 0;
 
                if (adjacency[1] == 0 && adjacency[6] == 1) 
                { 
                    tilemap[y][x] = WallRight; 
                    tilemap[y - 1][x] = RoofRight;
                }
            }
        }

        //image.saveToFile("res/test.png");

        /*
        tilemap.resize(map.getSize().y);
        for (uint32_t i = 0; i < tilemap.size(); i++)
        {
            tilemap[i].resize(map.getSize().x);
            for (uint32_t j = 0; j < tilemap[i].size(); j++)
            {
                const sf::Color color = map.getPixel(sf::Vector2u(j, i));
                
                if (color == sf::Color::White)
                {
                    tilemap[i][j] = Ground1;
                }
                else
                    tilemap[i][j] = None;
            }
        }*/
    }

    void SysTilemap::createTile(const sf::Vector2i& coordinate, uint32_t index, flecs::world& world)
    {
        using namespace components;

        world.entity()
            .set(Transform {
                .position = mapToWorld(coordinate),
                .scale    = { 1, 1 },
                .rotation = sf::radians(0)
            })
            .set(Sprite {
                .texture = _tilemap,
                .rectangle = sf::IntRect({ 64 * (int32_t)index, 0 }, { 64, 64 }),
                .frames = 1,
                .frame_time = 1.f
            })
            .set(MapCoordinates {
                .coordinates = coordinate
            })
            .set(Depth {
                .z = -10.f + mapToWorld(coordinate).y / 1000.f
            })
            .add<Tile>();
    }

    void SysTilemap::generateTile(const sf::Vector2i& coordinate, flecs::world& world) const
    {   
        using namespace components;

        /* Make sure the coordinates are within the tile map */
        if ((coordinate.x < 0 || coordinate.y < 0) || 
            (coordinate.y >= _tiles.size() || coordinate.x >= _tiles[0].size())) 
                return;

        const uint8_t tile_code = _tiles[coordinate.y][coordinate.x];
        int32_t index = (int32_t)tile_code - 1;
        if (index < 0) return;

        world.entity()
            .set(Transform {
                .position = mapToWorld(coordinate),
                .scale    = { 1, 1 },
                .rotation = sf::radians(0)
            })
            .set(Sprite {
                .texture = _tilemap,
                .rectangle = sf::IntRect({ 64 * index, 0 }, { 64, 64 }),
                .frames = 1,
                .frame_time = 1.f
            })
            .set(MapCoordinates {
                .coordinates = coordinate
            })
            .set(Depth {
                .z = -10.f + mapToWorld(coordinate).y / 1000.f
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
