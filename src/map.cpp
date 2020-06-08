#include "map.hpp"

#include <algorithm>
#include <cassert>
#include <limits>
#include <numeric>
#include <random>

soh::map::map(size_t width, size_t height, 
    float max_treasures, float max_creatures)
    : width{ width }
    , height{ height }
{
    generate_level(max_treasures, max_creatures);
}

size_t soh::map::get_width() const
{
    return width;
}

size_t soh::map::get_height() const
{
    return height;
}

const std::vector<int>& soh::map::get_map() const
{
    return level;
}

int soh::map::get_tile(utils::vec2<int> position) const
{
    return level[position.y * width + position.x];
}

utils::vec2<int> soh::map::nearest_treasure(utils::vec2<int> player_pos) const
{
    return nearest_entity(player_pos, treasure_list);
}

utils::vec2<int> soh::map::nearest_creature(utils::vec2<int> player_pos) const
{
    return nearest_entity(player_pos, creature_list);
}

void soh::map::delete_treasure(utils::vec2<int> position)
{
    delete_entity(position, treasure_list);
}

void soh::map::delete_creature(utils::vec2<int> position)
{
    delete_entity(position, creature_list);   
}

void soh::map::generate_level(float max_treasures, float max_creatures)
{
    assert(max_treasures >= 0.0f);
    assert(max_creatures >= 0.0f);

    auto to_coords = [=](int index)
    {
        return utils::vec2<int>{ static_cast<int>(index % width), static_cast<int>(index / width) };
    };

    level.resize(width * height);

    const auto entities_count{ static_cast<unsigned int>(level.size() * 0.5) };
    const auto max_treasures_count{ static_cast<unsigned int>(max_treasures * entities_count) };
    const auto max_creatures_count{ static_cast<unsigned int>(max_creatures * entities_count) };

    std::vector<int> empty_tiles(level.size() - 1);
    std::iota(empty_tiles.begin(), empty_tiles.end(), 1);
    std::shuffle(empty_tiles.begin(), empty_tiles.end(), std::mt19937{ std::random_device{}() });

    for (size_t i = 0; i < max_treasures_count; ++i)
    {
        treasure_list.emplace_back(to_coords(empty_tiles[i]));
        level[empty_tiles[i]] = 1;
    }

    for (size_t i = max_treasures_count; i < (max_treasures_count + max_creatures_count); ++i)
    {
        creature_list.emplace_back(to_coords(empty_tiles[i]));
        level[empty_tiles[i]] = -1;
    }
}

utils::vec2<int> soh::map::nearest_entity(
    utils::vec2<int> player_pos, const std::vector<utils::vec2<int>>& entity_list) const
{
    int min_distance{ std::numeric_limits<int>::max() };
    utils::vec2<int> nearest_entity;

    if (entity_list.empty())
    {
        nearest_entity = player_pos;
    }

    for (const auto entity_pos : entity_list)
    {
        auto distance{ utils::taxicab_distance(player_pos, entity_pos) };
        if (distance < min_distance)
        {
            min_distance = distance;
            nearest_entity = entity_pos;
        }

        if (distance <= 1)
        {
            break;
        }
    }

    return nearest_entity;
}

void soh::map::delete_entity(utils::vec2<int> entity_pos, std::vector<utils::vec2<int>>& entity_list)
{
    level[entity_pos.y * width + entity_pos.x] = 0;
    entity_list.erase(
        std::remove_if(
            entity_list.begin(), entity_list.end(), 
            [=](auto pos){ return pos.x == entity_pos.x && pos.y == entity_pos.y; }),
        entity_list.end());
}