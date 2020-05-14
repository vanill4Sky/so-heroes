#include "map.hpp"

#include <cassert>

soh::map::map(size_t width, size_t height, 
    float maxTreasures, float maxCreatures)
    : width{ width }
    , height{ height }
{
    generate_level(maxTreasures, maxCreatures);
}

void soh::map::generate_level(float maxTreasures, float maxCreatures)
{
    assert(maxTreasures >= 0.0f);
    assert(maxCreatures >= 0.0f);

    level.resize(width * height);

    auto entitiesCount{ static_cast<unsigned int>(level.size() * 0.5) };
    auto maxTreasuresCount{ static_cast<unsigned int>(maxTreasures * entitiesCount) };
    auto maxCreaturesCount{ static_cast<unsigned int>(maxCreatures * entitiesCount) };
}