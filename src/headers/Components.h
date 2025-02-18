#pragma once
#include <unordered_map>
#include "Entity.h"

struct Position {
    float x,y;
};

struct Velocity {
    float vx = 0, vy = 0;
};

extern std::unordered_map<Entity, Position> positionComponents;
extern std::unordered_map<Entity, Velocity> velocityComponents;