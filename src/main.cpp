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
    rm.positionComponents[rm.player] = {0, 0};
    rm.velocityComponents[rm.player] = {0, 0};
    rm.healthComponents[rm.player] = {100, 100};
    rm.sizeComponents[rm.player] = {50, 50};
    rm.behaviours[rm.player] = std::make_shared<PlayerBehaviour>();
    rm.typeComponents[rm.player] = std::make_shared<TypeComponent>(TypeComponent{EntityType::PLAYER});

    Entity enemy = rm.entityManager.createEntity();
    rm.positionComponents[enemy] = {200, 200};
    rm.velocityComponents[enemy] = {0, 0};
    rm.healthComponents[enemy] = {100, 100};
    rm.sizeComponents[enemy] = {50, 50};
    rm.behaviours[enemy] = std::make_shared<EnemyBehaviour>();
    rm.typeComponents[enemy] = std::make_shared<TypeComponent>(TypeComponent{EntityType::ENEMY});
    
    initRendering();

    rm.textureComponents[rm.player] = rm.textures["player"].get();

    rm.textureComponents[enemy] = rm.textures["player"].get();

    rm.window.updateFunction = renderPlayers;
    
    rm.window.run();

    std::cin.get();

    rm.window.close();

    return 0;
}






