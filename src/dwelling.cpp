#include "dwelling.hpp"

#include <algorithm>

soh::dwelling::dwelling(size_t id, std::string name, soh::treasury &treasury, soh::army &army)
    : id{ id }
    , name{std::move(name)}
    , treasury{treasury}
    , army{army}
    , rng{std::random_device{}()}
    , thread{&soh::dwelling::routine, this}
{
}

soh::dwelling::~dwelling()
{
    thread.join();
}

void soh::dwelling::routine()
{
    add_creatures(produce(10));
}

int soh::dwelling::produce(int creature_price)
{
    constexpr int production_duration{20};

    std::scoped_lock lk{treasury.mutex};

    static thread_local std::uniform_int_distribution dist{1, 20};
    const auto sleep_period{dist(rng)};
    for (int i = 0; i < sleep_period; ++i)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds{production_duration});
    }

    int max_creature_count{treasury.gold / creature_price};
    int produced_creature_count = std::min(dist(rng), max_creature_count);

    treasury.gold -= produced_creature_count * creature_price;

    return produced_creature_count;
}

void soh::dwelling::add_creatures(int creature_count)
{
    constexpr int addition_duration{20};

    if (creature_count > 0)
    {
        std::scoped_lock lk{army.mutex};

        static thread_local std::uniform_int_distribution dist{1, 20};
        const auto sleep_period{dist(rng)};
        for (int i = 0; i < sleep_period; ++i)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds{addition_duration});
        }

        army.size += creature_count;
    }
}
