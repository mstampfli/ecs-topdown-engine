#include "headers/Behaviours.h"
#include "headers/Entity.h"
#include "headers/Components.h"
#include "headers/Systems.h"

#include <windows.h>
#include <iostream>

std::unordered_map<Entity, std::unique_ptr<EntityBehaviour>> behaviours;

void PlayerBehaviour::update(Entity entity, float dt) {
    if (velocityComponents.find(entity) != velocityComponents.end()) {
        
        float speed = 5.0f;

        velocityComponents[entity].vx = 0; 
        velocityComponents[entity].vy = 0;

        if (GetAsyncKeyState('W')) velocityComponents[entity].vy = -speed;
        if (GetAsyncKeyState('A')) velocityComponents[entity].vx = -speed;
        if (GetAsyncKeyState('S')) velocityComponents[entity].vy = speed;
        if (GetAsyncKeyState('D')) velocityComponents[entity].vx = speed;
    }
}
