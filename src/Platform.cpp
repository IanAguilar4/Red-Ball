#include "Platform.hpp"

// Constructor de Platform
Platform::Platform(float x, float y, float w, float h) {
    shape.setSize(sf::Vector2f(w, h));
    shape.setPosition(x, y);
    shape.setFillColor(sf::Color(139, 69, 19)); // café
    bounds = shape.getGlobalBounds();
}

// Helper para crear picos
Spike createSpike(float x, float y, float width, float height,
                  bool moving, float minX, float maxX, float speed) {
    Spike s;
    s.shape.setPointCount(3);
    // Triángulo isósceles apuntando hacia arriba
    s.shape.setPoint(0, sf::Vector2f(0.f, height));
    s.shape.setPoint(1, sf::Vector2f(width / 2.f, 0.f));
    s.shape.setPoint(2, sf::Vector2f(width, height));
    s.shape.setPosition(x, y);
    s.shape.setFillColor(sf::Color::Black);
    s.moving = moving;
    s.minX = minX;
    s.maxX = maxX;
    s.speed = speed;
    s.direction = 1;
    return s;
}
