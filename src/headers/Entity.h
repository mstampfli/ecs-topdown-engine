#pragma once
#include <cstdint>

using Entity = std::uint32_t;

class EntityManager {
private:
    Entity nextEntityID = 0;
public:
    Entity createEntity();
};
