#include "headers/Events.h"
#include "headers/ResourceManager.h"

extern ResourceManager& rm;

void DamageEvent::process() {
    if (rm.healthComponents.find(target) != rm.healthComponents.end()) {
        rm.healthComponents[target].currentHealth -= damageAmount;
    }
}