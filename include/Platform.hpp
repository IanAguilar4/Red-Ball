#ifndef PLATFORM_HPP
#define PLATFORM_HPP

#include <SFML/Graphics.hpp>

// Estructura para representar una plataforma
struct Platform {
    sf::RectangleShape shape;
    sf::FloatRect bounds;

    Platform(float x, float y, float w, float h);
};

// Estructura para representar un pico/obstáculo sobre plataformas
struct Spike {
    sf::ConvexShape shape;
    bool moving;
    float minX, maxX;
    float speed;    // px/seg
    int direction;  // 1 derecha, -1 izquierda
};

// Helper para crear picos
Spike CreateSpike(float x, float y, float width, float height,
                  bool moving = false, float minX = 0.f,
                  float maxX = 0.f, float speed = 0.f);

#endif
