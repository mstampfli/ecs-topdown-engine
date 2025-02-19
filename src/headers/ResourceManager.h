#pragma once
#include <unordered_map>
#include <string>
#include "Components.h"

#include "../../graphicslib/src/Shader.h"
#include "../../graphicslib/src/Window.h"
#include "../../graphicslib/src/Mesh.h"
#include "../../graphicslib/src/Texture.h"
#include "Entity.h"
#include "Behaviours.h"
#include "Systems.h"

class ResourceManager {
public:
    static ResourceManager& getInstance(); // Singleton access
    Window window;
    std::unordered_map<Entity, std::shared_ptr<Position>> positionComponents;
    std::unordered_map<Entity, std::shared_ptr<Velocity>> velocityComponents;
    std::unordered_map<Entity, std::shared_ptr<Size>> sizeComponents;
    std::unordered_map<Entity, std::shared_ptr<EntityBehaviour>> behaviours;
    EntityManager entityManager;

    Entity player;
    MovementSystem movementSystem;
    EventSystem eventSystem;
    CombatSystem combatSystem;

    std::unordered_map<std::string, std::shared_ptr<Texture>> textures;
    std::unordered_map<std::string, std::shared_ptr<Shader>> shaders;
  
    std::unordered_map<Entity, std::shared_ptr<Shader>> shaderComponents;
    std::unordered_map<Entity, std::shared_ptr<Texture>> textureComponents;
    std::unordered_map<Entity, std::shared_ptr<Health>> healthComponents;
    std::unordered_map<Entity, std::unique_ptr<Mesh>> meshComponents;

    std::vector<Entity> sortedDrawEntities;

private:
    ResourceManager() = default; // Private constructor
    
};
