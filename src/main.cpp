#include "headers/Managers.h"
#include "headers/DeltaTime.h"
#include "headers/Behaviours.h"
#include "../libraries/graphicslib/include/Shader.h"
#include "../libraries/graphicslib/include/Texture.h"
#include "../libraries/graphicslib/include/Renderer.h"
#include "../libraries/graphicslib/include/Window.h"
#include <functional>
#include <unordered_map>

struct Health {
    float current;
    float max;
};

class MyEntityManager : public EntityManager {
public:
    const Health* getHealth(Entity entity) {
        if (healthComponents.count(entity) > 0) {
            return &healthComponents.at(entity);
        }
        return nullptr;
    }

    const float* getCurrentHealth(Entity entity) {
        if (healthComponents.count(entity) > 0) {
            return &healthComponents.at(entity).current;
        }
        return nullptr;
    }

    void setHealth(Entity entity, const Health& health) {
        healthComponents[entity] = health;
    }

    void setCurrentHealth(Entity entity, float health) {
        healthComponents[entity].current = health;
    }

private:
    std::unordered_map <Entity, Health> healthComponents;
};

class DamageEvent : public Event {
public:
    Entity target;
    float damage;
    DamageEvent(Entity target, float damage) : target(target), damage(damage) {}
};

class DamageStatus : public StatusAppliedEvent {
public:
    Entity source;
    DamageStatus(Entity source, Entity target, float interval, std::function<bool()> condition) : StatusAppliedEvent(target, interval, condition), source(source) {}
};
    
class MyCollisionSystem : public CollisionSystem {
public:

    void initialize(EntityManager* em, EventBus* eb) override {
        CollisionSystem::initialize(em, eb);
        myEntityManager = dynamic_cast<MyEntityManager*>(this->entityManager);
    }
    void update(float dt) override {
        CollisionSystem::update(dt);

        if (newCollisions.count(myEntityManager->getPlayer()) > 0) {
            for (Entity collider : newCollisions.at(myEntityManager->getPlayer())) {
                DamageStatus* event = new DamageStatus(collider, myEntityManager->getPlayer(), 1.0f, [this, collider]() { return isColliding(myEntityManager->getPlayer(), collider); });
                eventBus->publish<DamageStatus>(*event);
                Entity source;
            }
        }
    }
protected:
    MyEntityManager* myEntityManager = nullptr;
};

class PlayerBehaviour : public EntityBehaviour {
public:
    PlayerBehaviour(EntityManager* em, InputManager* im) 
        : EntityBehaviour(em)  
    {
        initialize(im);
    };

    void initialize(InputManager* im) {
        inputManager = im;
        myEntityManager = dynamic_cast<MyEntityManager*>(this->entityManager);
    };

    void update(Entity entity, float dt) override {
        Velocity* entityVelocity = myEntityManager->getVelocity(entity);
        if (!entityVelocity) return;

        entityVelocity->vx = 0.0f;
        entityVelocity->vy = 0.0f;
    
        if (inputManager->isKeyDown(GLFW_KEY_W)) entityVelocity->vy = -100.0f;
        if (inputManager->isKeyDown(GLFW_KEY_S)) entityVelocity->vy = 100.0f;
    
        if (inputManager->isKeyDown(GLFW_KEY_A)) entityVelocity->vx = -100.0f;
        if (inputManager->isKeyDown(GLFW_KEY_D)) entityVelocity->vx = 100.0f;
    }

private:
    InputManager* inputManager;
    MyEntityManager* myEntityManager;
};


class DamageSystem : public System {
public:
    void initialize(EntityManager* em, EventBus* eb) override {
        System::initialize(em, eb);
        myEntityManager = dynamic_cast<MyEntityManager*>(this->entityManager);
 
        eventBus->subscribe<DamageEvent>([this](const DamageEvent& damageEvent) { applyDamage(damageEvent); });
    }
    DamageSystem() = default;
    void update(float dt) override {
        return;
    }

private:

    void applyDamage(const DamageEvent& damageEvent) {
        const float* entityHealth = myEntityManager->getCurrentHealth(damageEvent.target);

        if (!entityHealth) return;
        myEntityManager->setCurrentHealth(damageEvent.target, *entityHealth - damageEvent.damage);
        std::cout << "Health: " << myEntityManager->getHealth(damageEvent.target)->current << std::endl;
    }


    MyEntityManager* myEntityManager;
};

class MyStatusSystem : public StatusSystem {
public:
    void initialize(EntityManager* em, EventBus* eb) override {
        StatusSystem::initialize(em, eb);
        myEntityManager = dynamic_cast<MyEntityManager*>(this->entityManager);

        std::cout << "Subscribing to DamageStatus..." << std::endl;  // ✅ Debugging line
        
        eventBus->subscribe<DamageStatus>([this](const DamageStatus& statusEvent) {
            DamageStatus* mutableStatus = new DamageStatus(statusEvent);
            this->addStatus(mutableStatus);
        });
        
    }

protected:
    void handleStatusUpdate(StatusAppliedEvent* status) override {
        if (DamageStatus* damageStatus = dynamic_cast<DamageStatus*>(status)) {
            //if (myEntityManager->getType(damageStatus->source) == EntityTypeRegistry::getType("enemy")) 
            DamageEvent damageEvent(damageStatus->target, 10.0f);
            
            eventBus->publish<DamageEvent>(damageEvent);
            status->timeSinceLastTick = 0.0f;
        }
    }    
    MyEntityManager* myEntityManager;
};

class MySystemManager : SystemManager {
public:
    void update(float dt) override {
        SystemManager::update(dt);
        damageSystem->update(dt);
    }
    void myInitialize(MyEntityManager* em, EventBus* eb, MovementSystem* ms, CollisionSystem* cs, StatusSystem* ss, DamageSystem* ds) {
        initialize(em, eb, ms, cs, ss);
        damageSystem = ds;
    }
protected:
    DamageSystem* damageSystem;
};


int main() {
    Window window;

    window.open(800, 600, "Window");

    window.initializeOpenGL();
    window.setBackgroundColor(0.0f,0.5f,1.0f);

    Renderer renderManager(&window);
    renderManager.initialize();
    
    MyEntityManager myEntityManager;
    EventBus eventBus;
    MovementSystem movementSystem;
    MyCollisionSystem collisionSystem;
    DamageSystem damageSystem;
    InputManager inputManager;
    MyStatusSystem statusSystem;
    MySystemManager systemManager;


    PlayerBehaviour playerBehaviour(&myEntityManager, &inputManager);


    Entity player = myEntityManager.createEntity();
    myEntityManager.setPlayer(player);
    myEntityManager.setBehaviour(player, &playerBehaviour);
    myEntityManager.setPosition(player, Position(0.0f, 0.0f, 0.0f));
    myEntityManager.setVelocity(player, Velocity(0.0f, 0.0f));
    myEntityManager.setSize(player, Size(50.0f, 50.0f));
    myEntityManager.setHealth(player, Health(100.0f, 100.0f));

    Entity player1 = myEntityManager.createEntity();
    myEntityManager.setPosition(player1, Position(100.0f, 0.0f, 0.0f));
    myEntityManager.setSize(player1, Size(50.0f, 50.0f));
    myEntityManager.setVelocity(player1, Velocity(0.0f, 0.0f));

    movementSystem.initialize(&myEntityManager, &eventBus);
    inputManager.initialize(&window);
    collisionSystem.initialize(&myEntityManager, &eventBus);
    damageSystem.initialize(&myEntityManager, &eventBus);
    statusSystem.initialize(&myEntityManager, &eventBus);

    systemManager.myInitialize(&myEntityManager, &eventBus, &movementSystem, &collisionSystem, &statusSystem, &damageSystem);

    Shader shader("../assets/shaders/vertex.glsl", "../assets/shaders/fragment.glsl");
    Texture texture("../assets/textures/person.jpg");

    shader.use();
    texture.bind(0);

    while (!window.shouldClose()) {
        window.clearScreen();
    
        renderManager.beginDraw();
        
        Position* playerPosition = myEntityManager.getPosition(player);
        const Size* playerSize = myEntityManager.getSize(player);

        Position* player1Position = myEntityManager.getPosition(player1);
        const Size* player1Size = myEntityManager.getSize(player1);

        renderManager.drawRectangle({playerPosition->x, playerPosition->y, playerPosition->z}, {playerSize->w, playerSize->h, 0.0f}, {255.0f, 255.0f, 255.0f}, 0);
        renderManager.drawRectangle({player1Position->x, player1Position->y, player1Position->z}, {player1Size->w, player1Size->h, 0.0f}, {255.0f, 255.0f, 255.0f}, 0);

        renderManager.endDraw();

        window.swapBuffers();

        dt.update();
        inputManager.update();
        movementSystem.update(dt.getDeltaTime());
        playerBehaviour.update(player, dt.getDeltaTime());
        collisionSystem.update(dt.getDeltaTime());
        statusSystem.update(dt.getDeltaTime());
    }

    
    
    return 0;
}


