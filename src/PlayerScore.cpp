#include "PlayerScore.hpp"
#include <fstream>
#include <algorithm>

void loadScores(const std::string& filename, std::vector<PlayerScore>& scores) {
    std::ifstream in(filename);
    if (!in) return;

    PlayerScore ps;
    while (in >> ps.name >> ps.time) {
        scores.push_back(ps);
    }

    std::sort(scores.begin(), scores.end(),
              [](const PlayerScore& a, const PlayerScore& b) {
                  return a.time < b.time;
              });
}

void saveScores(const std::string& filename, const std::vector<PlayerScore>& scores) {
    std::ofstream out(filename);
    if (!out) return;

    for (const auto& ps : scores) {
        out << ps.name << " " << ps.time << "\n";
    }
}
