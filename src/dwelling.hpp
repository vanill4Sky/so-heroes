#pragma once

#include <random>
#include <string>
#include <thread>

#include "treasury.hpp"
#include "army.hpp"
#include "visualization.hpp"

namespace soh
{

class dwelling
{
public:
    dwelling(size_t id, std::string name, soh::treasury& treasury, 
        soh::army& army, soh::visualization& visualization);
    dwelling(const dwelling&) = delete;
    dwelling(dwelling&&) = default;
    ~dwelling();

    dwelling& operator=(const dwelling&) = delete;
    dwelling& operator=(dwelling&&) = delete;

private:
    void routine();
    int produce(int creature_price);
    void add_creatures(int creature_count);

    size_t id;
    std::string name;
    soh::treasury& treasury;
    soh::army& army;
    std::default_random_engine rng;
    size_t produced_amount;
    soh::visualization& visualization;
    std::thread thread;
};
    
} // namespace soh
