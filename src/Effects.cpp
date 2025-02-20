#include "headers/Effects.h"
#include "headers/ResourceManager.h"
#include <memory>

extern ResourceManager& rm;

void EnemyTouchingPlayerStatus::applyEffect() {

    rm.eventSystem.addEvent(std::make_unique<DamageEvent>(target, rm.player, damageAmount));

}