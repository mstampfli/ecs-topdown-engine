#pragma once
#include "Entity.h"

class EntityEvent {
public:
    Entity attacker;  // Entity that caused the event
    Entity target;    // Entity that receives the event

    EntityEvent(Entity attacker, Entity target) 
        : attacker(attacker), target(target) {}

    virtual ~EntityEvent() = default; // Ensure proper cleanup
};

class DamageEvent : public EntityEvent {
public:
    float damageAmount;

    DamageEvent(Entity attacker, Entity target, float damage) 
        : EntityEvent(attacker, target), damageAmount(damage) {}
};