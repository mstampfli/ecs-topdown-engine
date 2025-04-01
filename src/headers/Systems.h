#pragma once

#include "Components.h"
#include "Events.h"

#include <vector>
#include <memory>
#include <unordered_map>
#include <unordered_set>

class EntityManager;
class EventBus;

class System {
public:
    virtual void update(float dt) = 0;
    virtual ~System() = default;
    virtual void initialize(EntityManager* em, EventBus* eb); 
protected:
    EntityManager* entityManager = nullptr;
    EventBus* eventBus = nullptr;
};

class MovementSystem : public System {
public:
    void update(float dt) override;
    virtual ~MovementSystem() = default;
protected:
    EntityManager* myEntityManager;
};

class StatusSystem : public System {
public:
    void update(float dt) override;
    void initialize(EntityManager* em, EventBus* eb) override;
    void registerStatus(std::type_index status, std::function<void(StatusAppliedEvent*)> handler);
    const std::vector<StatusAppliedEvent*>& getActiveStatuses() const;
    virtual ~StatusSystem() = default;

protected: 

    void addStatus(StatusAppliedEvent* status);
    void removeStatus(StatusAppliedEvent* status);
    std::vector<StatusAppliedEvent*> activeStatuses;
    bool isStatusEqual(const StatusAppliedEvent* status1, const StatusAppliedEvent* status2);
    
    std::unordered_map <std::type_index, std::function<void(StatusAppliedEvent*)>> statusHandlers;

};


class CollisionSystem : public System {
public:
    // Called every frame to check for collisions between entities.
    void update(float dt) override;
    
    bool isColliding(Entity a, Entity b) const;
    const std::unordered_map<Entity, std::unordered_set<Entity>>& getCollisions();

protected:
    // Basic AABB collision check between two entities.
    bool checkAABBCollision(Entity entity1, Entity entity2);
    std::unordered_map<Entity, std::unordered_set<Entity>> currentCollisions;
    std::unordered_map<Entity, std::unordered_set<Entity>> newCollisions;
    std::unordered_map<Entity, std::unordered_set<Entity>> endedCollisions;
};
