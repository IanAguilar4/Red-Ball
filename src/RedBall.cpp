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

// Estructura para representar un nivel completo
struct Level {
    Platform ground;                         // suelo principal
    std::vector<Platform> platforms;         // plataformas elevadas
    std::vector<sf::RectangleShape> obstacles; // obstáculos decorativos
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

// --- Definición de niveles ---

Level createLevel1() {
    Level level(0.f, 560.f, 800.f, 40.f);
    level.ground.shape.setFillColor(sf::Color(34, 139, 34));
    level.ground.bounds = level.ground.shape.getGlobalBounds();

    level.platforms.emplace_back(50.f, 450.f, 160.f, 20.f);
    level.platforms.emplace_back(250.f, 350.f, 160.f, 20.f);
    level.platforms.emplace_back(450.f, 250.f, 160.f, 20.f);
    level.platforms.emplace_back(650.f, 180.f, 100.f, 20.f);

    sf::RectangleShape obs1(sf::Vector2f(60.f, 60.f));
    obs1.setPosition(300.f, 500.f);
    obs1.setFillColor(sf::Color::Black);
    level.obstacles.push_back(obs1);

    sf::RectangleShape obs2(sf::Vector2f(60.f, 60.f));
    obs2.setPosition(500.f, 500.f);
    obs2.setFillColor(sf::Color::Black);
    level.obstacles.push_back(obs2);

    level.goal = sf::CircleShape(20.f, 5);
    level.goal.setOrigin(20.f, 20.f);
    level.goal.setPosition(700.f, 140.f);
    level.goal.setFillColor(sf::Color::Yellow);
    level.goal.setOutlineColor(sf::Color(255, 215, 0));
    level.goal.setOutlineThickness(3.f);

    level.spawn = sf::Vector2f(100.f, 200.f);

    return level;
}

Level createLevel2() {
    Level level(0.f, 560.f, 800.f, 40.f);
    level.ground.shape.setFillColor(sf::Color(34, 139, 34));
    level.ground.bounds = level.ground.shape.getGlobalBounds();

    level.platforms.emplace_back(80.f, 480.f, 140.f, 20.f);
    level.platforms.emplace_back(260.f, 420.f, 140.f, 20.f);
    level.platforms.emplace_back(440.f, 360.f, 140.f, 20.f);
    level.platforms.emplace_back(620.f, 300.f, 140.f, 20.f);

    sf::RectangleShape obs1(sf::Vector2f(50.f, 80.f));
    obs1.setPosition(200.f, 480.f);
    obs1.setFillColor(sf::Color::Black);
    level.obstacles.push_back(obs1);

    sf::RectangleShape obs2(sf::Vector2f(50.f, 80.f));
    obs2.setPosition(420.f, 480.f);
    obs2.setFillColor(sf::Color::Black);
    level.obstacles.push_back(obs2);

    level.goal = sf::CircleShape(20.f, 5);
    level.goal.setOrigin(20.f, 20.f);
    level.goal.setPosition(700.f, 260.f);
    level.goal.setFillColor(sf::Color::Yellow);
    level.goal.setOutlineColor(sf::Color(255, 215, 0));
    level.goal.setOutlineThickness(3.f);

    level.spawn = sf::Vector2f(70.f, 500.f);

    return level;
}

Level createLevel3() {
    Level level(0.f, 560.f, 800.f, 40.f);
    level.ground.shape.setFillColor(sf::Color(34, 139, 34));
    level.ground.bounds = level.ground.shape.getGlobalBounds();

    level.platforms.emplace_back(100.f, 500.f, 120.f, 20.f);
    level.platforms.emplace_back(260.f, 430.f, 120.f, 20.f);
    level.platforms.emplace_back(420.f, 360.f, 120.f, 20.f);
    level.platforms.emplace_back(580.f, 290.f, 120.f, 20.f);
    level.platforms.emplace_back(580.f, 210.f, 120.f, 20.f);

    sf::RectangleShape obs1(sf::Vector2f(60.f, 60.f));
    obs1.setPosition(350.f, 500.f);
    obs1.setFillColor(sf::Color::Black);
    level.obstacles.push_back(obs1);

    level.goal = sf::CircleShape(20.f, 5);
    level.goal.setOrigin(20.f, 20.f);
    level.goal.setPosition(640.f, 170.f);
    level.goal.setFillColor(sf::Color::Yellow);
    level.goal.setOutlineColor(sf::Color(255, 215, 0));
    level.goal.setOutlineThickness(3.f);

    level.spawn = sf::Vector2f(80.f, 520.f);

    return level;
}

// --- Juego Red Ball con niveles y ranking ---

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Red Ball - Niveles");

    // Cargar fuente
    sf::Font font;
    if (!font.loadFromFile("assets/fonts/arialbd.ttf")) {
        std::cout << "Error: No se pudo cargar la fuente (assets/fonts/arialbd.ttf)" << std::endl;
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

    int currentLevelIndex = 0;

    // Jugador
    sf::CircleShape ball(25.f);
    ball.setOrigin(25.f, 25.f);
    ball.setFillColor(sf::Color::Red);
    ball.setOutlineColor(sf::Color(139, 0, 0));
    ball.setOutlineThickness(2.f);

    // Física simple (la que ya tenías)
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

            // Título -> ingresar nombre
            if (state == GameState::Title) {
                if (event.type == sf::Event::KeyPressed &&
                    (event.key.code == sf::Keyboard::Enter ||
                     event.key.code == sf::Keyboard::Space)) {
                    nameInputBuffer.clear();
                    state = GameState::EnterName;
                }
            }
            // Pantalla de nombre
            else if (state == GameState::EnterName) {
                if (event.type == sf::Event::TextEntered) {
                    char c = static_cast<char>(event.text.unicode);
                    // Aceptar caracteres imprimibles sin espacios (para no complicar el archivo)
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
            // Nivel completado (esperando Enter para siguiente)
            else if (state == GameState::LevelComplete) {
                if (event.type == sf::Event::KeyPressed &&
                    (event.key.code == sf::Keyboard::Enter ||
                     event.key.code == sf::Keyboard::Space)) {
                    if (currentLevelIndex + 1 < static_cast<int>(levels.size())) {
                        resetLevel(currentLevelIndex + 1);
                        state = GameState::Playing;
                    } else {
                        // Partida terminada: registrar score
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
            // Pantalla final (ranking)
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

            // Control horizontal simple
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
                velocity.x -= acceleration * dt * 60.0f;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
                velocity.x += acceleration * dt * 60.0f;

            // Salto
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && onGround) {
                velocity.y = jumpForce;
                onGround = false;
            }

            // Resetear posición con D (sin reiniciar timer)
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

            // Reset onGround
            onGround = false;

            // Colisión con el suelo
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
                    sf::Vector2f platformPos = platform.shape.getPosition();
                    sf::Vector2f platformSize = platform.shape.getSize();

                    // Calcular el centro de la plataforma y la bola
                    float ballCenterY = ballPos.y;
                    float platformCenterY = platformPos.y + platformSize.y / 2.0f;
                    float ballCenterX = ballPos.x;
                    float platformCenterX = platformPos.x + platformSize.x / 2.0f;

                    // Calcular la penetración en cada dirección
                    float overlapLeft = (ballPos.x + 25.f) - pBounds.left;
                    float overlapRight = (pBounds.left + pBounds.width) - (ballPos.x - 25.f);
                    float overlapTop = (ballPos.y + 25.f) - pBounds.top;
                    float overlapBottom = (pBounds.top + pBounds.height) - (ballPos.y - 25.f);

                    // Encontrar la menor penetración para determinar el lado de la colisión
                    float minOverlap = std::min({overlapLeft, overlapRight, overlapTop, overlapBottom});

                    // Resolver la colisión basándose en la menor penetración
                    if (minOverlap == overlapTop && velocity.y > 0)
                    {
                        // Colisión desde arriba
                        ball.setPosition(ballPos.x, pBounds.top - 25.f);
                        velocity.y = 0;
                        onGround = true;
                    }
                    else if (minOverlap == overlapBottom && velocity.y < 0)
                    {
                        // Colisión desde abajo
                        ball.setPosition(ballPos.x, pBounds.top + pBounds.height + 25.f);
                        velocity.y = 0;
                    }
                    else if (minOverlap == overlapLeft)
                    {
                        // Colisión desde la izquierda
                        ball.setPosition(pBounds.left - 25.f, ballPos.y);
                        velocity.x = 0;
                    }
                    else if (minOverlap == overlapRight)
                    {
                        // Colisión desde la derecha
                        ball.setPosition(pBounds.left + pBounds.width + 25.f, ballPos.y);
                        velocity.x = 0;
                    }

                    ballBounds = ball.getGlobalBounds();
                }
            }

            // Bordes de la ventana
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

            // Colisión con obstáculos mortales (cuadrados negros)
            for (const auto& obstacle : lvl.obstacles)
            {
                sf::FloatRect obsBounds = obstacle.getGlobalBounds();
                if (ballBounds.intersects(obsBounds))
                {
                    // Obstáculo mortal: resetear posición
                    ball.setPosition(lvl.spawn);
                    velocity = sf::Vector2f(0.f, 0.f);
                    onGround = false;
                    break;
                }
            }

            // Caer fuera de pantalla: reinicia nivel pero mantiene totalRunTime
            if (ballPos.y > 650.f)
            {
                resetLevel(currentLevelIndex);
            }

            // Verificar meta
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
                sf::Text title("RED BALL", font, 72);
                title.setFillColor(sf::Color::Red);
                title.setStyle(sf::Text::Bold);
                title.setPosition(220.f, 120.f);

                sf::Text press("Presiona ENTER para jugar", font, 28);
                press.setFillColor(sf::Color::White);
                press.setPosition(170.f, 260.f);

                sf::Text hint("Se registrara tu tiempo total para el ranking", font, 20);
                hint.setFillColor(sf::Color::White);
                hint.setPosition(130.f, 320.f);

                window.draw(title);
                window.draw(press);
                window.draw(hint);
            }
        }
        else if (state == GameState::EnterName) {
            if (font.getInfo().family != "") {
                sf::Text prompt("Escribe tu nombre (sin espacios) y presiona ENTER", font, 24);
                prompt.setFillColor(sf::Color::White);
                prompt.setPosition(40.f, 200.f);

                sf::Text nameText(nameInputBuffer.empty() ? "_" : nameInputBuffer, font, 32);
                nameText.setFillColor(sf::Color::Yellow);
                nameText.setPosition(200.f, 260.f);

                sf::Text escText("ESC para volver al titulo", font, 18);
                escText.setFillColor(sf::Color::White);
                escText.setPosition(260.f, 320.f);

                window.draw(prompt);
                window.draw(nameText);
                window.draw(escText);
            }
        }
        else {
            Level &lvl = levels[currentLevelIndex];

            // Dibujar nivel
            window.draw(lvl.ground.shape);
            for (auto& p : lvl.platforms)  window.draw(p.shape);
            for (auto& o : lvl.obstacles)  window.draw(o);
            window.draw(lvl.goal);
            window.draw(ball);

            // HUD
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

                // Leyenda para resetear posición
                sf::Text resetHint("Presione \"D\" para reiniciar la posicion", font, 16);
                resetHint.setFillColor(sf::Color::Black);
                resetHint.setOutlineColor(sf::Color::White);
                resetHint.setOutlineThickness(1.f);
                sf::FloatRect hintBounds = resetHint.getLocalBounds();
                resetHint.setPosition(800.f - hintBounds.width - 10.f, 10.f);
                window.draw(resetHint);
            }

            // Mensajes de estado
            if (state == GameState::LevelComplete && font.getInfo().family != "") {
                sf::Text msg("Nivel completado!", font, 40);
                msg.setFillColor(sf::Color::Yellow);
                msg.setOutlineColor(sf::Color::Black);
                msg.setOutlineThickness(2.f);
                msg.setPosition(200.f, 230.f);
                window.draw(msg);

                sf::Text press("Presiona ENTER para continuar", font, 24);
                press.setFillColor(sf::Color::White);
                press.setPosition(190.f, 290.f);
                window.draw(press);
            }

            if (state == GameState::Finished && font.getInfo().family != "") {
                sf::Text msg("Has completado todos los niveles!", font, 32);
                msg.setFillColor(sf::Color::Yellow);
                msg.setOutlineColor(sf::Color::Black);
                msg.setOutlineThickness(2.f);
                msg.setPosition(90.f, 40.f);
                window.draw(msg);

                sf::Text press("Presiona R para volver al titulo", font, 22);
                press.setFillColor(sf::Color::White);
                press.setPosition(200.f, 90.f);
                window.draw(press);

                // Tabla de ranking
                sf::Text rankTitle("Ranking (mejores tiempos totales)", font, 24);
                rankTitle.setFillColor(sf::Color::White);
                rankTitle.setPosition(130.f, 150.f);
                window.draw(rankTitle);

                int maxLines = static_cast<int>(std::min<std::size_t>(highScores.size(), 8));
                for (int i = 0; i < maxLines; ++i) {
                    const PlayerScore& ps = highScores[i];

                    std::string line =
                        std::to_string(i + 1) + ". " + ps.name +
                        " - " + std::to_string(static_cast<int>(ps.time)) + " s";

                    sf::Text lineText(line, font, 22);

                    // Resaltar al jugador actual si coincide nombre y tiempo (aprox)
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
