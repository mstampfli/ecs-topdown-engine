#include "headers/Systems.h"
#include "headers/Entity.h"
#include "headers/Components.h"

void updateMovement(float dt) {
    for (auto& [entity, velocity] : velocityComponents) {
        if (positionComponents.find(entity) != positionComponents.end()) {
            positionComponents[entity].x += velocity.vx * dt;
            positionComponents[entity].y += velocity.vy * dt;
        }
    }
}