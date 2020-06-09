#include "gold_mine.hpp"

#include <algorithm>

#include "configuration.hpp"

soh::gold_mine::gold_mine(size_t id, std::string name, 
    soh::treasury& treasury, soh::visualization& visualization)
    : id{ id }
    , name{ std::move(name) }
    , treasury{ treasury }
    , rng{ std::random_device{}() }
    , extracted_amount{ 0 }
    , visualization{ visualization }
    , thread{ &soh::gold_mine::routine, this }
{
    visualization.add_gold_mine(id, this->name);
}

soh::gold_mine::~gold_mine()
{
    thread.join();
}

void soh::gold_mine::routine()
{
    while (!treasury.ready);

    do
    {
        transport(dig());
    } while (treasury.ready);

    visualization.update_gold_mine_info(id, soh::gold_mine_state::finish, 1.0f, extracted_amount);
}

int soh::gold_mine::dig()
{
    visualization.update_gold_mine_info(id, soh::gold_mine_state::digging, 0.0f, extracted_amount);

    static thread_local std::uniform_int_distribution dist_sleep{ 1, soh::params::gold_mine_dig_periods };
    const auto sleep_period{ dist_sleep(rng) };
    for (int i = 0; i < sleep_period; ++i)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds{ soh::params::gold_mine_dig_quantum });

        auto progress = i / static_cast<float>(sleep_period);
        visualization.update_gold_mine_info(id, soh::gold_mine_state::digging, progress, extracted_amount);
    }

    static thread_local std::uniform_int_distribution dist_gold{ 1, soh::params::gold_mine_dig_max_gold };

    return dist_gold(rng);
}

void soh::gold_mine::transport(int gold_amount)
{
    visualization.update_gold_mine_info(id, soh::gold_mine_state::waiting, 0.0f, extracted_amount);
    std::scoped_lock lk{ treasury.mutex };
    visualization.update_gold_mine_info(id, soh::gold_mine_state::transporting, 0.0f, extracted_amount);

    static thread_local std::uniform_int_distribution dist{ 1, soh::params::gold_mine_transport_max_periods };
    const auto sleep_period{ dist(rng) };
    for (int i = 0; i < sleep_period; ++i)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds{ soh::params::gold_mine_transport_quantum });

        auto progress = i / static_cast<float>(sleep_period);
        visualization.update_gold_mine_info(id, soh::gold_mine_state::transporting, progress, extracted_amount);
    }

    extracted_amount += gold_amount;
    treasury.gold += gold_amount;

    visualization.update_gold_amount(treasury.gold);
}
