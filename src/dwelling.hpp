#pragma once

#include <random>
#include <string>
#include <thread>

#include "treasury.hpp"
#include "army.hpp"

namespace soh
{

class dwelling
{
public:
    dwelling(size_t id, std::string name, soh::treasury& treasury, soh::army& army);
    ~dwelling();

private:
    void routine();
    int produce(int creature_price);
    void add_creatures(int creature_count);

    size_t id;
    std::string name;
    soh::treasury& treasury;
    soh::army& army;
    std::default_random_engine rng;
    std::thread thread;
};
    
} // namespace soh
