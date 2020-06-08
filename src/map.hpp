#pragma once

#include <mutex>
#include <vector>

namespace soh
{
class map
{
public:
    map(size_t width, size_t height, 
        float maxTreasures, float maxCreatures);

    std::mutex mutex;

    size_t get_width() const;
    size_t get_height() const;
    const std::vector<int>& get_map() const;
 
private:
    void generate_level(float maxTreasures, float maxCreatures);

    size_t width;
    size_t height;
    std::vector<int> level;
};
   
} // namespace soh