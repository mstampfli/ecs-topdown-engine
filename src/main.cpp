#include "headers/Behaviours.h"
#include "headers/Components.h"
#include "headers/Entity.h"
#include "headers/Systems.h"
#include "headers/Render.h"

int main() {
    EntityManager entityManager;

    Entity player = entityManager.createEntity();
    positionComponents[player] = { 0, 0 };
    velocityComponents[player] = { 0, 0 };

    behaviours[player] = std::make_unique<PlayerBehaviour>();

    while (true) {
        for (auto& [entity, behaviour] : behaviours) {
            behaviour->update(entity, 1.0f);
        }

        updateMovement(1.0f);

        renderPlayers();
    }

    return 0;

}