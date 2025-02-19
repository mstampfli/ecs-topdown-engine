#include "headers/Systems.h"
#include "headers/Entity.h"
#include "headers/Components.h"
#include "headers/ResourceManager.h"

extern ResourceManager& rm;

void updateMovement(float dt) {
    for (auto& entity : rm.entityManager.getEntities()) {
        if (rm.positionComponents.find(entity) != rm.positionComponents.end()) {
            rm.positionComponents[entity].x += rm.velocityComponents[entity].vx * dt;
            rm.positionComponents[entity].y += rm.velocityComponents[entity].vy * dt;
        }
    }
}