#include "../../include/vehicles/car.hpp"
#include <iostream>

Car::Car(CarData* data) {
    this->data = data;
    this->tickCounter = 0;
    this->isRunning = true;
    this->isActive = false;
    this->data->obj->x = -100;
    this->data->obj->y = -100;
    this->data->spriteId = 0;
    this->data->dir = 0;
}

Car::~Car() {
    isRunning = false;
    if(thr.joinable()) thr.join();
}

void Car::spawn(int startR, int startC, Direction dir, CarSpeed spd, int spriteId) {
    this->gridR = startR;
    this->gridC = startC;
    this->currentDir = dir;
    this->speed = spd;
    this->tickCounter = 0;
    this->isActive = true;
    
    // Passa os dados para a interface ler
    this->data->spriteId = spriteId;
    this->data->dir = (dir == Direction::RIGHT) ? 0 : 1; 
}

void Car::threadLoop(Grid& grid, std::condition_variable& globalClockCV, std::mutex& clockMutex, int& currentTick) {
    int lastSeenTick = currentTick;

    while (isRunning) {
        std::unique_lock<std::mutex> lock(clockMutex);
        globalClockCV.wait(lock, [&]() { 
            return currentTick != lastSeenTick || !isRunning; 
        });

        if (!isRunning) break;
        lastSeenTick = currentTick;
        lock.unlock();

        // Só tenta andar se estiver ativo na tela
        if (isActive) {
            tickCounter++;
            if (tickCounter >= static_cast<int>(speed)) {
                tickCounter = 0;
                tryMove(grid);
            }
        }
    }
}

void Car::tryMove(Grid& grid) {
    int nextR = gridR;
    int nextC = gridC;

    if (currentDir == Direction::DOWN) nextR++;
    else if (currentDir == Direction::RIGHT) nextC++;

    // 1. Checa se o carro saiu da tela (32 linhas, 48 colunas)
    if (nextR >= 32 || nextC >= 48) {
        grid.unlockCell(gridR, gridC); // Libera o espaço
        isActive = false;              // Fica disponível para o Spawner
        
        std::lock_guard<std::mutex> interfaceLock(carsMutex);
        data->obj->x = -100;           // Esconde o carro
        data->obj->y = -100;
        return;
    }

   // 2. LÓGICA DO SEMÁFORO: Checa se a próxima célula tem um semáforo vermelho
    // 2. LÓGICA DO SEMÁFORO: Checa se a próxima célula tem um semáforo vermelho
    {
        std::lock_guard<std::mutex> lock(lightsMutex);
        for (const auto& light : globalLights) {
            
            // Carro na Horizontal SÓ olha para semáforos da Horizontal (dir == 0)
            if (currentDir == Direction::RIGHT && light.dir == 0) {
                if (light.gridC == nextC && (light.gridR == nextR || light.gridR == nextR - 1)) {
                    if (!light.green) return; 
                }
            } 
            // Carro na Vertical SÓ olha para semáforos da Vertical (dir == 1)
            else if (currentDir == Direction::DOWN && light.dir == 1) {
                if (light.gridR == nextR && (light.gridC == nextC || light.gridC == nextC - 1)) {
                    if (!light.green) return; 
                }
            }
            
        }
    }
    // 3. Tenta andar e atualizar a tela
    if (grid.tryLockCell(nextR, nextC)) {
        grid.unlockCell(gridR, gridC);
        gridR = nextR;
        gridC = nextC;

        std::lock_guard<std::mutex> interfaceLock(carsMutex);
        data->obj->x = grid.getPixelX(gridC);
        data->obj->y = grid.getPixelY(gridR);
    }
}