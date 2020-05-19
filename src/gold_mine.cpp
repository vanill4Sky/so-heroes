#include "gold_mine.hpp"

#include <algorithm>

soh::gold_mine::gold_mine(std::string name, soh::treasury& treasury)
    : name{ std::move(name) }
    , treasury{ treasury }
    , rng{ std::random_device{}() }
    , thread{ &soh::gold_mine::routine, this }
{ }

soh::gold_mine::~gold_mine()
{
    thread.join();
}

void soh::gold_mine::routine()
{
    transport(dig());
}

int soh::gold_mine::dig()
{
    constexpr int dig_duration{ 20 };

    static thread_local std::uniform_int_distribution dist{ 1, 20 };
    const auto sleep_period{ dist(rng) };
    for (int i = 0; i < sleep_period; ++i)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds{ dig_duration });
    }

    return dist(rng) * 100;
}

void soh::gold_mine::transport(int gold_amount)
{
    constexpr int transport_duration{ 20 };

    std::scoped_lock lk{ treasury.mutex };

    static thread_local std::uniform_int_distribution dist{ 1, 20 };
    const auto sleep_period{ dist(rng) };
    for (int i = 0; i < sleep_period; ++i)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds{ transport_duration });
    }

    treasury.gold += gold_amount;
}
