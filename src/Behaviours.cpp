#include "headers/Behaviours.h"
#include "headers/Entity.h"
#include "headers/Components.h"
#include "headers/Systems.h"
#include "headers/ResourceManager.h"

#include <windows.h>
#include <iostream>

extern ResourceManager& rm;

void PlayerBehaviour::update(Entity entity, float dt) {
    if (rm.velocityComponents.find(entity) != rm.velocityComponents.end()) {
        
        float speed = 5.0f;

        rm.velocityComponents[entity].vx = 0; 
        rm.velocityComponents[entity].vy = 0;

        if (GetAsyncKeyState('W')) rm.velocityComponents[entity].vy = -speed;
        if (GetAsyncKeyState('A')) rm.velocityComponents[entity].vx = -speed;
        if (GetAsyncKeyState('S')) rm.velocityComponents[entity].vy = speed;
        if (GetAsyncKeyState('D')) rm.velocityComponents[entity].vx = speed;
    }
}
