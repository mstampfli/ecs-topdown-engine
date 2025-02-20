#pragma once
#include "Components.h"
#include "Events.h"
#include "Effects.h"
#include <vector>
#include <memory>
#include <vector>

class EventSystem {
public:
    std::vector<std::unique_ptr<EntityEvent>> eventQueue;

    void addEvent(std::unique_ptr<EntityEvent> event);

    void update();

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

class StatusSystem {
public:
    std::vector<std::shared_ptr<StatusEffect>> activeEffects;

    void update(float dt);

    void addStatusEffect(std::unique_ptr<StatusEffect> effect);
};
    
bool checkAABBCollision(Entity entity1, Entity entity2);