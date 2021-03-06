#pragma once

#include <cstdint>

namespace soh
{

namespace params
{

    const size_t map_width{ 100 };
    const size_t map_height{ 50 };
    const float map_max_treasures{ 0.3f }; // % of all entites
    const float map_max_creatures{ 0.3f }; // % of all entites

    const int player_count { 4 };
    const int player_move_quantum{ 1 };
    const int player_collect_quantum{ 1 }; 
    const int player_collect_max_periods{ 10 };
    const int player_collect_max_gold{ 2000 };
    const int player_fight_quantum{ 1 };
    const int player_fight_max_periods{ 10 };
    const float player_fight_max_loss{ 0.5f }; // % of army

    const int gold_mine_count{ 10 };
    const int gold_mine_dig_quantum{ 1 };
    const int gold_mine_dig_periods{ 10 };
    const int gold_mine_dig_max_gold{ 100 };
    const int gold_mine_transport_quantum{ 1 };
    const int gold_mine_transport_max_periods{ 10 };

    const int dwelling_count{ 10 };
    const int dwelling_produce_quantum{ 1 };
    const int dwelling_produce_periods{ 10 };
    const int dwelling_produce_creature_price{ 20 };
    const int dwelling_produce_max_creatures_count{ 100 };
    const int dwelling_add_quantum{ 1 };
    const int dwelling_add_max_periods{ 10 };

} // namespace params
    
} // namespace soh
