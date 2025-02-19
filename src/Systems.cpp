#include "headers/Systems.h"
#include "headers/Entity.h"
#include "headers/Components.h"
#include "headers/ResourceManager.h"
#include "headers/Events.h"
#include <memory>

extern ResourceManager& rm;

void EventSystem::update(float dt) {
    for (auto& event : eventQueue) {
        if (auto* damageEvent = dynamic_cast<DamageEvent*>(event.get())) {
            
            if (rm.healthComponents.find((*damageEvent).target) != rm.healthComponents.end()) {
                (*rm.healthComponents[(*damageEvent).target]).currentHealth -= (*damageEvent).damageAmount;
            }
        } 
    }
    eventQueue.clear(); // Clear after processing
}

void EventSystem::addEvent(std::unique_ptr<EntityEvent> event) {
    eventQueue.push_back(std::move(event));
}

void MovementSystem::update(float dt) {
    for (auto& entity : rm.entityManager.getEntities()) {
        if (rm.positionComponents.find(entity) != rm.positionComponents.end()) {
            (*rm.positionComponents[entity]).x += (*rm.velocityComponents[entity]).vx * dt;
            (*rm.positionComponents[entity]).y += (*rm.velocityComponents[entity]).vy * dt;
        }
    }
}

void CombatSystem::update(float dt) {
    for (auto& entity : rm.entityManager.getEntities()) {
        // Check if entity is an NPC
        if (rm.behaviours.find(entity) != rm.behaviours.end() &&
            dynamic_cast<EnemyBehaviour*>(rm.behaviours[entity].get())) {
            
            if (checkAABBCollision(entity, rm.player)) {
                std::unique_ptr<EntityEvent> damageEvent = std::make_unique<DamageEvent>(entity, rm.player, 100);
                rm.eventSystem.addEvent(std::move(damageEvent));
            }
        }
    }
}

bool checkAABBCollision(Entity entity1, Entity entity2) {
    float x1 = (*rm.positionComponents[entity1]).x;
    float y1 = (*rm.positionComponents[entity1]).y;
    float w1 = (*rm.sizeComponents[entity1]).w;
    float h1 = (*rm.sizeComponents[entity1]).h;

    float x2 = (*rm.positionComponents[entity2]).x;
    float y2 = (*rm.positionComponents[entity2]).y;
    float w2 = (*rm.sizeComponents[entity2]).w;
    float h2 = (*rm.sizeComponents[entity2]).h;

    // AABB Collision check (top-left origin)
    return (x1 < x2 + w2 &&
            x1 + w1 > x2 &&
            y1 < y2 + h2 &&
            y1 + h1 > y2);
}


