#pragma once

#include <mutex>
#include <vector>

#include "utils.hpp"

namespace soh
{
class map
{
public:
    map(size_t width, size_t height, 
        float max_treasures, float max_creatures);

    std::mutex mutex;

    size_t get_width() const;
    size_t get_height() const;
    const std::vector<int>& get_map() const;
    int get_tile(utils::vec2<int> position) const;

    utils::vec2<int> nearest_treasure(utils::vec2<int> player_pos) const;
    utils::vec2<int> nearest_creature(utils::vec2<int> player_pos) const;

    void delete_treasure(utils::vec2<int> position);
    void delete_creature(utils::vec2<int> position);
 
private:
    void generate_level(float max_treasures, float max_creatures);
    utils::vec2<int> nearest_entity(
        utils::vec2<int> player_pos, const std::vector<utils::vec2<int>>& entity_list) const;
    void delete_entity(utils::vec2<int> entity_pos, std::vector<utils::vec2<int>>& entity_list);

    size_t width;
    size_t height;
    std::vector<int> level;
    std::vector<utils::vec2<int>> treasure_list;
    std::vector<utils::vec2<int>> creature_list;
};
   
} // namespace soh