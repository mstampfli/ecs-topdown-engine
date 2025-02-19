#pragma once
#include <cstdint>
#include <unordered_set>

using Entity = std::uint32_t;

class EntityManager {
    public:
        Entity createEntity();
        void destroyEntity(Entity entity);
        
        bool entityExists(Entity entity) const;

        const std::unordered_set<Entity>& getEntities() const; // Read-only access

    
    private:
        std::unordered_set<Entity> activeEntities; // Stores all created entity IDs
        Entity nextEntityID = 0;
    };