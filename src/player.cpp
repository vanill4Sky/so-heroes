#include "player.hpp"

#include <algorithm>
#include <chrono>
#include <iostream>
#include <mutex>
#include <condition_variable>

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
    , visualization{ visualization }
    , thread{ &soh::player::routine, this }
{
    ++soh::player::instance_count;
    visualization.add_player(id, this->name);
}

soh::player::~player()
{
    --soh::player::instance_count;
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
        visualization.update_player_info(id, soh::player_state::waiting);

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
    visualization.update_player_info(id, soh::player_state::moving);
    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    return true;
}

void soh::player::collect_gold(bool found_gold)
{
    if (found_gold) {
        visualization.update_player_info(id, soh::player_state::collecting);
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
}

bool soh::player::move_to_opponent()
{
    visualization.update_player_info(id, soh::player_state::moving);
    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    return true;
}

void soh::player::fight_opponent(bool found_opponent)
{
    if (found_opponent) {
        visualization.update_player_info(id, soh::player_state::fighting);
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
}