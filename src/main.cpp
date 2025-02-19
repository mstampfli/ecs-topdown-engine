#include "headers/Behaviours.h"
#include "headers/Components.h"
#include "headers/Entity.h"
#include "headers/Systems.h"
#include "headers/Render.h"
#include "headers/ResourceManager.h"

#include "../graphicslib/src/Window.h" 
#include "../graphicslib/src/Shader.h" 
#include "../graphicslib/src/Mesh.h" 
#include "../graphicslib/src/Texture.h"

#include <iostream>
#include <vector>



ResourceManager& rm = ResourceManager::getInstance();


int main() {

    Entity player = rm.entityManager.createEntity();
    rm.positionComponents[player] = { 0, 0 };
    rm.velocityComponents[player] = { 0, 0 };
    rm.sizeComponents[player] = {50, 50};

    rm.behaviours[player] = std::make_shared<PlayerBehaviour>();
    
    initRendering();

    rm.textureComponents[player] = rm.textures["player"];

    rm.window.updateFunction = renderPlayers;
    
    rm.window.run();

    std::cin.get();

    rm.window.close();

    return 0;
}






