#pragma once

namespace soh
{

namespace params
{
    
    const unsigned int simulation_time{ 5 };

    const unsigned int map_width{ 20 };
    const unsigned int map_height{ 20 };

    const unsigned int player_count { 4 };
    const unsigned int player_move_quantum{ 20 };
    const unsigned int player_collect_quantum{ 20 };
    const unsigned int player_collect_max_periods{ 20 };
    const unsigned int player_collect_max_gold{ 2000 };
    const unsigned int player_fight_quantum{ 20 };
    const unsigned int player_fight_max_periods{ 20 };
    const unsigned int player_fight_max_loss{ 30 }; // % of army

    const unsigned int gold_mine_count{ 4 };
    const unsigned int gold_mine_dig_quantum{ 20 };
    const unsigned int gold_mine_dig_periods{ 20 };
    const unsigned int gold_mine_dig_max_gold{ 20 };
    const unsigned int gold_mine_transport_quantum{ 20 };
    const unsigned int gold_mine_transport_max_periods{ 20 };

    const unsigned int dwelling_count{ 4 };
    const unsigned int dwelling_produce_quantum{ 20 };
    const unsigned int dwelling_produce_periods{ 20 };
    const unsigned int dwelling_produce_creature_price{ 10 };
    const unsigned int dwelling_add_quantum{ 20 };
    const unsigned int dwelling_add_max_periods{ 20 };

} // namespace params
    
} // namespace soh
