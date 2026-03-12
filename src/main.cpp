#include <iostream>
#include <vector>
#include <memory>
#include <chrono>
#include <random>

#include "../include/utils/utils.hpp"
#include "../include/core/Grid.hpp"
#include "../include/concurrency/synchronization.hpp"
#include "../include/concurrency/traffic_light.hpp"
#include "../include/vehicles/car.hpp"
#include "../include/interface/interface.hpp"

std::vector<TrafficLightData> globalLights;
std::vector<CarData> globalCars;
std::mutex carsMutex;
std::mutex lightsMutex;

int main() {
    std::cout << "Iniciando Simulador de Trafego...\n";

    Grid grid(1200, 800, 25);

    // =====================================
    // 1. CRIANDO OS 16 SEMÁFOROS (8 Cruzamentos x 2 Vias)
    // =====================================
    globalLights.resize(16);
    std::vector<std::unique_ptr<TrafficLight>> trafficThreads;
    
    int vCols[] = {8, 18, 28, 38}; // Colunas das ruas verticais
    int hRows[] = {10, 22};        // Linhas das ruas horizontais
    
    int lightIndex = 0;
    for (int r : hRows) {
        for (int c : vCols) {
            // Semáforo para o carro da Horizontal (Verde por padrão)
            globalLights[lightIndex].gridR = r;
            globalLights[lightIndex].gridC = c;
            globalLights[lightIndex].dir = 0; // <-- ADICIONE ISTO (0 = Horizontal)
            globalLights[lightIndex].obj = new Object{grid.getPixelX(c) - 15, grid.getPixelY(r) - 15};
            globalLights[lightIndex].green = true; 
            trafficThreads.push_back(std::make_unique<TrafficLight>(&globalLights[lightIndex], 40)); 
            lightIndex++;

            // Semáforo para o carro da Vertical (Vermelho por padrão)
            globalLights[lightIndex].gridR = r - 1; 
            globalLights[lightIndex].gridC = c;
            globalLights[lightIndex].dir = 1; // <-- ADICIONE ISTO (1 = Vertical)
            globalLights[lightIndex].obj = new Object{grid.getPixelX(c) + 15, grid.getPixelY(r) - 25};
            globalLights[lightIndex].green = false; 
            trafficThreads.push_back(std::make_unique<TrafficLight>(&globalLights[lightIndex], 40)); 
            lightIndex++;
        }
    }

    // =====================================
    // 2. CRIANDO OS 20 CARROS (Invisíveis no começo)
    // =====================================
    int TOTAL_CARS = 20;
    globalCars.resize(TOTAL_CARS);
    std::vector<std::unique_ptr<Car>> carThreads;

    for(int i = 0; i < TOTAL_CARS; i++) {
        globalCars[i].obj = new Object{-100, -100};
        carThreads.push_back(std::make_unique<Car>(&globalCars[i]));
    }

    // Liga as threads dos Semáforos e Carros (ficam dormindo)
    for(auto& t : trafficThreads) {
        t->thr = std::thread(&TrafficLight::threadLoop, t.get());
    }
    for(auto& c : carThreads) {
        c->thr = std::thread(&Car::threadLoop, c.get(), std::ref(grid), 
                             std::ref(GlobalClock::cv), std::ref(GlobalClock::mtx), 
                             std::ref(GlobalClock::currentTick));
    }

    // =====================================
    // 3. THREAD DO SPAWNER (Com 2 Faixas e 6 Sprites)
    // =====================================
    std::thread spawnerThread([&]() {
        std::mt19937 rng(time(nullptr));
        std::uniform_int_distribution<int> distRoad(0, 1);  // 0: Vertical, 1: Horizontal
        std::uniform_int_distribution<int> distV(0, 3);     // 4 ruas verticais
        std::uniform_int_distribution<int> distH(0, 1);     // 2 ruas horizontais
        
        // DECLARAÇÕES QUE FALTARAM:
        std::uniform_int_distribution<int> distLane(0, 1);  // Sorteia a faixa: 0 ou 1
        std::uniform_int_distribution<int> distSpd(1, 3);   // Sorteia a velocidade
        std::uniform_int_distribution<int> distSprite(0, 5);// Sorteia o sprite: de 0 a 5

        while (GlobalClock::isRunning) {
            std::this_thread::sleep_for(std::chrono::milliseconds(800)); // Tenta spawnar a cada 800ms

            for (auto& c : carThreads) {
                if (!c->isActive) { 
                    int type = distRoad(rng);
                    
                    // AQUI AS VARIÁVEIS SÃO CRIADAS ANTES DO SPAWN
                    int lane = distLane(rng); 
                    int spriteId = distSprite(rng);
                    CarSpeed spd = (distSpd(rng) == 1) ? CarSpeed::FAST : CarSpeed::MEDIUM;
                    
                    if (type == 0) {
                        // Spawna na Vertical
                        c->spawn(0, vCols[distV(rng)] + lane, Direction::DOWN, spd, spriteId);
                    } else {
                        // Spawna na Horizontal
                        c->spawn(hRows[distH(rng)] + lane, 0, Direction::RIGHT, spd, spriteId);
                    }
                    break;
                }
            }
        }
    });

    // 4. Inicia o Relógio (Agora mais lento: 1 Tick = 250ms)
    GlobalClock::start(250);

    // 5. Interface Gráfica
    Renderer renderer;
    while (true) {
        renderer.render();
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    GlobalClock::stop();
    if (spawnerThread.joinable()) spawnerThread.join();
    return 0;
}