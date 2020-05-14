#pragma once

#include <mutex>

namespace soh
{

struct treasury
{
    std::mutex mutex;
    int gold{ 0 };
};

} // namespace soh