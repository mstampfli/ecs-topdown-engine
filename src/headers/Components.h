#pragma once
#include <unordered_map>
#include <string>
#include "Entity.h"

struct Position {
    float x, y, z;

    bool operator==(const Position& other) const {
        return x == other.x && y == other.y && z == other.z;
    }
};

struct Velocity {
    float vx = 0.0f, vy = 0.0f, vz = 0.0f;
};

struct Size {
    float w, h;
};



