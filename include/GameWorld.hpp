#ifndef GAMEWORLD_HPP
#define GAMEWORLD_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include "Platform.hpp"

// Estructura para representar un nivel completo
struct Level {
    Platform ground;                         // suelo principal
    std::vector<Platform> platforms;         // plataformas elevadas
    std::vector<sf::RectangleShape> obstacles; // obstáculos "bloque" (negros)
    std::vector<Spike> spikes;              // picos estáticos o móviles
    sf::CircleShape goal;                    // objetivo (estrella)
    sf::Vector2f spawn;                      // posición inicial del jugador

    Level(float gx, float gy, float gw, float gh);
};

// Funciones para crear niveles
Level createLevel1();
Level createLevel2();
Level createLevel3();
Level createLevel4();
Level createLevel5();
Level createLevel6();
Level createLevel7();
Level createLevel8();
Level createLevel9();
Level createLevel10();

#endif
