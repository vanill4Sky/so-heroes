#pragma once

#include <mutex>

namespace soh
{

struct army
{
    std::mutex mutex;
    int size{ 0 };
};

} // namespace soh
