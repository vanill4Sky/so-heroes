#include "player.hpp"

#include <algorithm>
#include <chrono>
#include <iostream>
#include <mutex>
#include <condition_variable>

#include "configuration.hpp"

int soh::player::instance_count{ 0 };
int soh::player::current_player{ -1 };
bool soh::player::gameover{ false };
std::mutex soh::player::mutex;
std::condition_variable soh::player::cv;

soh::player::player(std::string name, soh::treasury& treasury, 
    soh::army& army, soh::map& map, soh::visualization& visualization)
    : name{ std::move(name) }
    , treasury{ treasury }
    , army{ army }
    , map{ map }
    , id{ soh::player::instance_count }
    , position{ 0, 0 }
    , rng{ std::random_device{}() }
    , visualization{ visualization }
    , thread{ &soh::player::routine, this }
{
    ++soh::player::instance_count;
    visualization.add_player(id, this->name);
}

soh::player::~player()
{
    --soh::player::instance_count;
    visualization.update_player_info(id, soh::player_state::finish, 1.0f);
    thread.join();
}

void soh::player::routine() 
{
    while (true) 
    {
        std::unique_lock<std::mutex> lk(mutex);
        cv.wait(lk, [id = this->id]{return id == current_player || gameover;});
        
        if (gameover) {
            return;
        }

        bool found_gold{ move_to_gold() };
        collect_gold(found_gold);
        bool found_opponent{ move_to_opponent() };
        fight_opponent(found_opponent);
        visualization.update_player_info(id, soh::player_state::waiting, 0.0f);

        if (!found_gold && !found_opponent) {
            gameover = true;
        }

        current_player = (current_player + 1) % instance_count;
    
        lk.unlock();
        cv.notify_all();
    }
}

bool soh::player::move_to_gold()
{
    visualization.update_player_info(id, soh::player_state::moving, 0.0f);

    auto treasure_position = map.nearest_treasure(position);
    if (treasure_position != position)
    {
        visualization.update_tile(position.y, position.x, map.get_tile(position));

        std::this_thread::sleep_for(std::chrono::milliseconds(soh::params::player_move_quantum));

        position = treasure_position;
        visualization.update_tile(position.y, position.x, id, true);

        return true;
    }
    else
    {
        return false;
    }
}

void soh::player::collect_gold(bool found_gold)
{
    if (found_gold) 
    {
        visualization.update_player_info(id, soh::player_state::collecting, 0.0f);

        map.delete_treasure(position);

        std::scoped_lock lk{ treasury.mutex };

        static thread_local std::uniform_int_distribution dist_sleep{ 1, soh::params::player_collect_max_periods };
        const auto sleep_period{ dist_sleep(rng) };
        for (int i = 0; i < sleep_period; ++i)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds{ soh::params::player_collect_quantum });

            auto progress = i / static_cast<float>(sleep_period);
            visualization.update_player_info(id, soh::player_state::collecting, progress);
        }

        static thread_local std::uniform_int_distribution dist_gold{ 1, soh::params::gold_mine_dig_max_gold };

        treasury.gold += dist_gold(rng);
    }
}

bool soh::player::move_to_opponent()
{
    visualization.update_player_info(id, soh::player_state::moving, 0.0f);

    auto creature_position = map.nearest_creature(position);
    if (creature_position != position)
    {
        visualization.update_tile(position.y, position.x, map.get_tile(position));

        std::this_thread::sleep_for(std::chrono::milliseconds(soh::params::player_move_quantum));

        position = creature_position;
        visualization.update_tile(position.y, position.x, id, true);

        return true;
    }
    else
    {
        return false;
    }
}

void soh::player::fight_opponent(bool found_opponent)
{
    if (found_opponent) 
    {
        visualization.update_player_info(id, soh::player_state::fighting, 0.0f);

        {
            std::scoped_lock lk{ army.mutex };

            if (army.size <= 0)
            {
                return;
            }

            static thread_local std::uniform_int_distribution dist_sleep{ 1, soh::params::player_fight_max_periods };
            const auto sleep_period{ dist_sleep(rng) };
            for (int i = 0; i < sleep_period; ++i)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds{ soh::params::player_fight_quantum });

                auto progress = i / static_cast<float>(sleep_period);
                visualization.update_player_info(id, soh::player_state::fighting, progress);
            }

            army.size -= (army.size * soh::params::player_fight_max_loss);
        }

        map.delete_creature(position);
    }
}