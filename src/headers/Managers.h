#pragma once
#include <unordered_map>
#include <unordered_set>
#include <string>

#include "Entity.h"
#include "Components.h"
#include "Behaviours.h"
#include "Systems.h"
#include "Events.h"

#include <memory>
#include <vector>
#include <algorithm>
#include <typeindex>
#include <unordered_map>
#include <unordered_set>

#include "../../libraries/soloud/include/soloud.h"
#include "../../libraries/soloud/include/soloud_wav.h"

#include "../../libraries/graphicslib/include/Shader.h"
#include "../../libraries/graphicslib/include/Window.h"
#include "../../libraries/graphicslib/include/Texture.h"
#include "../../libraries/graphicslib/include/Renderer.h"

using EntityType = std::uint32_t;

class InputManager {
public:
    InputManager() = default;
    ~InputManager() = default;

    void update();
    bool isKeyDown(int key);
    bool isKeyUp(int key);
    bool isKeyPressed(int key);
    bool isKeyReleased(int key);

    void initialize(Window* window);

protected:
    Window* window;
    std::unordered_map<int, bool> previousInputs;
    std::unordered_map<int, bool> currentInputs;

    void storeKeyInputs(int key, int action);
    static void keyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods);
};

class RenderManager : public Renderer{
public:
    Window* window;
    RenderManager(Window* window) : Renderer(window), window(window) {}
};

class AudioManager {
public:

    void initialize();

    void loadSound(const std::string& name, const std::string& filePath);
    
    SoLoud::handle playSound(const std::string& name, float volume = 1.0f);

    void stopSound(SoLoud::handle activeSound);

    void stopAll();
    void setVolume(SoLoud::handle activeSound, float volume);
    bool isPlaying(SoLoud::handle activeSound);

    void pauseSound(SoLoud::handle handle);
    void resumeSound(SoLoud::handle handle);
    bool isPaused(SoLoud::handle handle);

    std::unordered_map<std::string, std::unique_ptr<SoLoud::Wav>>& getAllSounds();

    ~AudioManager() {
        audioEngine.deinit();
    }
private:
    SoLoud::Soloud audioEngine;
    std::unordered_map<std::string, std::unique_ptr<SoLoud::Wav>> sounds;
};

class EventBus {
public:

    template<typename EventTemplate>
    void subscribe(std::function<void(const EventTemplate&)> handler) {
        static_assert(std::is_base_of<Event, EventTemplate>::value, "EventType must be derived from Event");
        // Use the event type's runtime type information as a key.
        auto& handlers = subscribers[std::type_index(typeid(EventTemplate))];
        // Wrap the handler so that it accepts a base Event reference.
        // When an event is published, this wrapper casts it back to EventType.
        handlers.push_back([handler](const Event& event) {
            handler(static_cast<const EventTemplate&>(event));
        });
    };

    template<typename EventTemplate>
    void publish(const EventTemplate& event) const {
        static_assert(std::is_base_of<Event, EventTemplate>::value, "EventType must be derived from Event");

        auto it = subscribers.find(std::type_index(typeid(EventTemplate)));
        if (it != subscribers.end()) {
            // Call each subscribed handler with the event.
            for (const auto& handler : it->second) {
                handler(event);
            }
        }
    }

    EventBus() = default;
protected:
    // Maps each event type to a list of handler functions.
    std::unordered_map<std::type_index, std::vector<std::function<void(const Event&)>>> subscribers;
};
    
class EntityManager {
public:

    void setSize(Entity entity, const Size& size);
    const Size* getSize(Entity entity);

    void setBehaviour(Entity entity, EntityBehaviour* behaviour);
    const EntityBehaviour* getBehaviour(Entity entity);

    void setType(Entity entity, EntityType type);
    const EntityType* getType(Entity entity);

    Position* getPosition(Entity entity);
    Velocity* getVelocity(Entity entity);

    void setPosition(Entity entity, const Position& position);
    void setVelocity(Entity entity, const Velocity& velocity);

    Entity createEntity();

    virtual void destroyEntity(Entity entity);
    
    bool entityExists(Entity entity) const;

    const std::unordered_set<Entity>& getEntitiesByType(EntityType type) const;

    const std::unordered_set<Entity>& getAllEntities() const;

    void setPlayer(Entity entity);

    Entity getPlayer();

    EntityManager() = default;

protected:

    Entity player;
    std::unordered_map<Entity, Position> positionComponents;
    std::unordered_map<Entity, Velocity> velocityComponents;
    std::unordered_map<Entity, Size> sizeComponents;
    std::unordered_map<Entity, EntityBehaviour*> behaviours;
    std::unordered_map<Entity, EntityType> typeComponents;
    std::unordered_set<Entity> activeEntities;
    std::unordered_map<EntityType, std::unordered_set<Entity>> activeEntitiesByType;
    Entity nextEntityID = 0;
};

class SystemManager {
public:
    virtual void update(float dt);
    void initialize(EntityManager* em, EventBus* eb, MovementSystem* ms, CollisionSystem* cs, StatusSystem* ss);
    SystemManager() = default; 
    
protected:
    MovementSystem* movementSystem;
    StatusSystem* statusSystem;
    CollisionSystem* collisionSystem;

};

class EntityTypeRegistry {
public:
    static EntityType registerType(const std::string& typeName) {
        static EntityType nextTypeID = 0; 
        return typeMap[typeName] = nextTypeID++;
    }

    static const EntityType* getType(const std::string& typeName) {
        auto it = typeMap.find(typeName);
        return (it != typeMap.end()) ? &it->second : nullptr;
    }

private:
    static inline std::unordered_map<std::string, EntityType> typeMap;
};
        
