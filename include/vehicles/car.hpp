#pragma once
#include <thread>
#include <mutex>
#include <condition_variable>

#include "../utils/utils.hpp"
#include "../core/Grid.hpp"
#include "../concurrency/traffic_light.hpp"

enum class CarSpeed { 
    FAST = 1,   
    MEDIUM = 2, 
    SLOW = 4    
};

class Car {
private:
    CarSpeed speed;
    int tickCounter;
    bool isRunning;
    int gridR;
    int gridC;
    Direction currentDir;

public:
    CarData* data;
    std::thread thr;
    bool isActive;

    Car(CarData* data);
    ~Car();

    Car(const Car&) = delete;
    Car& operator=(const Car&) = delete;

    // Atualizado com spriteId
    void spawn(int startR, int startC, Direction dir, CarSpeed spd, int spriteId);

    void threadLoop(Grid& grid, std::condition_variable& globalClockCV, std::mutex& clockMutex, int& currentTick);
    void tryMove(Grid& grid);
};