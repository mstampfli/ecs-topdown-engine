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

class ResourceManager {
public:
    static ResourceManager& getInstance(); // Singleton access
    Window window;
    std::unordered_map<std::string, Shader> shaders; // Store loaded shaders
    std::unordered_map<Entity, Position> positionComponents;
    std::unordered_map<Entity, Velocity> velocityComponents;
    std::unordered_map<Entity, Size> sizeComponents;
    std::unordered_map<Entity, std::unique_ptr<EntityBehaviour>> behaviours;
    std::vector<Mesh*> drawObjects;
    EntityManager entityManager;
    std::unique_ptr<Shader> shader; 
    std::unique_ptr<Texture> texture;


private:
    ResourceManager() = default; // Private constructor
    
};
