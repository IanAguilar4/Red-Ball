#ifndef REDBALL_HPP
#define REDBALL_HPP

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

class RedBall {
public:
    RedBall(b2World* world, float radius, float x, float y);
    ~RedBall();
    
    void ApplyForce(float x, float y);
    void ApplyImpulse(float x, float y);
    sf::Vector2f GetPosition() const;
    sf::CircleShape GetShape() const;
    b2Body* GetBody();
    
private:
    b2Body* body;
    b2Shape* shape;
    float radius;
};

#endif
