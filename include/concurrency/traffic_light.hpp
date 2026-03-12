#pragma once
#include "../utils/utils.hpp"
#include "../concurrency/synchronization.hpp" // Precisa conhecer o Relógio Global!
#include <thread>
#include <mutex>

class TrafficLight {
private:
    int ticksForToggle;
    int tickCounter;
    bool isRunning;

public:
    TrafficLightData* data; // Ponteiro para a struct global que a interface lê
    std::thread thr;

    // Construtor atualizado
    TrafficLight(TrafficLightData* data, int ticksForToggle);
    ~TrafficLight();

    // Impede cópia na memória (segurança para threads)
    TrafficLight(const TrafficLight&) = delete;
    TrafficLight& operator=(const TrafficLight&) = delete;

    // O loop que roda na thread
    void threadLoop();
    
    // Função para inverter a cor
    void toggle();
};