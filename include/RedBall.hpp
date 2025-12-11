#ifndef REDBALL_HPP
#define REDBALL_HPP

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

class RedBall {
public:
    RedBall(b2World* world, float radius, float x, float y);
    ~RedBall();
    
    void applyForce(float x, float y);
    void applyImpulse(float x, float y);
    sf::Vector2f getPosition() const;
    sf::CircleShape getShape() const;
    b2Body* getBody();
    
private:
    b2Body* body;
    b2Shape* shape;
    float radius;
};

#endif
