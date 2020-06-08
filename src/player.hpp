#pragma once

#include <condition_variable>
#include <string>
#include <thread>

#include "army.hpp"
#include "treasury.hpp"
#include "map.hpp"
#include "visualization.hpp"
#include "utils.hpp"

namespace soh
{

class player
{
public:
    player(std::string name, soh::treasury& treasury, 
        soh::army& army, soh::map& map, soh::visualization& visualization);
    player(const player&) = delete;
    player(player&&) = default;
    ~player();

    player& operator=(const player&) = delete;
    player& operator=(player&&) = delete;

    void routine();
    bool move_to_gold();
    void collect_gold(bool found_gold);
    bool move_to_opponent();
    void fight_opponent(bool found_opponent);

    static int instance_count;
    static int current_player;
    static bool gameover;
    static std::mutex mutex;
    static std::condition_variable cv;

private:
    std::string name;
    utils::vec2<int> position;
    soh::treasury& treasury;
    soh::army& army;
    soh::map& map;
    int id;
    soh::visualization& visualization;
    std::thread thread;
};

} // namespace soh

