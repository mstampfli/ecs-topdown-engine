#include "headers/Behaviours.h"
#include "headers/Entity.h"
#include "headers/Components.h"
#include "headers/Systems.h"
#include "headers/ResourceManager.h"

#include <windows.h>
#include <iostream>
#include <cmath>

extern ResourceManager& rm;

void PlayerBehaviour::update(Entity entity, float dt) {
    if (rm.velocityComponents.find(entity) != rm.velocityComponents.end()) {
        
        float speed = 180.0f;
        float xSpeed = 0.0f;
        float ySpeed = 0.0f;

        rm.velocityComponents[entity].vx = 0; 
        rm.velocityComponents[entity].vy = 0;

        if (GetAsyncKeyState('W')) ySpeed = -speed;
        if (GetAsyncKeyState('A')) xSpeed = -speed;
        if (GetAsyncKeyState('S')) ySpeed = speed;
        if (GetAsyncKeyState('D')) xSpeed = speed;

        float vectorLength = std::sqrt(xSpeed * xSpeed + ySpeed * ySpeed);
        if (vectorLength > 0.01f) {
            rm.velocityComponents[entity].vx = xSpeed / vectorLength * speed;
            rm.velocityComponents[entity].vy = ySpeed / vectorLength * speed;
        } 
    }
}

void EnemyBehaviour::update(Entity entity, float dt) {
    if (rm.velocityComponents.find(entity) != rm.velocityComponents.end()) {
        
        float speed = 100.0f;

        
        float xDistance = rm.positionComponents[rm.player].x - rm.positionComponents[entity].x;
        float yDistance = rm.positionComponents[rm.player].y - rm.positionComponents[entity].y;

        float vectorLength = std::sqrt(xDistance * xDistance + yDistance * yDistance);

        if (vectorLength > 0.01f) {
            float normX = xDistance / vectorLength; // Normalize x
            float normY = yDistance / vectorLength; // Normalize y
            
            // Apply velocity in the direction of the player
            rm.velocityComponents[entity].vx = normX * speed;
            rm.velocityComponents[entity].vy = normY * speed;
        } else {
            rm.velocityComponents[entity].vx = 0; 
            rm.velocityComponents[entity].vy = 0;
        }

    } 
}  

