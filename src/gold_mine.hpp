#pragma once

#include <random>
#include <string>
#include <thread>

#include "treasury.hpp"
#include "visualization.hpp"

namespace soh
{

class gold_mine
{
public:
    gold_mine(size_t id, std::string name, soh::treasury& treasury, 
        soh::visualization& visualization);
    gold_mine(const gold_mine&) = delete;
    gold_mine(gold_mine&&) = default;
    ~gold_mine();

    gold_mine& operator=(const gold_mine&) = delete;
    gold_mine& operator=(gold_mine&&) = delete;

private:
    void routine();
    int dig();
    void transport(int gold_amount);

    size_t id;
    std::string name;
    soh::treasury& treasury;
    std::default_random_engine rng;
    size_t extracted_amount;
    soh::visualization& visualization;
    std::thread thread;
};
    
} // namespace soh
