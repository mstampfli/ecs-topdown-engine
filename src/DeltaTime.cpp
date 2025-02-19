#include "headers/DeltaTime.h"
#include <chrono>

DeltaTime& DeltaTime::getInstance() {
    static DeltaTime instance;
    return instance;
}

float DeltaTime::getDeltaTime() {
    static std::chrono::high_resolution_clock::time_point lastFrame = std::chrono::high_resolution_clock::now();
    
    auto now = std::chrono::high_resolution_clock::now();
    float deltaTime = std::chrono::duration<float>(now - lastFrame).count();
    lastFrame = now;  

    return deltaTime;  
}


