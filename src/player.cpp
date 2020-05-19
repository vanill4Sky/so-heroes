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
    soh::army& army, soh::map& map)
    : name{ std::move(name) }
    , treasury{ treasury }
    , army{ army }
    , map{ map }
    , id{ soh::player::instance_count }
    , thread{ &soh::player::routine, this }
{
    ++soh::player::instance_count;
}

soh::player::~player()
{
    --soh::player::instance_count;
    thread.join();
}

void soh::player::routine() 
{
    std::unique_lock<std::mutex> lk(mutex);
    cv.wait(lk, [id = this->id]{return id == current_player || gameover;});
    
    if (gameover) {
        return;
    }
 
    std::cout << "Worker thread " << id << " is processing data\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "Worker thread " << id << " signals data processing completed\n";
    current_player = (current_player + 1) % instance_count;
 
    lk.unlock();
    cv.notify_all();
}