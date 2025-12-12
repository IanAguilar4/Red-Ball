#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include "GameState.hpp"
#include "Platform.hpp"
#include "GameWorld.hpp"
#include "PlayerScore.hpp"
#include "Utils.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Blue Balls - Niveles");

    // Cargar fuente
    sf::Font font;
    if (!font.loadFromFile("assets/fonts/arialbd.ttf")) {
        std::cout << "Error: No se pudo cargar la fuente (assets/fonts/arialbd.ttf)" << std::endl;
    }

    // Textura para la bola
    sf::Texture ballTexture;
    if (!ballTexture.loadFromFile("assets/sprites/blueface.png")) {
        std::cout << "Error: No se pudo cargar la textura de la bola (assets/sprites/blueface.png)" << std::endl;
    } else {
        ballTexture.setSmooth(true);
    }

    // Scores
    std::vector<PlayerScore> highScores;
    loadScores("scores.txt", highScores);

    std::string playerName;
    std::string nameInputBuffer;

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
    ball.setFillColor(sf::Color::White);
    ball.setOutlineColor(sf::Color::Transparent);
    ball.setTexture(&ballTexture);

    // Física
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
    float timeElapsed = 0.0f;
    float totalRunTime = 0.0f;

    auto resetLevel = [&](int index) {
        currentLevelIndex = index;
        Level &lvl = levels[currentLevelIndex];
        ball.setPosition(lvl.spawn);
        velocity = sf::Vector2f(0.f, 0.f);
        onGround = false;
        timeElapsed = 0.0f;
        clock.restart();
    };

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
                    if (event.text.unicode == 8) {
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

        // LÓGICA DEL JUEGO
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

            // Colisión con obstáculos (bloques mortales)
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

            // Colisión con picos (mortales)
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

        // DIBUJO
        window.clear(sf::Color(135, 206, 235));

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
