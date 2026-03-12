#include "../../include/concurrency/traffic_light.hpp"
#include <iostream>

TrafficLight::TrafficLight(TrafficLightData* data, int ticksForToggle) {
    this->data = data;
    this->ticksForToggle = ticksForToggle;
    this->tickCounter = 0; // Inicializando corretamente!
    this->isRunning = true;
    
}

TrafficLight::~TrafficLight() {
    isRunning = false;
    if(thr.joinable()) {
        thr.join();
    }
}

void TrafficLight::threadLoop() {
    // Grava o último tick visto
    int lastSeenTick = GlobalClock::currentTick;

    while (isRunning) {
        // 1. Dorme até o Relógio Global avisar que o tempo passou
        std::unique_lock<std::mutex> lock(GlobalClock::mtx);
        GlobalClock::cv.wait(lock, [&]() { 
            return GlobalClock::currentTick != lastSeenTick || !isRunning; 
        });

        if (!isRunning) break;
        lastSeenTick = GlobalClock::currentTick;
        lock.unlock(); // Solta o relógio para os outros semáforos e carros lerem

        // 2. Conta o tick e verifica se deve alterar a cor
        tickCounter++;
        if (tickCounter >= ticksForToggle) {
            tickCounter = 0; // Reseta o contador
            toggle();        // Muda a cor
        }
    }
}

void TrafficLight::toggle() {
    // Trava o mutex dos semáforos (que criamos no utils.hpp)
    // Isso impede que a Interface tente desenhar o semáforo no exato 
    // milissegundo em que estamos mudando a cor dele.
    std::lock_guard<std::mutex> lock(lightsMutex);
    
    data->green = !data->green;
    
    // Opcional: imprimir no terminal para você acompanhar rodando
    // std::cout << "Semaforo alternado! Verde: " << (data->green ? "SIM" : "NAO") << "\n";
}