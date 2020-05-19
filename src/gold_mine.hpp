#pragma once

#include <random>
#include <string>
#include <thread>

#include "treasury.hpp"

namespace soh
{

class gold_mine
{
public:
    gold_mine(std::string name, soh::treasury& treasury);
    ~gold_mine();

private:
    void routine();
    int dig();
    void transport(int gold_amount);

    std::string name;
    soh::treasury& treasury;
    std::default_random_engine rng;
    std::thread thread;
};
    
} // namespace soh
