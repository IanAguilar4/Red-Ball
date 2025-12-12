#ifndef PLAYERSCORE_HPP
#define PLAYERSCORE_HPP

#include <string>
#include <vector>

// Estructura para los puntajes
struct PlayerScore {
    std::string name;
    float time; // tiempo total en segundos
};

// Funciones para manejo de scores
void LoadScores(const std::string& filename, std::vector<PlayerScore>& scores);
void SaveScores(const std::string& filename, const std::vector<PlayerScore>& scores);

#endif
