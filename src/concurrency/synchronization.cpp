#include "../../include/concurrency/synchronization.hpp"
#include <chrono>

namespace GlobalClock {
    // Inicialização das variáveis globais
    std::condition_variable cv;
    std::mutex mtx;
    int currentTick = 0;
    bool isRunning = false;
    std::thread clockThread;

    void start(int tickDurationMs) {
        isRunning = true;
        
        // Cria a thread do relógio
        clockThread = std::thread([tickDurationMs]() {
            while (isRunning) {
                // 1. O relógio dorme pelo tempo de 1 Tick (ex: 100ms)
                std::this_thread::sleep_for(std::chrono::milliseconds(tickDurationMs));
                
                // 2. Trava o mutex rapidinho só para atualizar o número do tick
                {
                    std::lock_guard<std::mutex> lock(mtx);
                    currentTick++;
                }
                
                // 3. O MAESTRO GRITA: Acorda TODAS as threads que estão esperando!
                cv.notify_all(); 
            }
        });
    }

    void stop() {
        isRunning = false;
        // Dá um último grito para garantir que ninguém fique preso dormindo quando o programa fechar
        cv.notify_all(); 
        
        // Espera a thread do relógio terminar de forma segura
        if (clockThread.joinable()) {
            clockThread.join();
        }
    }
}