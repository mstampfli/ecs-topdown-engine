#include "headers/Entity.h"

Entity EntityManager::createEntity() {
    Entity newEntity = nextEntityID++;
    activeEntities.insert(newEntity);
    return newEntity;
}

void EntityManager::destroyEntity(Entity entity) {
    activeEntities.erase(entity);
}

bool EntityManager::entityExists(Entity entity) const {
    return activeEntities.find(entity) != activeEntities.end();
}

const std::unordered_set<Entity>& EntityManager::getEntities() const {
    return activeEntities;
}
