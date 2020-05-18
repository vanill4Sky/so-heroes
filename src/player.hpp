#pragma once

#include <condition_variable>
#include <string>
#include <thread>

#include "army.hpp"
#include "treasury.hpp"
#include "map.hpp"

namespace soh
{

class player
{
public:
    player(std::string name, soh::treasury& treasury, 
        soh::army& army, soh::map& map);
    ~player();

    void routine();

    static int instance_count;
    static int current_player;
    static std::mutex mutex;
    static std::condition_variable cv;

private:
    std::string name;
    soh::treasury& treasury;
    soh::army& army;
    soh::map& map;
    int id;
    std::thread thread;
};

} // namespace soh

