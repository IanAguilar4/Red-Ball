#ifndef GAMEWORLD_HPP
#define GAMEWORLD_HPP

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

class GameWorld {
public:
    GameWorld(float gravityX = 0.0f, float gravityY = 10.0f);
    ~GameWorld();
    
    void step(float timeStep, int velocityIterations, int positionIterations);
    b2World* getWorld();
    
private:
    b2World* world;
};

#endif
