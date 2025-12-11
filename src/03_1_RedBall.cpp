#include <SFML/Graphics.hpp>

// Ejemplo: Control avanzado de la bola con física simple
int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Red Ball - Control Avanzado");

    // Crear la bola roja
    sf::CircleShape redBall(30.f);
    redBall.setFillColor(sf::Color::Red);
    redBall.setOrigin(30.f, 30.f);
    redBall.setPosition(400.f, 500.f);

    // Variables de física simple
    float velocityX = 0.0f;
    float velocityY = 0.0f;
    float acceleration = 0.5f;
    float friction = 0.95f;
    float gravity = 0.3f;
    bool onGround = true;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Control de movimiento horizontal
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            velocityX -= acceleration;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            velocityX += acceleration;
        
        // Salto
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && onGround)
        {
            velocityY = -15.0f;
            onGround = false;
        }

        // Aplicar gravedad
        velocityY += gravity;

        // Aplicar fricción
        velocityX *= friction;

        // Actualizar posición
        redBall.move(velocityX, velocityY);

        // Colisión con el suelo
        if (redBall.getPosition().y >= 530.f)
        {
            redBall.setPosition(redBall.getPosition().x, 530.f);
            velocityY = 0;
            onGround = true;
        }

        // Colisión con los bordes laterales
        if (redBall.getPosition().x < 30.f)
            redBall.setPosition(30.f, redBall.getPosition().y);
        if (redBall.getPosition().x > 770.f)
            redBall.setPosition(770.f, redBall.getPosition().y);

        window.clear(sf::Color(135, 206, 235));
        
        // Dibujar el suelo
        sf::RectangleShape ground(sf::Vector2f(800.f, 40.f));
        ground.setPosition(0.f, 560.f);
        ground.setFillColor(sf::Color(34, 139, 34));
        window.draw(ground);
        
        window.draw(redBall);
        window.display();
    }

    return 0;
}
