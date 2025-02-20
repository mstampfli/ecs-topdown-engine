#include "headers/Systems.h"
#include "headers/Entity.h"
#include "headers/Components.h"
#include "headers/ResourceManager.h"
#include "headers/Events.h"
#include <memory>
#include <algorithm>

extern ResourceManager& rm;

void EventSystem::update() {
    for (auto& event : eventQueue) {
        (*event).process();
    }
    eventQueue.clear(); 
}

void EventSystem::addEvent(std::unique_ptr<EntityEvent> event) {
    eventQueue.push_back(std::move(event));
}

void MovementSystem::update(float dt) {
    for (auto& entity : rm.entityManager.getEntities()) {
        if (rm.positionComponents.find(entity) != rm.positionComponents.end()) {
            rm.positionComponents[entity].x += rm.velocityComponents[entity].vx * dt;
            rm.positionComponents[entity].y += rm.velocityComponents[entity].vy * dt;
        }
    }
}

void CombatSystem::update(float dt) {
    for (auto& entity : rm.entityManager.getEntities()) {
        applyEnemyTouchStatus(entity);
    }
}

void StatusSystem::update(float dt) {
    for (auto it = activeEffects.begin(); it != activeEffects.end();) {
        auto& effect = *it;
        (*effect).timeSinceLastTick += dt;

        // Apply effect if enough time has passed
        if ((*effect).timeSinceLastTick >= (*effect).interval) {
            (*effect).applyEffect();
            (*effect).timeSinceLastTick = 0; // Reset timer
        }

        // Remove effect if expired
        if (!(*effect).condition) {
            it = activeEffects.erase(it);
        } else {
            ++it;
        }
    }
}

void StatusSystem::addStatusEffect(std::unique_ptr<StatusEffect> effect) {
    activeEffects.push_back(std::move(effect));
}

void applyEnemyTouchStatus(Entity entity) {
    // Check if entity is an enemy
    if (rm.behaviours.find(entity) != rm.behaviours.end() &&
    dynamic_cast<EnemyBehaviour*>(rm.behaviours[entity].get())) {
    
        if (checkAABBCollision(entity, rm.player)) {
            bool effectAlreadyActive = std::any_of(
                rm.statusSystem.activeEffects.begin(),
                rm.statusSystem.activeEffects.end(),
                [entity](const std::shared_ptr<StatusEffect>& effect) {
                    return (*effect).source == entity && (*effect).target == rm.player &&
                            dynamic_cast<EnemyTouchingPlayerStatus*>(effect.get()) != nullptr;
                }
            );
            
            if (effectAlreadyActive) {
                return;
            }
            
            rm.statusSystem.addStatusEffect(std::make_unique<EnemyTouchingPlayerStatus>(entity, rm.player, 1.0f, [entity]() -> bool { return checkAABBCollision(entity, rm.player); }, 10)); 
        }
    }
}

bool checkAABBCollision(Entity entity1, Entity entity2) {
    float x1 = rm.positionComponents[entity1].x;
    float y1 = rm.positionComponents[entity1].y;
    float w1 = rm.sizeComponents[entity1].w;
    float h1 = rm.sizeComponents[entity1].h;

    float x2 = rm.positionComponents[entity2].x;
    float y2 = rm.positionComponents[entity2].y;
    float w2 = rm.sizeComponents[entity2].w;
    float h2 = rm.sizeComponents[entity2].h;

    // AABB Collision check (top-left origin)
    return (x1 < x2 + w2 &&
            x1 + w1 > x2 &&
            y1 < y2 + h2 &&
            y1 + h1 > y2);
}


