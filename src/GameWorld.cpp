#include "GameWorld.hpp"
#include "Platform.hpp"

// Constructor de Level
Level::Level(float gx, float gy, float gw, float gh)
    : ground(gx, gy, gw, gh) {}

// Nivel 1: muy básico
Level CreateLevel1() {
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
    level.spikes.push_back(CreateSpike(320.f, 370.f - 18.f, 40.f, 18.f));

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
Level CreateLevel2() {
    Level level(0.f, 560.f, 800.f, 40.f);
    level.ground.shape.setFillColor(sf::Color(34, 139, 34));
    level.ground.bounds = level.ground.shape.getGlobalBounds();

    level.platforms.emplace_back(80.f, 450.f, 140.f, 20.f);
    level.platforms.emplace_back(260.f, 420.f, 140.f, 20.f);
    level.platforms.emplace_back(440.f, 360.f, 140.f, 20.f);
    level.platforms.emplace_back(620.f, 300.f, 140.f, 20.f);

    // Obstáculo en suelo
    sf::RectangleShape obs1(sf::Vector2f(50.f, 80.f));
    obs1.setPosition(220.f, 480.f);
    obs1.setFillColor(sf::Color::Black);
    level.obstacles.push_back(obs1);

    // Pico estático sobre la primera plataforma
    level.spikes.push_back(CreateSpike(110.f, 450.f - 18.f, 40.f, 18.f));

    // Pico móvil sobre la tercera plataforma (se mueve de izquierda a derecha)
    level.spikes.push_back(
        CreateSpike(460.f, 360.f - 18.f, 40.f, 18.f,
                    true, 440.f, 540.f, 70.f) // minX, maxX, speed (ajustado para no salirse)
    );

    level.goal = sf::CircleShape(20.f, 5);
    level.goal.setOrigin(20.f, 20.f);
    level.goal.setPosition(720.f, 260.f);
    level.goal.setFillColor(sf::Color::Yellow);
    level.goal.setOutlineColor(sf::Color(255, 215, 0));
    level.goal.setOutlineThickness(3.f);

    level.spawn = sf::Vector2f(70.f, 540.f);

    return level;
}

// Nivel 3: torre sencilla con picos
Level CreateLevel3() {
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
    level.spikes.push_back(CreateSpike(290.f, 430.f - 18.f, 40.f, 18.f));

    // Pico móvil sobre la cuarta plataforma
    level.spikes.push_back(
        CreateSpike(600.f, 290.f - 20.f, 40.f, 20.f,
                    true, 580.f, 660.f, 80.f)
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
Level CreateLevel4() {
    Level level(0.f, 560.f, 800.f, 40.f);
    level.ground.shape.setFillColor(sf::Color(34, 139, 34));
    level.ground.bounds = level.ground.shape.getGlobalBounds();

    level.platforms.emplace_back(60.f, 450.f, 120.f, 20.f);
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
    level.spikes.push_back(CreateSpike(410.f, 380.f - 18.f, 40.f, 18.f));
    level.spikes.push_back(
        CreateSpike(560.f, 320.f - 18.f, 40.f, 18.f,
                    true, 540.f, 620.f, 90.f)
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
Level CreateLevel5() {
    Level level(0.f, 560.f, 800.f, 40.f);
    level.ground.shape.setFillColor(sf::Color(34, 139, 34));
    level.ground.bounds = level.ground.shape.getGlobalBounds();

    level.platforms.emplace_back(50.f, 440.f, 200.f, 20.f);
    level.platforms.emplace_back(320.f, 440.f, 160.f, 20.f);
    level.platforms.emplace_back(560.f, 440.f, 180.f, 20.f);

    level.platforms.emplace_back(200.f, 350.f, 120.f, 20.f);
    level.platforms.emplace_back(440.f, 320.f, 120.f, 20.f);

    // Bloque en hueco central
    sf::RectangleShape obs3(sf::Vector2f(60.f, 80.f));
    obs3.setPosition(300.f, 480.f);
    obs3.setFillColor(sf::Color::Black);
    level.obstacles.push_back(obs3);

    // Picos sobre plataformas superiores
    level.spikes.push_back(CreateSpike(230.f, 350.f - 18.f, 40.f, 18.f));
    level.spikes.push_back(
        CreateSpike(460.f, 320.f - 18.f, 40.f, 18.f,
                    true, 440.f, 520.f, 100.f)
    );

    level.goal = sf::CircleShape(20.f, 5);
    level.goal.setOrigin(20.f, 20.f);
    level.goal.setPosition(720.f, 400.f);
    level.goal.setFillColor(sf::Color::Yellow);
    level.goal.setOutlineColor(sf::Color(255, 215, 0));
    level.goal.setOutlineThickness(3.f);

    level.spawn = sf::Vector2f(80.f, 410.f);

    return level;
}

// Nivel 6: plataformas pequeñas
Level CreateLevel6() {
    Level level(0.f, 560.f, 800.f, 40.f);
    level.ground.shape.setFillColor(sf::Color(34, 139, 34));
    level.ground.bounds = level.ground.shape.getGlobalBounds();

    level.platforms.emplace_back(80.f, 450.f, 90.f, 20.f);
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
        CreateSpike(510.f, 290.f - 18.f, 40.f, 18.f,
                    true, 500.f, 550.f, 110.f)
    );

    // Pico estático sobre la tercera
    level.spikes.push_back(CreateSpike(380.f, 360.f - 18.f, 40.f, 18.f));

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
Level CreateLevel7() {
    Level level(0.f, 560.f, 800.f, 40.f);
    level.ground.shape.setFillColor(sf::Color(34, 139, 34));
    level.ground.bounds = level.ground.shape.getGlobalBounds();

    level.platforms.emplace_back(60.f, 440.f, 140.f, 20.f);
    level.platforms.emplace_back(260.f, 440.f, 140.f, 20.f);
    level.platforms.emplace_back(460.f, 440.f, 140.f, 20.f);

    level.platforms.emplace_back(200.f, 380.f, 120.f, 20.f);
    level.platforms.emplace_back(400.f, 340.f, 120.f, 20.f);
    level.platforms.emplace_back(610.f, 300.f, 120.f, 20.f);

    sf::RectangleShape obs1(sf::Vector2f(60.f, 80.f));
    obs1.setPosition(340.f, 480.f);
    obs1.setFillColor(sf::Color::Black);
    level.obstacles.push_back(obs1);

    // Picos sobre plataformas altas
    level.spikes.push_back(CreateSpike(230.f, 380.f - 18.f, 40.f, 18.f));
    level.spikes.push_back(
        CreateSpike(430.f, 340.f - 18.f, 40.f, 18.f,
                    true, 400.f, 480.f, 120.f)
    );
    level.spikes.push_back(CreateSpike(640.f, 300.f - 18.f, 40.f, 18.f));

    level.goal = sf::CircleShape(20.f, 5);
    level.goal.setOrigin(20.f, 20.f);
    level.goal.setPosition(720.f, 260.f);
    level.goal.setFillColor(sf::Color::Yellow);
    level.goal.setOutlineColor(sf::Color(255, 215, 0));
    level.goal.setOutlineThickness(3.f);

    level.spawn = sf::Vector2f(80.f, 540.f);

    return level;
}

// Nivel 8: camino largo con techos peligrosos
Level CreateLevel8() {
    Level level(0.f, 560.f, 800.f, 40.f);
    level.ground.shape.setFillColor(sf::Color(34, 139, 34));
    level.ground.bounds = level.ground.shape.getGlobalBounds();

    level.platforms.emplace_back(150.f, 450.f, 160.f, 20.f);
    level.platforms.emplace_back(320.f, 430.f, 160.f, 20.f);
    level.platforms.emplace_back(490.f, 360.f, 160.f, 20.f);
    level.platforms.emplace_back(660.f, 290.f, 120.f, 20.f);

    // Bloque en extremo derecho
    sf::RectangleShape obsFloor(sf::Vector2f(60.f, 80.f));
    obsFloor.setPosition(700.f, 480.f);
    obsFloor.setFillColor(sf::Color::Black);
    level.obstacles.push_back(obsFloor);

    // Picos sobre segunda y tercera plataformas
    level.spikes.push_back(CreateSpike(360.f, 430.f - 18.f, 40.f, 18.f));
    level.spikes.push_back(
        CreateSpike(520.f, 360.f - 18.f, 40.f, 18.f,
                    true, 490.f, 610.f, 130.f)
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
Level CreateLevel9() {
    Level level(0.f, 560.f, 800.f, 40.f);
    level.ground.shape.setFillColor(sf::Color(34, 139, 34));
    level.ground.bounds = level.ground.shape.getGlobalBounds();

    level.platforms.emplace_back(80.f, 440.f, 120.f, 20.f);
    level.platforms.emplace_back(260.f, 440.f, 120.f, 20.f);
    level.platforms.emplace_back(440.f, 440.f, 120.f, 20.f);

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
    level.spikes.push_back(CreateSpike(180.f, 420.f - 18.f, 40.f, 18.f));
    level.spikes.push_back(
        CreateSpike(370.f, 380.f - 18.f, 40.f, 18.f,
                    true, 350.f, 410.f, 130.f)
    );
    level.spikes.push_back(CreateSpike(480.f, 260.f - 18.f, 40.f, 18.f));

    level.goal = sf::CircleShape(20.f, 5);
    level.goal.setOrigin(20.f, 20.f);
    level.goal.setPosition(720.f, 220.f);
    level.goal.setFillColor(sf::Color::Yellow);
    level.goal.setOutlineColor(sf::Color(255, 215, 0));
    level.goal.setOutlineThickness(3.f);

    level.spawn = sf::Vector2f(90.f, 540.f);

    return level;
}

// Nivel 10: final vertical
Level CreateLevel10() {
    Level level(0.f, 560.f, 800.f, 40.f);
    level.ground.shape.setFillColor(sf::Color(34, 139, 34));
    level.ground.bounds = level.ground.shape.getGlobalBounds();

    level.platforms.emplace_back(80.f, 450.f, 90.f, 20.f);
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
    level.spikes.push_back(CreateSpike(220.f, 440.f - 18.f, 40.f, 18.f));
    level.spikes.push_back(CreateSpike(340.f, 380.f - 18.f, 40.f, 18.f));
    level.spikes.push_back(
        CreateSpike(460.f, 320.f - 18.f, 40.f, 18.f,
                    true, 440.f, 490.f, 150.f)
    );
    level.spikes.push_back(
        CreateSpike(580.f, 260.f - 18.f, 40.f, 18.f,
                    true, 560.f, 610.f, 170.f)
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
