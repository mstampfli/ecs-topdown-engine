#include "headers/Systems.h"
#include "headers/Entity.h"
#include "headers/Components.h"
#include "headers/Managers.h"
#include "headers/Events.h"

#include <memory>
#include <algorithm>
#include <typeinfo>


void System::initialize(EntityManager* em, EventBus* eb) {
    entityManager = em;
    eventBus = eb;
}

void MovementSystem::update(float dt) {
    for (auto& entity : entityManager->getAllEntities()) {
        Velocity* vel = entityManager->getVelocity(entity);
        Position* pos = entityManager->getPosition(entity);

        if (vel && pos) {
            pos->x += vel->vx * dt;
            pos->y += vel->vy * dt;
        }
    }
}

void StatusSystem::update(float dt) {
    for (auto it = activeStatuses.begin(); it != activeStatuses.end();) {
        
        auto& status = *it;
        if (!status) {
            ++it; 
            continue;
        }

        status->timeSinceLastTick += dt;

        // Apply effect if enough time has passed
        if (status->timeSinceLastTick >= status->interval || status->timeSinceLastTick == 0.0f) {
            handleStatusUpdate(status);
        }

        // Remove effect if expired
        if(!status->condition) continue;
        if (!status->condition()) {
            std::cout << "Deleting status: " << status << std::endl;
            status->condition = std::function<bool()>(); 
            delete status;
            it = activeStatuses.erase(it);
        } else {
            ++it;
        }
    }
}


void StatusSystem::initialize(EntityManager* em, EventBus* eb) {
    eventBus = eb;
    eventBus->subscribe<StatusAppliedEvent>([this](const StatusAppliedEvent& statusEvent) {
        StatusAppliedEvent* mutableStatus = new StatusAppliedEvent(statusEvent);
        this->addStatus(mutableStatus);
    });
}

void StatusSystem::addStatus(StatusAppliedEvent* status) {
    std::cout << "Adding status: " << status << std::endl;
    activeStatuses.push_back(status);
    handleStatusUpdate(status);
}

bool StatusSystem::isStatusEqual(const StatusAppliedEvent* status1, const StatusAppliedEvent* status2) {
    return status1 == status2;
}

void StatusSystem::removeStatus(StatusAppliedEvent* status) {
    auto it = std::remove_if(activeStatuses.begin(), activeStatuses.end(),
        [this, &status](const StatusAppliedEvent* s) {
            return isStatusEqual(s, status);
        });

    for (auto iter = it; iter != activeStatuses.end(); ++iter) {
        delete *iter;  
    }

    activeStatuses.erase(it, activeStatuses.end());
}

const std::vector<StatusAppliedEvent*>& StatusSystem::getActiveStatuses() const{
    return activeStatuses;
}

bool CollisionSystem::checkAABBCollision(Entity entity1, Entity entity2) {

    const Position* posA = entityManager->getPosition(entity1);
    const Size* sizeA = entityManager->getSize(entity1);
    const Position* posB = entityManager->getPosition(entity2);
    const Size* sizeB = entityManager->getSize(entity2);

    if (!posA || !sizeA || !posB || !sizeB) return false;

    // AABB collision check (assuming top-left origin)
    return (posA->x < posB->x + sizeB->w &&
            posA->x + sizeA->w > posB->x &&
            posA->y < posB->y + sizeB->h &&
            posA->y + sizeA->h > posB->y);
}

bool CollisionSystem::isColliding(Entity firstEntity, Entity secondEntity) const {
    
    if (currentCollisions.count(firstEntity) == 0) return false;
    if (currentCollisions.at(firstEntity).count(secondEntity) > 0) return true;
 
    return false;
}

const std::unordered_map<Entity, std::unordered_set<Entity>>& CollisionSystem::getCollisions() {
    return currentCollisions;
}

void CollisionSystem::update(float dt) {
    std::unordered_map<Entity, std::unordered_set<Entity>> allCollisionsNew;
    const std::unordered_set<Entity> entities = entityManager->getAllEntities();
    newCollisions.clear();
    endedCollisions.clear();

    for (auto iterator1 = entities.begin(); iterator1 != entities.end(); ++iterator1) {
        for (auto iterator2 = std::next(iterator1); iterator2 != entities.end(); ++iterator2) {
            if (checkAABBCollision(*iterator1, *iterator2)) {
                //std::cout << "Collision detected: " << *iterator1 << " with " << *iterator2 << std::endl;
                allCollisionsNew[*iterator1].insert(*iterator2);
                allCollisionsNew[*iterator2].insert(*iterator1);
            }
        }
    }


    for (auto& [entity, newSet] : allCollisionsNew) {
        for (Entity collider : newSet) {
            if (currentCollisions.count(entity) == 0 || currentCollisions[entity].count(collider) == 0) {
                newCollisions[entity].insert(collider);  // ✅ Fixes incorrect "new collision" tracking
            }
        }
    }

    for (auto& [entity, oldSet] : currentCollisions) {
        for (Entity collider : oldSet) {
            if (allCollisionsNew.count(entity) == 0 || allCollisionsNew[entity].count(collider) == 0) {
                endedCollisions[entity].insert(collider);
            }
        }
    }
    currentCollisions.clear();

    currentCollisions = std::move(allCollisionsNew);
    
}

