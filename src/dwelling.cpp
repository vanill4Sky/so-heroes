#include "dwelling.hpp"

#include <algorithm>

soh::dwelling::dwelling(size_t id, std::string name, soh::treasury &treasury, 
    soh::army &army, soh::visualization& visualization)
    : id{ id }
    , name{std::move(name)}
    , treasury{treasury}
    , army{army}
    , rng{std::random_device{}()}
    , produced_amount{ 0 }
    , visualization{ visualization }
    , thread{&soh::dwelling::routine, this}
{
    visualization.add_dwelling(id, this->name);
}

soh::dwelling::~dwelling()
{
    thread.join();
}

void soh::dwelling::routine()
{
    while (!treasury.ready);

    do
    {
        add_creatures(produce(10));
    } while (treasury.ready);
}

int soh::dwelling::produce(int creature_price)
{
    constexpr int production_duration{20};

    visualization.update_dwelling_info(id, soh::dwelling_state::waiting, produced_amount);
    std::scoped_lock lk{treasury.mutex};
    visualization.update_dwelling_info(id, soh::dwelling_state::producing, produced_amount);

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
        visualization.update_dwelling_info(id, soh::dwelling_state::waiting, produced_amount);
        std::scoped_lock lk{army.mutex};
        visualization.update_dwelling_info(id, soh::dwelling_state::adding, produced_amount);

        static thread_local std::uniform_int_distribution dist{1, 20};
        const auto sleep_period{dist(rng)};
        for (int i = 0; i < sleep_period; ++i)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds{addition_duration});
        }

        produced_amount += creature_count;
        army.size += creature_count;
    }
}
