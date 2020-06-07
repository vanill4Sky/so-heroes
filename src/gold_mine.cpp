#include "gold_mine.hpp"

#include <algorithm>

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
}

int soh::gold_mine::dig()
{
    constexpr int digging_duration{ 20 };

    visualization.update_gold_mine_info(id, soh::gold_mine_state::digging, extracted_amount);

    static thread_local std::uniform_int_distribution dist{ 1, 20 };
    const auto sleep_period{ dist(rng) };
    for (int i = 0; i < sleep_period; ++i)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds{ digging_duration });
    }

    return dist(rng) * 100;
}

void soh::gold_mine::transport(int gold_amount)
{
    constexpr int transport_duration{ 20 };

    visualization.update_gold_mine_info(id, soh::gold_mine_state::waiting, extracted_amount);
    std::scoped_lock lk{ treasury.mutex };
    visualization.update_gold_mine_info(id, soh::gold_mine_state::transporting, extracted_amount);

    static thread_local std::uniform_int_distribution dist{ 1, 20 };
    const auto sleep_period{ dist(rng) };
    for (int i = 0; i < sleep_period; ++i)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds{ transport_duration });
    }

    extracted_amount += gold_amount;
    treasury.gold += gold_amount;
}
