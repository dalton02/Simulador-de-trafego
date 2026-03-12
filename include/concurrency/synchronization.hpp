#pragma once
#include <condition_variable>
#include <mutex>
#include <thread>

// Usamos um namespace para organizar tudo que pertence ao Relógio Global
namespace GlobalClock {
    // Variáveis que as outras threads (carros e semáforos) vão escutar
    extern std::condition_variable cv;
    extern std::mutex mtx;
    extern int currentTick;
    extern bool isRunning;
    
    // A thread exclusiva do relógio
    extern std::thread clockThread;

    // Funções de controle
    void start(int tickDurationMs);
    void stop();
}