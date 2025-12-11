#ifndef PLATFORM_HPP
#define PLATFORM_HPP

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

class Platform {
public:
    Platform(b2World* world, float x, float y, float width, float height);
    ~Platform();
    
    sf::RectangleShape getShape() const;
    sf::Vector2f getPosition() const;
    sf::Vector2f getSize() const;
    
private:
    b2Body* body;
    b2Shape* shape;
    float width;
    float height;
};

#endif
