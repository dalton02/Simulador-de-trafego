#include "../../include/ui/graphics.hpp"
#include "../../include/core/car.hpp"
#include "../../include/core/grid.hpp"
#include "../../include/core/traffic_light.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Angle.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/WindowEnums.hpp>

sf::RenderWindow window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}),
                        "Traffic Master Car");
sf::Texture textureCar;
sf::Texture textureCar2;
sf::Texture textureCar3;
sf::Texture textureRed;
sf::Texture textureAmbulance;
sf::Texture textureGreen;

void load() {
  bool ignore = textureCar.loadFromFile("assets/car.png");
  ignore = textureCar2.loadFromFile("assets/car2.png");
  ignore = textureCar3.loadFromFile("assets/car3.png");
  ignore = textureGreen.loadFromFile("assets/green.png");
  ignore = textureRed.loadFromFile("assets/red.png");
  ignore = textureAmbulance.loadFromFile("assets/ambulance.png");
}

bool showCollision = false;

void drawRoads() {
  for (Object obj : roadManager.getRoads()) {

    float pixelX = obj.x * 1.f;
    float pixelY = obj.y * 1.f;
    float width = obj.width * 1.f;
    float height = obj.height * 1.f;

    sf::RectangleShape vRoad({width, height});
    vRoad.setPosition({pixelX, pixelY});
    vRoad.setFillColor(sf::Color(50, 50, 50));
    window.draw(vRoad);

    sf::RectangleShape vStripe;
    vStripe.setFillColor(sf::Color(255, 255, 0));

    if (width > height) {
      float stripeWidth = width * 0.04f;
      float stripeHeight = height * 0.2f;
      float stripeY = pixelY + (height - stripeHeight) / 2;

      vStripe.setSize({stripeWidth, stripeHeight});

      for (float x = pixelX + 5; x < pixelX + width; x += width * 0.1f) {
        vStripe.setPosition({x, stripeY});
        window.draw(vStripe);
      }

    } else if (height > width) {
      float stripeWidth = width * 0.2f;
      float stripeHeight = height * 0.04f;

      float stripeX = pixelX + (width - stripeWidth) / 2;
      vStripe.setSize({stripeWidth, stripeHeight});
      for (float y = pixelY + 10; y < pixelY + height; y += height * 0.1f) {
        vStripe.setPosition({stripeX, y});
        window.draw(vStripe);
      }
    }
  }
}

void drawCars() {

  for (const auto &car : globalCars) {

    float pixelX = car->car.x;
    float pixelY = car->car.y;
    float carH = car->car.height;

    float carW = car->car.width;

    if (showCollision) {
      sf::RectangleShape origin({carW, carH});
      origin.setPosition({pixelX, pixelY});
      origin.setFillColor(sf::Color(255, 255, 255, 255));
      window.draw(origin);
    }

    sf::Sprite sprite(textureCar2);

    int speed = car->speedX;
    if (car->speedX == 0) {
      speed = car->speedY;
    }
    if (speed == 2) {
      sprite.setTexture(textureCar3);
    }

    if (speed == 4) {
      sprite.setTexture(textureCar);
    }
    if (car->isAmbulance) {
      sprite.setTexture(textureAmbulance);
    }
    sprite.setPosition({pixelX, pixelY + carH});
    if (car->speedX > 0) {
      sprite.setRotation(sf::degrees(-90.f));
    } else {
      sprite.setPosition({pixelX, pixelY});
    }
    window.draw(sprite);

    if (!showCollision) {
      continue;
    }

    Object collisor = car->collisorLight;
    float width = collisor.width;
    float height = collisor.height;

    float x = collisor.x;
    float y = collisor.y;

    sf::RectangleShape rec({width, height});
    rec.setPosition({x, y});
    rec.setFillColor(sf::Color(255, 0, 0, 100));
    rec.setOutlineColor(sf::Color(255, 0, 0, 240));
    window.draw(rec);

    collisor = car->collisorFront;
    width = collisor.width;
    height = collisor.height;

    x = collisor.x;
    y = collisor.y;

    sf::RectangleShape rec2({width, height});
    rec2.setPosition({x, y});
    rec2.setFillColor(sf::Color(0, 255, 0, 100));
    rec2.setOutlineColor(sf::Color(0, 255, 0, 240));
    window.draw(rec2);
  }
}

void drawLights() {

  for (TrafficLight &light : globalLights) {

    sf::Sprite rect(textureRed);

    float pixelX = light.obj.x * 1.f;
    float pixelY = light.obj.y * 1.f;

    if (light.direction == 0) {
      pixelX -= roadManager.size;
    } else {
      pixelY -= roadManager.size;
    }

    if (light.green) {
      rect.setTexture(textureGreen);
    }

    rect.setPosition({pixelX, pixelY});

    window.draw(rect);

    if (!showCollision) {
      continue;
    }
    sf::RectangleShape rec2({light.obj.width * 1.0f, light.obj.height * 1.0f});

    pixelX = light.obj.x * 1.f;
    pixelY = light.obj.y * 1.f;
    rec2.setPosition({pixelX, pixelY});
    rec2.setFillColor(sf::Color(0, 130, 255, 100));
    window.draw(rec2);
  }
}

void draw() {

  while (const std::optional event = window.pollEvent()) {
    if (event->is<sf::Event::Closed>()) {
      window.close();
    }
  }

  window.clear(sf::Color(196, 196, 196));

  drawRoads();
  drawCars();
  drawLights();

  window.display();
}
