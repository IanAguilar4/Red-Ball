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
Level CreateLevel1();
Level CreateLevel2();
Level CreateLevel3();
Level CreateLevel4();
Level CreateLevel5();
Level CreateLevel6();
Level CreateLevel7();
Level CreateLevel8();
Level CreateLevel9();
Level CreateLevel10();

#endif
