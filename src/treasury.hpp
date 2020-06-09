#pragma once

#include <atomic>
#include <mutex>

namespace soh
{

struct treasury
{
    std::atomic<bool> ready{ false };
    std::mutex mutex;
    int gold{ 0 };
};

} // namespace soh