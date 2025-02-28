#pragma once
#include "Entity.h"
#include "Components.h"
#include <typeindex>
#include <functional>
#include <unordered_map>
#include <vector>
#include <memory>
    
class Event {
public:
    virtual ~Event() = default;
};

class StatusEvent : public Event {
public:
    virtual ~StatusEvent() = default;
};

class StatusAppliedEvent : public StatusEvent {
public:
    Entity target;
    float interval;
    std::function<bool()> condition;
    float timeSinceLastTick = 0.0f;

    StatusAppliedEvent(Entity target, float interval, std::function<bool()> condition) 
        : target(target), interval(interval), condition(condition) {}

    virtual ~StatusAppliedEvent() = default;
};

class StatusRemovedEvent : public StatusEvent {
public:
    Entity target;
    StatusAppliedEvent status;
};