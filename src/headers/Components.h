#pragma once
#include <unordered_map>
#include "Entity.h"

struct Position {
    float x,y;
};

struct Velocity {
    float vx = 0, vy = 0;
};

struct Size {
    float w, h;
};

struct Health {
    std::uint32_t maxHealth, currentHealth;
};

enum class EntityType {
    PLAYER,
    ENEMY,
    NPC
};

struct TypeComponent {
    EntityType type;
};