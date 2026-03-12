#pragma once
#include <vector>
#include <mutex>

struct Object {
    float x;
    float y;
};

struct TrafficLightData {
    Object* obj;
    bool green;
    int gridR; // Linha do cruzamento
    int gridC; // Coluna do cruzamento
    int dir;
};

struct CarData {
    Object* obj;
    float speedX;
    float speedY;
    int spriteId; // De 0 a 5
    int dir;      // 0 = Direita, 1 = Baixo
};

extern std::vector<TrafficLightData> globalLights;
extern std::vector<CarData> globalCars;
extern std::mutex carsMutex;
extern std::mutex lightsMutex;