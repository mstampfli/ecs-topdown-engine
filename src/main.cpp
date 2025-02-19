#include "headers/Behaviours.h"
#include "headers/Components.h"
#include "headers/Entity.h"
#include "headers/Systems.h"
#include "headers/Render.h"
#include "headers/ResourceManager.h"
#include "headers/DeltaTime.h"

#include "../graphicslib/src/Window.h" 
#include "../graphicslib/src/Shader.h" 
#include "../graphicslib/src/Mesh.h" 
#include "../graphicslib/src/Texture.h"

#include <iostream>
#include <vector>



ResourceManager& rm = ResourceManager::getInstance();
DeltaTime& dt = DeltaTime::getInstance();

int main() {

    rm.player = rm.entityManager.createEntity();
    rm.positionComponents[rm.player] = std::make_shared<Position>(Position{0, 0});
    rm.velocityComponents[rm.player] = std::make_shared<Velocity>(Velocity{0, 0});
    rm.healthComponents[rm.player] = std::make_shared<Health>(Health{100, 100});
    rm.sizeComponents[rm.player] = std::make_shared<Size>(Size{50, 50});
    rm.behaviours[rm.player] = std::make_shared<PlayerBehaviour>();

    Entity enemy = rm.entityManager.createEntity();
    rm.positionComponents[enemy] = std::make_shared<Position>(Position{200, 200});
    rm.velocityComponents[enemy] = std::make_shared<Velocity>(Velocity{0, 0});
    rm.healthComponents[enemy] = std::make_shared<Health>(Health{100, 100});
    rm.sizeComponents[enemy] = std::make_shared<Size>(Size{50, 50});
    rm.behaviours[enemy] = std::make_shared<EnemyBehaviour>();
    
    initRendering();

    rm.textureComponents[rm.player] = rm.textures["player"];

    rm.textureComponents[enemy] = rm.textures["player"];

    rm.window.updateFunction = renderPlayers;
    
    rm.window.run();

    std::cin.get();

    rm.window.close();

    return 0;
}






