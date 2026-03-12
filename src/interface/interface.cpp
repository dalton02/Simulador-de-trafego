#include "../../include/interface/interface.hpp"
#include "../../include/utils/utils.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

static const int WINDOW_W = 1200;
static const int WINDOW_H = 800;
static const int ROAD_W = 50; 

static sf::RenderWindow window(sf::VideoMode({WINDOW_W, WINDOW_H}), "Simulador de Trafego");

static float verticalRoads[4] = {200, 450, 700, 950};
static float horizontalRoads[2] = {250, 550};

// Arrays de Texturas
static sf::Texture carRightTextures[6];
static sf::Texture carDownTextures[6];
static bool loaded = false;

Renderer::Renderer() {
    if(!loaded) {
        // Carrega as 12 imagens usando o padrão de nome (1 a 6)
        for(int i = 0; i < 6; i++) {
            std::string pathDir = "src/assets/car_dir_" + std::to_string(i + 1) + ".png";
            std::string pathBaixo = "src/assets/car_baixo_" + std::to_string(i + 1) + ".png";
            
            if(!carRightTextures[i].loadFromFile(pathDir))
                std::cout << "Erro carregando: " << pathDir << "\n";
                
            if(!carDownTextures[i].loadFromFile(pathBaixo))
                std::cout << "Erro carregando: " << pathBaixo << "\n";
        }
        loaded = true;
    }
}

void Renderer::render() {
    while (const std::optional<sf::Event> event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
            exit(0);
        }
    }

    if (!window.isOpen()) return;
    window.clear(sf::Color(30,30,30));

    // Desenha as vias verticais e horizontais
    for(int i=0; i<4; i++) {
        sf::RectangleShape road({(float)ROAD_W,(float)WINDOW_H});
        road.setPosition({verticalRoads[i],0});
        road.setFillColor(sf::Color(20,20,20));
        window.draw(road);
        for(int y=0; y<WINDOW_H; y+=50) {
            sf::RectangleShape line({4,25});
            line.setPosition({verticalRoads[i] + ROAD_W/2 - 2,(float)y});
            line.setFillColor(sf::Color::Yellow);
            window.draw(line);
        }
    }

    for(int i=0; i<2; i++) {
        sf::RectangleShape road({(float)WINDOW_W,(float)ROAD_W});
        road.setPosition({0,horizontalRoads[i]});
        road.setFillColor(sf::Color(20,20,20));
        window.draw(road);
        for(int x=0; x<WINDOW_W; x+=50) {
            sf::RectangleShape line({25,4});
            line.setPosition({(float)x, horizontalRoads[i] + ROAD_W/2 - 2});
            line.setFillColor(sf::Color::Yellow);
            window.draw(line);
        }
    }

    // Desenha Semáforos
    {
        std::lock_guard<std::mutex> lock(lightsMutex);
        for(auto &light : globalLights) {
            sf::CircleShape shape(6); // Menor para ficar discreto
            shape.setPosition({light.obj->x, light.obj->y});
            shape.setFillColor(light.green ? sf::Color::Green : sf::Color::Red);
            window.draw(shape);
        }
    }

    // Desenha Carros com a escala e imagem corretas
    {
        std::lock_guard<std::mutex> lock(carsMutex);
        for(auto &car : globalCars) {
            if (car.obj->x < 0) continue; // Pula os inativos

            // No SFML 3, passamos a textura direto na construção
            sf::Sprite sprite(car.dir == 0 ? carRightTextures[car.spriteId] : carDownTextures[car.spriteId]);

            // Centraliza o ponto de origem (ancora) no meio da imagem (SFML 3.x)
            sf::FloatRect bounds = sprite.getLocalBounds();
            sprite.setOrigin(sf::Vector2f(bounds.size.x / 2.0f, bounds.size.y / 2.0f));

            // Força a imagem a caber em 20x20 pixels (deixando 5px de folga na célula)
            sprite.setScale(sf::Vector2f(20.0f / bounds.size.x, 20.0f / bounds.size.y));
            sprite.setPosition(sf::Vector2f(car.obj->x, car.obj->y));

            window.draw(sprite);
        }
    }
    window.display();
}