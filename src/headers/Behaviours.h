#pragma once
#include "Components.h"
#include <memory>

// Base class for behaviors
class EntityBehaviour {
public:
    virtual void update(Entity entity, float dt) = 0;
    virtual ~EntityBehaviour() = default; // Ensure proper cleanup
};

// Player-specific behavior
class PlayerBehaviour : public EntityBehaviour {
public:
    void update(Entity entity, float dt) override;
};

