#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>

// Estructura para representar una plataforma
struct Platform {
    sf::RectangleShape shape;
    sf::FloatRect bounds;
    
    Platform(float x, float y, float w, float h) {
        shape.setSize(sf::Vector2f(w, h));
        shape.setPosition(x, y);
        shape.setFillColor(sf::Color(139, 69, 19));
        bounds = shape.getGlobalBounds();
    }
};

// Juego completo de Red Ball con física simple
int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Red Ball - Juego Completo");
    
    // Crear la bola roja (jugador)
    sf::CircleShape ball(25.f);
    ball.setOrigin(25.f, 25.f);
    ball.setFillColor(sf::Color::Red);
    ball.setOutlineColor(sf::Color(139, 0, 0));
    ball.setOutlineThickness(2.f);
    ball.setPosition(100.f, 200.f);
    
    // Variables de física
    sf::Vector2f velocity(0.f, 0.f);
    const float gravity = 0.5f;
    const float acceleration = 0.8f;
    const float maxSpeed = 8.0f;
    const float jumpForce = -15.0f;
    const float friction = 0.85f;
    bool onGround = false;
    
    // Crear suelo
    Platform ground(0.f, 560.f, 800.f, 40.f);
    ground.shape.setFillColor(sf::Color(34, 139, 34));
    
    // Crear plataformas
    std::vector<Platform> platforms;
    platforms.push_back(Platform(50.f, 450.f, 160.f, 20.f));
    platforms.push_back(Platform(250.f, 350.f, 160.f, 20.f));
    platforms.push_back(Platform(450.f, 250.f, 160.f, 20.f));
    platforms.push_back(Platform(650.f, 180.f, 100.f, 20.f));
    
    // Crear obstáculos
    std::vector<sf::RectangleShape> obstacles;
    sf::RectangleShape obs1(sf::Vector2f(60.f, 60.f));
    obs1.setPosition(300.f, 500.f);
    obs1.setFillColor(sf::Color::Black);
    obstacles.push_back(obs1);
    
    sf::RectangleShape obs2(sf::Vector2f(60.f, 60.f));
    obs2.setPosition(500.f, 500.f);
    obs2.setFillColor(sf::Color::Black);
    obstacles.push_back(obs2);
    
    // Crear objetivo (estrella)
    sf::CircleShape goal(20.f, 5);
    goal.setOrigin(20.f, 20.f);
    goal.setPosition(700.f, 140.f);
    goal.setFillColor(sf::Color::Yellow);
    goal.setOutlineColor(sf::Color(255, 215, 0));
    goal.setOutlineThickness(3.f);
    
    bool gameWon = false;
    sf::Clock clock;
    float timeElapsed = 0.0f;
    
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            
            // Salto
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Space && onGround)
                {
                    velocity.y = jumpForce;
                    onGround = false;
                }
                
                // Reiniciar con R
                if (event.key.code == sf::Keyboard::R)
                {
                    ball.setPosition(100.f, 200.f);
                    velocity = sf::Vector2f(0.f, 0.f);
                    gameWon = false;
                    timeElapsed = 0.0f;
                }
            }
        }
        
        if (!gameWon)
        {
            // Control horizontal
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
                velocity.x -= acceleration;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
                velocity.x += acceleration;
            
            // Aplicar fricción
            velocity.x *= friction;
            
            // Limitar velocidad horizontal
            if (velocity.x > maxSpeed) velocity.x = maxSpeed;
            if (velocity.x < -maxSpeed) velocity.x = -maxSpeed;
            
            // Aplicar gravedad
            velocity.y += gravity;
            
            // Actualizar posición
            ball.move(velocity);
            
            // Reset onGround
            onGround = false;
            
            // Colisión con el suelo
            sf::FloatRect ballBounds = ball.getGlobalBounds();
            if (ballBounds.intersects(ground.bounds))
            {
                if (velocity.y > 0)
                {
                    ball.setPosition(ball.getPosition().x, ground.bounds.top - 25.f);
                    velocity.y = 0;
                    onGround = true;
                }
            }
            
            // Colisión con plataformas
            for (auto& platform : platforms)
            {
                if (ballBounds.intersects(platform.bounds))
                {
                    sf::Vector2f ballPos = ball.getPosition();
                    sf::Vector2f platformPos = platform.shape.getPosition();
                    sf::Vector2f platformSize = platform.shape.getSize();
                    
                    // Colisión desde arriba
                    if (velocity.y > 0 && ballPos.y < platformPos.y + platformSize.y / 2)
                    {
                        ball.setPosition(ballPos.x, platform.bounds.top - 25.f);
                        velocity.y = 0;
                        onGround = true;
                    }
                    // Colisión desde abajo
                    else if (velocity.y < 0 && ballPos.y > platformPos.y + platformSize.y / 2)
                    {
                        ball.setPosition(ballPos.x, platform.bounds.top + platform.bounds.height + 25.f);
                        velocity.y = 0;
                    }
                    // Colisión lateral
                    else
                    {
                        if (ballPos.x < platformPos.x)
                            ball.setPosition(platform.bounds.left - 25.f, ballPos.y);
                        else
                            ball.setPosition(platform.bounds.left + platform.bounds.width + 25.f, ballPos.y);
                        velocity.x = 0;
                    }
                }
            }
            
            // Colisión con los bordes de la ventana
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
            
            // Evitar caer fuera de la pantalla
            if (ballPos.y > 650.f)
            {
                ball.setPosition(100.f, 200.f);
                velocity = sf::Vector2f(0.f, 0.f);
            }
            
            // Verificar colisión con objetivo
            sf::Vector2f goalPos = goal.getPosition();
            ballPos = ball.getPosition();
            float distance = std::sqrt(std::pow(ballPos.x - goalPos.x, 2) + std::pow(ballPos.y - goalPos.y, 2));
            
            if (distance < 45.f)
            {
                gameWon = true;
                std::cout << "¡Ganaste! Tiempo: " << timeElapsed << " segundos" << std::endl;
            }
            
            timeElapsed += clock.restart().asSeconds();
        }
        else
        {
            clock.restart();
        }
        
        // Limpiar ventana con color de cielo
        window.clear(sf::Color(135, 206, 235));
        
        // Dibujar suelo
        window.draw(ground.shape);
        
        // Dibujar plataformas
        for (auto& platform : platforms)
        {
            window.draw(platform.shape);
        }
        
        // Dibujar obstáculos
        for (auto& obstacle : obstacles)
        {
            window.draw(obstacle);
        }
        
        // Dibujar objetivo (estrella)
        window.draw(goal);
        
        // Dibujar la bola roja
        window.draw(ball);
        
        // Mensaje de victoria
        if (gameWon)
        {
            sf::Font font;
            // Intentar cargar fuente si existe
            if (font.loadFromFile("assets/fonts/arial.ttf"))
            {
                sf::Text winText;
                winText.setFont(font);
                winText.setString("¡GANASTE!");
                winText.setCharacterSize(72);
                winText.setFillColor(sf::Color::Green);
                winText.setOutlineColor(sf::Color::White);
                winText.setOutlineThickness(3.f);
                winText.setPosition(250.f, 250.f);
                window.draw(winText);
                
                sf::Text resetText;
                resetText.setFont(font);
                resetText.setString("Presiona R para reiniciar");
                resetText.setCharacterSize(24);
                resetText.setFillColor(sf::Color::White);
                resetText.setPosition(270.f, 350.f);
                window.draw(resetText);
            }
        }
        
        window.display();
    }
    
    return 0;
}
