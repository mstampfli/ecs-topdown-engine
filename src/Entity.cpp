#include "headers/Entity.h"

Entity EntityManager::createEntity() {
    return nextEntityID++;
}
