#pragma once
#include <chrono>

class DeltaTime {
public:
    static DeltaTime& getInstance(); //Singleton access
    float getDeltaTime();
    
private:
    DeltaTime() = default;
    DeltaTime(const DeltaTime&) = delete;
    DeltaTime& operator=(const DeltaTime&) = delete;
};
