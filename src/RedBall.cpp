#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <algorithm>
#include <string>

// --- Estados del juego ---
enum class GameState {
    Title,
    EnterName,
    Playing,
    LevelComplete,
    Finished
};

// Estructura para representar una plataforma
struct Platform {
    sf::RectangleShape shape;
    sf::FloatRect bounds;

    Platform(float x, float y, float w, float h) {
        shape.setSize(sf::Vector2f(w, h));
        shape.setPosition(x, y);
        shape.setFillColor(sf::Color(139, 69, 19)); // café
        bounds = shape.getGlobalBounds();
    }
};

// Estructura para representar un pico/obstáculo sobre plataformas
struct Spike {
    sf::ConvexShape shape;
    bool moving;
    float minX, maxX;
    float speed;    // px/seg
    int direction;  // 1 derecha, -1 izquierda
};

struct Level {
    Platform ground;                         // suelo principal
    std::vector<Platform> platforms;         // plataformas elevadas
    std::vector<sf::RectangleShape> obstacles; // obstáculos "bloque" (negros)
    std::vector<Spike> spikes;              // picos estáticos o móviles
    sf::CircleShape goal;                    // objetivo (estrella)
    sf::Vector2f spawn;                      // posición inicial del jugador

    Level(float gx, float gy, float gw, float gh)
        : ground(gx, gy, gw, gh) {}
};

// Estructura para los puntajes
struct PlayerScore {
    std::string name;
    float time; // tiempo total en segundos
};

// ---- Funciones para manejo de scores ----
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

// Centrar un texto horizontalmente en la ventana
void centerTextX(sf::Text& text, float windowWidth) {
    sf::FloatRect b = text.getLocalBounds();
    text.setPosition((windowWidth - b.width) / 2.f - b.left, text.getPosition().y);
}

// Helper para crear picos
Spike createSpike(float x, float y, float width, float height,
                  bool moving = false, float minX = 0.f,
                  float maxX = 0.f, float speed = 0.f) {
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

// --- Definición de niveles ---

// Nivel 1: muy básico
Level createLevel1() {
    Level level(0.f, 560.f, 800.f, 40.f);
    level.ground.shape.setFillColor(sf::Color(34, 139, 34));
    level.ground.bounds = level.ground.shape.getGlobalBounds();

    level.platforms.emplace_back(80.f, 450.f, 160.f, 20.f);
    level.platforms.emplace_back(280.f, 370.f, 160.f, 20.f);
    level.platforms.emplace_back(480.f, 300.f, 160.f, 20.f);

    // Obstáculo en suelo
    sf::RectangleShape obs1(sf::Vector2f(60.f, 60.f));
    obs1.setPosition(450.f, 500.f);
    obs1.setFillColor(sf::Color::Black);
    level.obstacles.push_back(obs1);

    // Pico estático sobre la segunda plataforma
    level.spikes.push_back(createSpike(320.f, 370.f - 18.f, 40.f, 18.f));

    level.goal = sf::CircleShape(20.f, 5);
    level.goal.setOrigin(20.f, 20.f);
    level.goal.setPosition(700.f, 260.f);
    level.goal.setFillColor(sf::Color::Yellow);
    level.goal.setOutlineColor(sf::Color(255, 215, 0));
    level.goal.setOutlineThickness(3.f);

    level.spawn = sf::Vector2f(80.f, 520.f);

    return level;
}

// Nivel 2: más vertical y un pico móvil suave
Level createLevel2() {
    Level level(0.f, 560.f, 800.f, 40.f);
    level.ground.shape.setFillColor(sf::Color(34, 139, 34));
    level.ground.bounds = level.ground.shape.getGlobalBounds();

    level.platforms.emplace_back(80.f, 480.f, 140.f, 20.f);
    level.platforms.emplace_back(260.f, 420.f, 140.f, 20.f);
    level.platforms.emplace_back(440.f, 360.f, 140.f, 20.f);
    level.platforms.emplace_back(620.f, 300.f, 140.f, 20.f);

    // Obstáculo en suelo
    sf::RectangleShape obs1(sf::Vector2f(50.f, 80.f));
    obs1.setPosition(220.f, 480.f);
    obs1.setFillColor(sf::Color::Black);
    level.obstacles.push_back(obs1);

    // Pico estático sobre la primera plataforma
    level.spikes.push_back(createSpike(110.f, 480.f - 18.f, 40.f, 18.f));

    // Pico móvil sobre la tercera plataforma (se mueve de izquierda a derecha)
    level.spikes.push_back(
        createSpike(460.f, 360.f - 18.f, 40.f, 18.f,
                    true, 450.f, 520.f, 70.f) // minX, maxX, speed
    );

    level.goal = sf::CircleShape(20.f, 5);
    level.goal.setOrigin(20.f, 20.f);
    level.goal.setPosition(720.f, 260.f);
    level.goal.setFillColor(sf::Color::Yellow);
    level.goal.setOutlineColor(sf::Color(255, 215, 0));
    level.goal.setOutlineThickness(3.f);

    level.spawn = sf::Vector2f(70.f, 520.f);

    return level;
}

// Nivel 3: torre sencilla con picos
Level createLevel3() {
    Level level(0.f, 560.f, 800.f, 40.f);
    level.ground.shape.setFillColor(sf::Color(34, 139, 34));
    level.ground.bounds = level.ground.shape.getGlobalBounds();

    level.platforms.emplace_back(100.f, 500.f, 120.f, 20.f);
    level.platforms.emplace_back(260.f, 430.f, 120.f, 20.f);
    level.platforms.emplace_back(420.f, 360.f, 120.f, 20.f);
    level.platforms.emplace_back(580.f, 290.f, 120.f, 20.f);
    level.platforms.emplace_back(580.f, 210.f, 120.f, 20.f);

    // Bloque en suelo
    sf::RectangleShape obs1(sf::Vector2f(60.f, 60.f));
    obs1.setPosition(350.f, 500.f);
    obs1.setFillColor(sf::Color::Black);
    level.obstacles.push_back(obs1);

    // Pico estático sobre la segunda plataforma
    level.spikes.push_back(createSpike(290.f, 430.f - 18.f, 40.f, 18.f));

    // Pico móvil sobre la cuarta plataforma
    level.spikes.push_back(
        createSpike(600.f, 290.f - 20.f, 40.f, 20.f,
                    true, 580.f, 680.f, 80.f)
    );

    level.goal = sf::CircleShape(20.f, 5);
    level.goal.setOrigin(20.f, 20.f);
    level.goal.setPosition(640.f, 170.f);
    level.goal.setFillColor(sf::Color::Yellow);
    level.goal.setOutlineColor(sf::Color(255, 215, 0));
    level.goal.setOutlineThickness(3.f);

    level.spawn = sf::Vector2f(80.f, 520.f);

    return level;
}

// Nivel 4: escalera larga
Level createLevel4() {
    Level level(0.f, 560.f, 800.f, 40.f);
    level.ground.shape.setFillColor(sf::Color(34, 139, 34));
    level.ground.bounds = level.ground.shape.getGlobalBounds();

    level.platforms.emplace_back(60.f, 500.f, 120.f, 20.f);
    level.platforms.emplace_back(220.f, 440.f, 120.f, 20.f);
    level.platforms.emplace_back(380.f, 380.f, 120.f, 20.f);
    level.platforms.emplace_back(540.f, 320.f, 120.f, 20.f);
    level.platforms.emplace_back(660.f, 260.f, 100.f, 20.f);

    // Bloques en suelo
    sf::RectangleShape obs1(sf::Vector2f(50.f, 70.f));
    obs1.setPosition(180.f, 490.f);
    obs1.setFillColor(sf::Color::Black);
    level.obstacles.push_back(obs1);

    sf::RectangleShape obs2(sf::Vector2f(50.f, 70.f));
    obs2.setPosition(420.f, 490.f);
    obs2.setFillColor(sf::Color::Black);
    level.obstacles.push_back(obs2);

    // Picos sobre tercera y cuarta plataformas
    level.spikes.push_back(createSpike(410.f, 380.f - 18.f, 40.f, 18.f));
    level.spikes.push_back(
        createSpike(560.f, 320.f - 18.f, 40.f, 18.f,
                    true, 540.f, 650.f, 90.f)
    );

    level.goal = sf::CircleShape(20.f, 5);
    level.goal.setOrigin(20.f, 20.f);
    level.goal.setPosition(720.f, 220.f);
    level.goal.setFillColor(sf::Color::Yellow);
    level.goal.setOutlineColor(sf::Color(255, 215, 0));
    level.goal.setOutlineThickness(3.f);

    level.spawn = sf::Vector2f(80.f, 520.f);

    return level;
}

// Nivel 5: puentes con huecos
Level createLevel5() {
    Level level(0.f, 560.f, 800.f, 40.f);
    level.ground.shape.setFillColor(sf::Color(34, 139, 34));
    level.ground.bounds = level.ground.shape.getGlobalBounds();

    level.platforms.emplace_back(50.f, 450.f, 200.f, 20.f);
    level.platforms.emplace_back(320.f, 450.f, 160.f, 20.f);
    level.platforms.emplace_back(560.f, 450.f, 180.f, 20.f);

    level.platforms.emplace_back(200.f, 350.f, 120.f, 20.f);
    level.platforms.emplace_back(440.f, 320.f, 120.f, 20.f);

    // Bloque en hueco central
    sf::RectangleShape obs3(sf::Vector2f(60.f, 80.f));
    obs3.setPosition(300.f, 480.f);
    obs3.setFillColor(sf::Color::Black);
    level.obstacles.push_back(obs3);

    // Picos sobre plataformas superiores
    level.spikes.push_back(createSpike(230.f, 350.f - 18.f, 40.f, 18.f));
    level.spikes.push_back(
        createSpike(460.f, 320.f - 18.f, 40.f, 18.f,
                    true, 440.f, 520.f, 100.f)
    );

    level.goal = sf::CircleShape(20.f, 5);
    level.goal.setOrigin(20.f, 20.f);
    level.goal.setPosition(720.f, 410.f);
    level.goal.setFillColor(sf::Color::Yellow);
    level.goal.setOutlineColor(sf::Color(255, 215, 0));
    level.goal.setOutlineThickness(3.f);

    level.spawn = sf::Vector2f(80.f, 420.f);

    return level;
}

// Nivel 6: plataformas pequeñas
Level createLevel6() {
    Level level(0.f, 560.f, 800.f, 40.f);
    level.ground.shape.setFillColor(sf::Color(34, 139, 34));
    level.ground.bounds = level.ground.shape.getGlobalBounds();

    level.platforms.emplace_back(80.f, 500.f, 90.f, 20.f);
    level.platforms.emplace_back(220.f, 430.f, 90.f, 20.f);
    level.platforms.emplace_back(360.f, 360.f, 90.f, 20.f);
    level.platforms.emplace_back(500.f, 290.f, 90.f, 20.f);
    level.platforms.emplace_back(640.f, 230.f, 90.f, 20.f);

    sf::RectangleShape obs1(sf::Vector2f(50.f, 60.f));
    obs1.setPosition(180.f, 500.f);
    obs1.setFillColor(sf::Color::Black);
    level.obstacles.push_back(obs1);

    sf::RectangleShape obs2(sf::Vector2f(50.f, 60.f));
    obs2.setPosition(410.f, 500.f);
    obs2.setFillColor(sf::Color::Black);
    level.obstacles.push_back(obs2);

    // Pico móvil sobre la cuarta plataforma
    level.spikes.push_back(
        createSpike(510.f, 290.f - 18.f, 40.f, 18.f,
                    true, 500.f, 620.f, 110.f)
    );

    // Pico estático sobre la tercera
    level.spikes.push_back(createSpike(380.f, 360.f - 18.f, 40.f, 18.f));

    level.goal = sf::CircleShape(20.f, 5);
    level.goal.setOrigin(20.f, 20.f);
    level.goal.setPosition(700.f, 190.f);
    level.goal.setFillColor(sf::Color::Yellow);
    level.goal.setOutlineColor(sf::Color(255, 215, 0));
    level.goal.setOutlineThickness(3.f);

    level.spawn = sf::Vector2f(90.f, 520.f);

    return level;
}

// Nivel 7: mezcla bajas/altas
Level createLevel7() {
    Level level(0.f, 560.f, 800.f, 40.f);
    level.ground.shape.setFillColor(sf::Color(34, 139, 34));
    level.ground.bounds = level.ground.shape.getGlobalBounds();

    level.platforms.emplace_back(60.f, 500.f, 140.f, 20.f);
    level.platforms.emplace_back(260.f, 500.f, 140.f, 20.f);
    level.platforms.emplace_back(460.f, 500.f, 140.f, 20.f);

    level.platforms.emplace_back(200.f, 380.f, 120.f, 20.f);
    level.platforms.emplace_back(400.f, 340.f, 120.f, 20.f);
    level.platforms.emplace_back(610.f, 300.f, 120.f, 20.f);

    sf::RectangleShape obs1(sf::Vector2f(60.f, 80.f));
    obs1.setPosition(340.f, 480.f);
    obs1.setFillColor(sf::Color::Black);
    level.obstacles.push_back(obs1);

    // Picos sobre plataformas altas
    level.spikes.push_back(createSpike(230.f, 380.f - 18.f, 40.f, 18.f));
    level.spikes.push_back(
        createSpike(430.f, 340.f - 18.f, 40.f, 18.f,
                    true, 400.f, 500.f, 120.f)
    );
    level.spikes.push_back(createSpike(640.f, 300.f - 18.f, 40.f, 18.f));

    level.goal = sf::CircleShape(20.f, 5);
    level.goal.setOrigin(20.f, 20.f);
    level.goal.setPosition(720.f, 260.f);
    level.goal.setFillColor(sf::Color::Yellow);
    level.goal.setOutlineColor(sf::Color(255, 215, 0));
    level.goal.setOutlineThickness(3.f);

    level.spawn = sf::Vector2f(80.f, 520.f);

    return level;
}

// Nivel 8: camino largo con techos peligrosos (el de tu captura)
Level createLevel8() {
    Level level(0.f, 560.f, 800.f, 40.f);
    level.ground.shape.setFillColor(sf::Color(34, 139, 34));
    level.ground.bounds = level.ground.shape.getGlobalBounds();

    level.platforms.emplace_back(150.f, 500.f, 160.f, 20.f);
    level.platforms.emplace_back(320.f, 430.f, 160.f, 20.f);
    level.platforms.emplace_back(490.f, 360.f, 160.f, 20.f);
    level.platforms.emplace_back(660.f, 290.f, 120.f, 20.f);

    // Bloque en extremo derecho (como en tu screenshot)
    sf::RectangleShape obsFloor(sf::Vector2f(60.f, 80.f));
    obsFloor.setPosition(700.f, 480.f);
    obsFloor.setFillColor(sf::Color::Black);
    level.obstacles.push_back(obsFloor);

    // Picos sobre segunda y tercera plataformas
    level.spikes.push_back(createSpike(360.f, 430.f - 18.f, 40.f, 18.f));
    level.spikes.push_back(
        createSpike(520.f, 360.f - 18.f, 40.f, 18.f,
                    true, 510.f, 610.f, 130.f)
    );

    level.goal = sf::CircleShape(20.f, 5);
    level.goal.setOrigin(20.f, 20.f);
    level.goal.setPosition(740.f, 250.f);
    level.goal.setFillColor(sf::Color::Yellow);
    level.goal.setOutlineColor(sf::Color(255, 215, 0));
    level.goal.setOutlineThickness(3.f);

    level.spawn = sf::Vector2f(170.f, 520.f);

    return level;
}

// Nivel 9: muchas rutas
Level createLevel9() {
    Level level(0.f, 560.f, 800.f, 40.f);
    level.ground.shape.setFillColor(sf::Color(34, 139, 34));
    level.ground.bounds = level.ground.shape.getGlobalBounds();

    level.platforms.emplace_back(80.f, 500.f, 120.f, 20.f);
    level.platforms.emplace_back(260.f, 500.f, 120.f, 20.f);
    level.platforms.emplace_back(440.f, 500.f, 120.f, 20.f);

    level.platforms.emplace_back(150.f, 420.f, 100.f, 20.f);
    level.platforms.emplace_back(350.f, 380.f, 100.f, 20.f);
    level.platforms.emplace_back(550.f, 340.f, 100.f, 20.f);

    level.platforms.emplace_back(250.f, 300.f, 120.f, 20.f);
    level.platforms.emplace_back(460.f, 260.f, 120.f, 20.f);

    sf::RectangleShape obs1(sf::Vector2f(50.f, 70.f));
    obs1.setPosition(210.f, 490.f);
    obs1.setFillColor(sf::Color::Black);
    level.obstacles.push_back(obs1);

    sf::RectangleShape obs2(sf::Vector2f(50.f, 70.f));
    obs2.setPosition(390.f, 490.f);
    obs2.setFillColor(sf::Color::Black);
    level.obstacles.push_back(obs2);

    // Picos varios
    level.spikes.push_back(createSpike(180.f, 420.f - 18.f, 40.f, 18.f));
    level.spikes.push_back(
        createSpike(370.f, 380.f - 18.f, 40.f, 18.f,
                    true, 350.f, 430.f, 130.f)
    );
    level.spikes.push_back(createSpike(480.f, 260.f - 18.f, 40.f, 18.f));

    level.goal = sf::CircleShape(20.f, 5);
    level.goal.setOrigin(20.f, 20.f);
    level.goal.setPosition(720.f, 220.f);
    level.goal.setFillColor(sf::Color::Yellow);
    level.goal.setOutlineColor(sf::Color(255, 215, 0));
    level.goal.setOutlineThickness(3.f);

    level.spawn = sf::Vector2f(90.f, 520.f);

    return level;
}

// Nivel 10: final vertical
Level createLevel10() {
    Level level(0.f, 560.f, 800.f, 40.f);
    level.ground.shape.setFillColor(sf::Color(34, 139, 34));
    level.ground.bounds = level.ground.shape.getGlobalBounds();

    level.platforms.emplace_back(80.f, 500.f, 90.f, 20.f);
    level.platforms.emplace_back(200.f, 440.f, 90.f, 20.f);
    level.platforms.emplace_back(320.f, 380.f, 90.f, 20.f);
    level.platforms.emplace_back(440.f, 320.f, 90.f, 20.f);
    level.platforms.emplace_back(560.f, 260.f, 90.f, 20.f);
    level.platforms.emplace_back(680.f, 200.f, 90.f, 20.f);

    sf::RectangleShape obs1(sf::Vector2f(50.f, 70.f));
    obs1.setPosition(160.f, 490.f);
    obs1.setFillColor(sf::Color::Black);
    level.obstacles.push_back(obs1);

    sf::RectangleShape obs2(sf::Vector2f(50.f, 70.f));
    obs2.setPosition(360.f, 490.f);
    obs2.setFillColor(sf::Color::Black);
    level.obstacles.push_back(obs2);

    sf::RectangleShape obs3(sf::Vector2f(50.f, 70.f));
    obs3.setPosition(560.f, 490.f);
    obs3.setFillColor(sf::Color::Black);
    level.obstacles.push_back(obs3);

    // Picos complicados al final
    level.spikes.push_back(createSpike(220.f, 440.f - 18.f, 40.f, 18.f));
    level.spikes.push_back(createSpike(340.f, 380.f - 18.f, 40.f, 18.f));
    level.spikes.push_back(
        createSpike(460.f, 320.f - 18.f, 40.f, 18.f,
                    true, 440.f, 520.f, 150.f)
    );
    level.spikes.push_back(
        createSpike(580.f, 260.f - 18.f, 40.f, 18.f,
                    true, 560.f, 680.f, 170.f)
    );

    level.goal = sf::CircleShape(20.f, 5);
    level.goal.setOrigin(20.f, 20.f);
    level.goal.setPosition(720.f, 160.f);
    level.goal.setFillColor(sf::Color::Yellow);
    level.goal.setOutlineColor(sf::Color(255, 215, 0));
    level.goal.setOutlineThickness(3.f);

    level.spawn = sf::Vector2f(90.f, 520.f);

    return level;
}

// --- Juego Red Ball con niveles y ranking ---

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Blue Balls - Niveles");

    // Cargar fuente
    sf::Font font;
    if (!font.loadFromFile("assets/fonts/arialbd.ttf")) {
        std::cout << "Error: No se pudo cargar la fuente (assets/fonts/arialbd.ttf)" << std::endl;
    }

    // NUEVO: textura para la bola
    sf::Texture ballTexture;
    if (!ballTexture.loadFromFile("assets/sprites/blueface.png")) {
        std::cout << "Error: No se pudo cargar la textura de la bola (assets/sprites/blueface.png)" << std::endl;
    } else {
        ballTexture.setSmooth(true); // se ve menos pixelado
    }

    // Scores
    std::vector<PlayerScore> highScores;
    loadScores("scores.txt", highScores);


    std::string playerName;        // nombre actual
    std::string nameInputBuffer;   // lo que el usuario va escribiendo

    // Definir niveles
    std::vector<Level> levels;
    levels.push_back(createLevel1());
    levels.push_back(createLevel2());
    levels.push_back(createLevel3());
    levels.push_back(createLevel4());
    levels.push_back(createLevel5());
    levels.push_back(createLevel6());
    levels.push_back(createLevel7());
    levels.push_back(createLevel8());
    levels.push_back(createLevel9());
    levels.push_back(createLevel10());

    int currentLevelIndex = 0;

    // Jugador
    sf::CircleShape ball(25.f);
    ball.setOrigin(25.f, 25.f);

    // Color base blanco para que no tinte la textura
    ball.setFillColor(sf::Color::White);
    ball.setOutlineColor(sf::Color::Transparent); // opcional, sin borde

    // Asignar textura de la cara
    ball.setTexture(&ballTexture);

    // Física (igual que tenías)
    sf::Vector2f velocity(0.f, 0.f);
    const float gravity = 0.5f;
    const float acceleration = 1.2f;
    const float maxSpeed = 12.0f;
    const float jumpForce = -15.0f;
    const float friction = 0.85f;
    bool onGround = false;

    // Estado del juego
    GameState state = GameState::Title;

    sf::Clock clock;
    float timeElapsed = 0.0f;    // tiempo del nivel actual
    float totalRunTime = 0.0f;   // tiempo total de la partida (todos los niveles)

    auto resetLevel = [&](int index) {
        currentLevelIndex = index;
        Level &lvl = levels[currentLevelIndex];
        ball.setPosition(lvl.spawn);
        velocity = sf::Vector2f(0.f, 0.f);
        onGround = false;
        timeElapsed = 0.0f;
        clock.restart();
    };

    // Comenzamos en Título
    resetLevel(0);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (state == GameState::Title) {
                if (event.type == sf::Event::KeyPressed &&
                    (event.key.code == sf::Keyboard::Enter ||
                     event.key.code == sf::Keyboard::Space)) {
                    nameInputBuffer.clear();
                    state = GameState::EnterName;
                }
            }
            else if (state == GameState::EnterName) {
                if (event.type == sf::Event::TextEntered) {
                    char c = static_cast<char>(event.text.unicode);
                    if (event.text.unicode == 8) { // Backspace
                        if (!nameInputBuffer.empty())
                            nameInputBuffer.pop_back();
                    } else if (event.text.unicode >= 33 && event.text.unicode <= 126) {
                        if (nameInputBuffer.size() < 12) {
                            nameInputBuffer.push_back(c);
                        }
                    }
                } else if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Enter) {
                        if (!nameInputBuffer.empty()) {
                            playerName = nameInputBuffer;
                            totalRunTime = 0.0f;
                            resetLevel(0);
                            state = GameState::Playing;
                        }
                    } else if (event.key.code == sf::Keyboard::Escape) {
                        state = GameState::Title;
                    }
                }
            }
            else if (state == GameState::LevelComplete) {
                if (event.type == sf::Event::KeyPressed &&
                    (event.key.code == sf::Keyboard::Enter ||
                     event.key.code == sf::Keyboard::Space)) {
                    if (currentLevelIndex + 1 < static_cast<int>(levels.size())) {
                        resetLevel(currentLevelIndex + 1);
                        state = GameState::Playing;
                    } else {
                        PlayerScore ps;
                        ps.name = playerName.empty() ? std::string("Anon") : playerName;
                        ps.time = totalRunTime;

                        highScores.push_back(ps);
                        std::sort(highScores.begin(), highScores.end(),
                                  [](const PlayerScore& a, const PlayerScore& b) {
                                      return a.time < b.time;
                                  });
                        if (highScores.size() > 10)
                            highScores.resize(10);

                        saveScores("scores.txt", highScores);

                        state = GameState::Finished;
                    }
                }
            }
            else if (state == GameState::Finished) {
                if (event.type == sf::Event::KeyPressed &&
                    event.key.code == sf::Keyboard::R) {
                    state = GameState::Title;
                }
            }
        }

        float dt = clock.restart().asSeconds();
        if (dt > 0.05f) dt = 0.05f;

        // --------- LÓGICA DEL JUEGO ---------
        if (state == GameState::Playing) {
            timeElapsed += dt;
            totalRunTime += dt;

            Level &lvl = levels[currentLevelIndex];

            // Actualizar spikes móviles
            for (auto &spike : lvl.spikes) {
                if (spike.moving) {
                    float dx = spike.speed * dt * spike.direction;
                    spike.shape.move(dx, 0.f);
                    float x = spike.shape.getPosition().x;
                    if (x < spike.minX) {
                        spike.shape.setPosition(spike.minX, spike.shape.getPosition().y);
                        spike.direction = 1;
                    } else if (x > spike.maxX) {
                        spike.shape.setPosition(spike.maxX, spike.shape.getPosition().y);
                        spike.direction = -1;
                    }
                }
            }

            // Control horizontal
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
                velocity.x -= acceleration * dt * 60.0f;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
                velocity.x += acceleration * dt * 60.0f;

            // Salto
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && onGround) {
                velocity.y = jumpForce;
                onGround = false;
            }

            // Reset posición
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                ball.setPosition(lvl.spawn);
                velocity = sf::Vector2f(0.f, 0.f);
                onGround = false;
            }

            // Fricción
            velocity.x *= std::pow(friction, dt * 60.0f);

            // Limitar velocidad horizontal
            if (velocity.x > maxSpeed) velocity.x = maxSpeed;
            if (velocity.x < -maxSpeed) velocity.x = -maxSpeed;

            // Gravedad
            velocity.y += gravity * dt * 60.0f;

            // Actualizar posición
            ball.move(velocity * dt * 60.0f);

            onGround = false;

            // Colisión con suelo
            sf::FloatRect ballBounds = ball.getGlobalBounds();
            if (ballBounds.intersects(lvl.ground.bounds))
            {
                if (velocity.y > 0)
                {
                    ball.setPosition(ball.getPosition().x, lvl.ground.bounds.top - 25.f);
                    velocity.y = 0;
                    onGround = true;
                }
                ballBounds = ball.getGlobalBounds();
            }

            // Colisión con plataformas
            for (auto& platform : lvl.platforms)
            {
                sf::FloatRect pBounds = platform.bounds;
                if (ballBounds.intersects(pBounds))
                {
                    sf::Vector2f ballPos = ball.getPosition();

                    float overlapLeft   = (ballPos.x + 25.f) - pBounds.left;
                    float overlapRight  = (pBounds.left + pBounds.width) - (ballPos.x - 25.f);
                    float overlapTop    = (ballPos.y + 25.f) - pBounds.top;
                    float overlapBottom = (pBounds.top + pBounds.height) - (ballPos.y - 25.f);

                    float minOverlap = std::min({overlapLeft, overlapRight, overlapTop, overlapBottom});

                    if (minOverlap == overlapTop && velocity.y > 0)
                    {
                        ball.setPosition(ballPos.x, pBounds.top - 25.f);
                        velocity.y = 0;
                        onGround = true;
                    }
                    else if (minOverlap == overlapBottom && velocity.y < 0)
                    {
                        ball.setPosition(ballPos.x, pBounds.top + pBounds.height + 25.f);
                        velocity.y = 0;
                    }
                    else if (minOverlap == overlapLeft)
                    {
                        ball.setPosition(pBounds.left - 25.f, ballPos.y);
                        velocity.x = 0;
                    }
                    else if (minOverlap == overlapRight)
                    {
                        ball.setPosition(pBounds.left + pBounds.width + 25.f, ballPos.y);
                        velocity.x = 0;
                    }

                    ballBounds = ball.getGlobalBounds();
                }
            }

            // Bordes de ventana
            sf::Vector2f ballPos = ball.getPosition();
            if (ballPos.x < 25.f)
            {
                ball.setPosition(25.f, ballPos.y);
                velocity.x = 0;
            }
            if (ballPos.x > 775.f)
            {
                ball.setPosition(775.f, ballPos.y);
                velocity.x = 0;
            }

            // Colisión con obstáculos (bloques)
            ballBounds = ball.getGlobalBounds();
            for (const auto& obstacle : lvl.obstacles)
            {
                sf::FloatRect obsBounds = obstacle.getGlobalBounds();
                if (ballBounds.intersects(obsBounds))
                {
                    ball.setPosition(lvl.spawn);
                    velocity = sf::Vector2f(0.f, 0.f);
                    onGround = false;
                    break;
                }
            }

            // Colisión con picos
            ballBounds = ball.getGlobalBounds();
            for (const auto& spike : lvl.spikes)
            {
                sf::FloatRect sBounds = spike.shape.getGlobalBounds();
                if (ballBounds.intersects(sBounds))
                {
                    ball.setPosition(lvl.spawn);
                    velocity = sf::Vector2f(0.f, 0.f);
                    onGround = false;
                    break;
                }
            }

            // Caer fuera de pantalla
            if (ballPos.y > 650.f)
            {
                resetLevel(currentLevelIndex);
            }

            // Meta
            sf::Vector2f goalPos = lvl.goal.getPosition();
            ballPos = ball.getPosition();
            float distance = std::sqrt(std::pow(ballPos.x - goalPos.x, 2) +
                                       std::pow(ballPos.y - goalPos.y, 2));

            if (distance < 45.f)
            {
                state = GameState::LevelComplete;
                std::cout << "Nivel " << (currentLevelIndex + 1)
                          << " completado. Tiempo nivel: " << timeElapsed
                          << " s  | Tiempo total: " << totalRunTime << " s\n";
            }
        }

        // --------- DIBUJO ---------
        window.clear(sf::Color(135, 206, 235)); // cielo

        if (state == GameState::Title) {
            if (font.getInfo().family != "") {
                sf::Text title("BLUE BALLS", font, 72);
                title.setFillColor(sf::Color::Blue);
                title.setStyle(sf::Text::Bold);
                title.setPosition(0.f, 120.f);
                centerTextX(title, 800.f);

                sf::Text press("Presiona ENTER para jugar", font, 28);
                press.setFillColor(sf::Color::White);
                press.setPosition(0.f, 260.f);
                centerTextX(press, 800.f);

                sf::Text hint("Se registrara tu tiempo total para el ranking", font, 20);
                hint.setFillColor(sf::Color::White);
                hint.setPosition(0.f, 320.f);
                centerTextX(hint, 800.f);

                window.draw(title);
                window.draw(press);
                window.draw(hint);
            }
        }
        else if (state == GameState::EnterName) {
            if (font.getInfo().family != "") {
                sf::Text prompt("Escribe tu nombre (sin espacios) y presiona ENTER", font, 24);
                prompt.setFillColor(sf::Color::White);
                prompt.setPosition(0.f, 200.f);
                centerTextX(prompt, 800.f);

                sf::Text nameText(nameInputBuffer.empty() ? "_" : nameInputBuffer, font, 32);
                nameText.setFillColor(sf::Color::Yellow);
                nameText.setPosition(0.f, 260.f);
                centerTextX(nameText, 800.f);

                sf::Text escText("ESC para volver al titulo", font, 18);
                escText.setFillColor(sf::Color::White);
                escText.setPosition(0.f, 320.f);
                centerTextX(escText, 800.f);

                window.draw(prompt);
                window.draw(nameText);
                window.draw(escText);
            }
        }
        else {
            Level &lvl = levels[currentLevelIndex];

            window.draw(lvl.ground.shape);
            for (auto& p : lvl.platforms)  window.draw(p.shape);
            for (auto& o : lvl.obstacles)  window.draw(o);
            for (auto& s : lvl.spikes)     window.draw(s.shape);
            window.draw(lvl.goal);
            window.draw(ball);

            if (font.getInfo().family != "") {
                sf::Text levelText("Nivel " + std::to_string(currentLevelIndex + 1), font, 20);
                levelText.setFillColor(sf::Color::Black);
                levelText.setPosition(10.f, 10.f);
                window.draw(levelText);

                sf::Text timeText("Nivel: " +
                                  std::to_string(static_cast<int>(timeElapsed)) + " s", font, 18);
                timeText.setFillColor(sf::Color::Black);
                timeText.setPosition(10.f, 35.f);
                window.draw(timeText);

                sf::Text totalText("Total: " +
                                   std::to_string(static_cast<int>(totalRunTime)) + " s", font, 18);
                totalText.setFillColor(sf::Color::Black);
                totalText.setPosition(10.f, 55.f);
                window.draw(totalText);

                sf::Text resetHint("Presione \"D\" para reiniciar la posicion", font, 16);
                resetHint.setFillColor(sf::Color::Black);
                resetHint.setOutlineColor(sf::Color::White);
                resetHint.setOutlineThickness(1.f);
                sf::FloatRect hintBounds = resetHint.getLocalBounds();
                resetHint.setPosition(800.f - hintBounds.width - 10.f, 10.f);
                window.draw(resetHint);
            }

            if (state == GameState::LevelComplete && font.getInfo().family != "") {
                sf::Text msg("Nivel completado!", font, 40);
                msg.setFillColor(sf::Color::Yellow);
                msg.setOutlineColor(sf::Color::Black);
                msg.setOutlineThickness(2.f);
                msg.setPosition(0.f, 230.f);
                centerTextX(msg, 800.f);
                window.draw(msg);

                sf::Text press("Presiona ENTER para continuar", font, 24);
                press.setFillColor(sf::Color::White);
                press.setPosition(0.f, 290.f);
                centerTextX(press, 800.f);
                window.draw(press);
            }

            if (state == GameState::Finished && font.getInfo().family != "") {
                sf::Text msg("Has completado todos los niveles!", font, 32);
                msg.setFillColor(sf::Color::Yellow);
                msg.setOutlineColor(sf::Color::Black);
                msg.setOutlineThickness(2.f);
                msg.setPosition(0.f, 40.f);
                centerTextX(msg, 800.f);
                window.draw(msg);

                sf::Text press("Presiona R para volver al titulo", font, 22);
                press.setFillColor(sf::Color::White);
                press.setPosition(0.f, 90.f);
                centerTextX(press, 800.f);
                window.draw(press);

                sf::Text rankTitle("Ranking (mejores tiempos totales)", font, 24);
                rankTitle.setFillColor(sf::Color::White);
                rankTitle.setPosition(0.f, 150.f);
                centerTextX(rankTitle, 800.f);
                window.draw(rankTitle);

                int maxLines = static_cast<int>(std::min<std::size_t>(highScores.size(), 8));
                for (int i = 0; i < maxLines; ++i) {
                    const PlayerScore& ps = highScores[i];

                    std::string line =
                        std::to_string(i + 1) + ". " + ps.name +
                        " - " + std::to_string(static_cast<int>(ps.time)) + " s";

                    sf::Text lineText(line, font, 22);
                    if (!playerName.empty() &&
                        ps.name == playerName &&
                        std::fabs(ps.time - totalRunTime) < 0.5f) {
                        lineText.setFillColor(sf::Color::Yellow);
                    } else {
                        lineText.setFillColor(sf::Color::White);
                    }
                    lineText.setPosition(180.f, 190.f + i * 28.f);
                    window.draw(lineText);
                }
            }
        }

        window.display();
    }

    return 0;
}
