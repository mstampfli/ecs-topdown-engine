#pragma once
#include "Components.h"
#include "Events.h"
#include <vector>
#include <memory>
#include <vector>

class EventSystem {
public:
    std::vector<std::unique_ptr<EntityEvent>> eventQueue;

    void addEvent(std::unique_ptr<EntityEvent> event);

    void update(float dt);

};

class MovementSystem {
public:
    virtual void update(float dt);
    ~MovementSystem() = default;
};

class CombatSystem {
public:
    virtual void update(float dt);
    ~CombatSystem() = default;
};

bool checkAABBCollision(Entity entity1, Entity entity2);