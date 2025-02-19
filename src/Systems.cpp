#include "headers/Systems.h"
#include "headers/Entity.h"
#include "headers/Components.h"
#include "headers/ResourceManager.h"

extern ResourceManager& rm;

void updateMovement(float dt) {
    for (auto& [entity, velocity] : rm.velocityComponents) {
        if (rm.positionComponents.find(entity) != rm.positionComponents.end()) {
            rm.positionComponents[entity].x += velocity.vx * dt;
            rm.positionComponents[entity].y += velocity.vy * dt;
        }
    }
}